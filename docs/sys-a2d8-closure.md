# A2D8 mechanical closure

Status: **mechanical confidence A**. Original vendor name unknown.

## Neutral contract

```c
uint32_t SYS_A2D8(const void *state, uint8_t post_flag);
```

Two physical 32-bit stack slots are consumed. `state` is mandatory. Only the low byte of the second slot is used. The top-level container is read-only directly, while the object reached through its first pointer field is mutated.

## Handler evidence

A2D8 is exactly 0x96 bytes in all three canonical ROMs:

- AS3000 2005: `0x004D5AFA`, file `+0x015AFA`.
- NEO 2005: `0x005D7DE0`, file `+0x017DE0`.
- NEO 2013: `0x00429818`, file `+0x019818`.

Each handler has one RTS at `+0x94`; all branch targets remain inside the body. The only cross-ROM differences are four relocated absolute helper operands. Neutralizing them produces common SHA-256:

`6d791ab5c21b4cc32e2b39f9114bd7edad2030a6525f09a6474ebe7d3e6e1974`

## State effects

The handler copies `state->field4` into offset `+0x24` of the object reached through `state->field0`. It then compares/clamps that current field against longwords at `+0x28` and `+0x2C`, consults a helper once or twice, and may refresh `+0x24` from that helper.

A non-zero word at pointee `+0x4A` bypasses the normal helper sequence and returns zero immediately.

## Return behavior

The normal path invokes the same secondary helper correlated during A2D0 analysis with the physical shape equivalent to `[state->field0, 1]`. A2D8 deliberately saves that helper's `D0.L` in `D7` and copies `D7` back to `D0` at the epilogue.

When the saved status is zero and `post_flag != 0`, an additional helper is called. That helper's return is ignored and the previously saved status remains the A2D8 result.

The correlated secondary helper has a single final `MOVEQ #0,D0` exit in AS3000 2005, NEO 2005 and NEO 2013. Combined with the explicit zero early-exit, the exact reachable A2D8 return domain in those firmware images is therefore the singleton:

```text
0x00000000
```

The return is still modeled as a full 32-bit status rather than `void`: the firmware constructs it deliberately on both control-flow families, including an explicit `D0 -> D7 -> D0` preservation sequence on the normal path. This differs materially from residual/staging-only cases such as the unresolved A2B0 return intention.

## Callers and xrefs

Direct ROM-wide searches for absolute `JSR`, absolute `JMP`, and direct `BSR.W` references to A2D8 are negative in all three canonical ROMs. SmartApplet occurrences inspected are the standard contiguous syscall table, except an additional Thesaurus Small byte match that lies in data rather than confirmed 68000 code. No independent A2D8 prototype or vendor symbol was recovered from BetaWise or `neo-re`.

## Cross-generation comparison

The complete external handler is instruction-identical after relocating four helper addresses. The external two-slot ABI, state-field mechanics and reachable return domain are common to all three compared generations.

## Adversarial review

- The function is not modeled as `void`; `D0.L` is explicitly constructed and propagated.
- No third external argument is claimed because no later stack slot is read.
- The optional helper's return is not exposed.
- `const` on `state` does not imply absence of side effects on the transitively referenced object.
- No descriptive vendor name is invented from neighboring routines.

## Regression status

Emulator-first regression is **specified, not executed**. Tests should cover the `+0x4A` guard, in-range/out-of-range `+0x24` clamp behavior, one- and two-helper refresh paths, `post_flag` zero/non-zero, upper-bit invariance of the second physical slot, and `D0.L == 0` across AS3000 2005, NEO 2005 and NEO 2013.

## 2026-09-04 source-first re-audit

Canonical ROM identities and exact A2D8 handler fingerprints reproduced 3/3: AS3000 `3950b2ae056e7f047fe6765add3e55b13a0a2bce3cfeca0f1bde47385c16e3df`, NEO 2005 `4fe6a880c92dbd2891aa2be9d85d3cdf386d2bc249cd82bb1b6c400881567999`, and NEO 2013 `c29f0e3e409d7b565624e4ac36f78fac45dd6027b6efa39306c28fee7aca4477`.

A fresh aligned whole-ROM control-transfer scan reproduced zero absolute `JSR`, zero absolute `JMP`, and zero direct `BSR.W` callers in every generation. The 0x96-byte lengths and terminal `RTS` at `+0x94` also reproduced 3/3. Static structural regression: **12/12 PASS** (3 canonical ROM identities, 3 exact handler fingerprints, 3 terminal size/epilogue checks, 3 negative direct-xref sets).

No source or manual evidence contradicted the two-slot contract, transitive mutation model, or singleton reachable `D0.L` domain `{0}`. Dynamic/emulator regression remains **specified / not executed**.

Private per-ROM disassemblies and ROM-derived workpapers are retained outside the public repository.
