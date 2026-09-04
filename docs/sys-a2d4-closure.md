# A2D4 mechanical closure

Status: **mechanical confidence A**. Original vendor name unknown.

## Public neutral contract

```c
uint32_t SYS_A2D4(const void *state,
                  const uint8_t *source,
                  uint32_t value,
                  uint8_t flags);
```

The four ABI arguments occupy 32-bit physical stack slots. `state` is mandatory. `source` is consumed as a full pointer and is dereferenced by the internal worker. `value` is a full 32-bit scalar. Only the low byte of `flags` is consumed.

## Firmware evidence

The handler is exactly 0x5E bytes in all three canonical firmware images:

- AS3000 2005: runtime `0x004D5A9C`, file `+0x015A9C`.
- NEO 2005: runtime `0x005D7D82`, file `+0x017D82`.
- NEO 2013: runtime `0x004297BA`, file `+0x0197BA`.

It contains one RTS at `+0x5C`; all branches remain inside the function. The three bodies differ only in two relocated absolute JSR operands. After neutralizing those relocations the common SHA-256 is:

`33d67efaf45128af92d39b4c4699f89b5f887bab1c8e90fb1bffa3d823ea1a2e`

## Control flow and return contract

The first field of `state` is treated as a pointer. A non-zero word at pointee `+0x4A` returns `0x02000005`. Otherwise a first helper result is compared with the word at pointee `+0x00`; mismatch returns `0x02000006`.

The remaining path calls a six-slot worker with the mechanically reconstructed shape:

```text
[state->field0,
 state->field4,
 source,
 value,
 low8(flags),
 &local_result]
```

The worker initializes `local_result` to zero. Its explicit non-zero result stores are `0x01000002` and `0x01000001`. Its final output-writing path stores the result of the same secondary helper correlated during A2D0 analysis; that helper has a single final `D0 = 0` exit in AS3000 2005, NEO 2005 and NEO 2013.

Therefore the exact contractual `D0.L` domain is:

```text
0x00000000
0x01000001
0x01000002
0x02000005
0x02000006
```

The worker itself returns `1`, but A2D4 does **not** expose that value; it reloads `local_result` after the worker call.

## Callers and xrefs

Direct ROM-wide searches for absolute `JSR`, absolute `JMP`, and direct `BSR.W` callers of A2D4 are negative in all three canonical ROMs. SmartApplet searches mostly find the standard contiguous syscall stub table. The extra `A2 D4` occurrence inspected in NEO Wireless File Transfer lies inside the immediate operand of `MOVEA.L #0x0000A2D4,A0`, not an executed A-line opcode. Additional thesaurus matches are data, not confirmed 68000 callers.

No independent A2D4 prototype or vendor symbol was recovered from BetaWise or `neo-re`.

## Generation comparison

The external A2D4 handler is mechanically identical modulo relocations in all three ROMs. The internal worker in NEO 2013 adds acceptance of byte value `0x8F` in one character/classification path. This is preserved as a real internal generation difference; it does not change the four-argument ABI or the return domain above.

## Adversarial review

Several tempting interpretations were rejected:

- `source` is not modeled as an integer merely because it occupies a longword slot: the worker loads it into an address register and dereferences it.
- The return is not Boolean; four distinct non-zero statuses are mechanically reachable.
- The worker's own `D0=1` is not the A2D4 return; A2D4 returns the output longword.
- No fifth external argument is claimed; the handler never reads a slot after the low byte of the fourth argument.
- Status and flag names remain neutral rather than guessed from neighboring routines.

## Regression status

Emulator-first regression is **specified, not executed**. Tests should cover each of the five return values, preserve the low byte of `flags` while varying its upper 24 bits, exercise valid/invalid guard and comparison paths, and compare AS3000 2005, NEO 2005 and NEO 2013.

## 2026-09-04 source-first re-audit

The closure was revalidated directly from all three canonical ROMs after correcting the neighboring A2C8 xref record. Canonical ROM identities and exact handler fingerprints reproduced 3/3: AS3000 `866d912bf7d9ee9d101c2ca176ab5df1c5d88b7a7d0f6854bba352849db4c682`, NEO 2005 `a00a72e5e4073a634eadd810ddc710098f27df435c900a1ebe858b7a4401b332`, and NEO 2013 `ddc9127b5268cb28229e8dfa8afa71a924a7a1aa24c9ba01cfc69f11c2d00a41`.

A fresh aligned whole-ROM control-transfer scan reproduced the negative direct-caller result in every generation: zero absolute `JSR`, zero absolute `JMP`, and zero direct `BSR.W` callers to A2D4. The 0x5E-byte lengths and terminal `RTS` at `+0x5C` also reproduced exactly. Static structural regression: **12/12 PASS** (3 canonical ROM identities, 3 exact handler fingerprints, 3 terminal size/epilogue checks, 3 negative direct-xref sets).

No source or manual evidence contradicted the four-slot neutral ABI or the five-value `D0.L` domain. Dynamic/emulator regression remains **specified / not executed**.

Private evidence includes per-ROM disassembly, exact bytes, helper correlation and workpapers. ROM/disassembly material is intentionally not published in this repository.
