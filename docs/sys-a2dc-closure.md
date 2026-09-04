# SYS_A2DC mechanical closure

Status: mechanically closed, confidence A. Dynamic/emulator regression is specified but not yet executed.

## Evidence base

A2DC was reconstructed independently from the canonical AlphaSmart 3000 2005, NEO 2005 and NEO 2013 OS3K ROMs. The private workpapers retain ROM hashes, runtime/file mappings and separate 68k disassemblies; firmware and extensive disassembly are not published here.

The AlphaSmart 3000 implementation is an 8-byte stub that returns `0x02000003` and does not inspect stack arguments. NEO 2005 and NEO 2013 use 0x90-byte handlers that are instruction-equivalent after three absolute-call relocations.

## Neutral ABI

```c
uint32_t SYS_A2DC(void *state,
                  const void *source_4e,
                  const void *source_52,
                  const void *source_56,
                  const void *source_5a,
                  uint8_t post_flag);
```

For NEO, six 32-bit stack slots are consumed. `state` is mandatory. The four `source_*` arguments are nullable pointers; a non-null argument replaces the corresponding pointer field in the state object. Only the low byte of `post_flag` is consumed. AlphaSmart 3000 ignores all six parameters because its handler is a platform stub.

## Return and side effects

AlphaSmart 3000 returns exactly `0x02000003` with no handler-side state mutation. NEO returns `0x02000005` when its state guard is active; otherwise the normal path returns 0. The cross-platform observed D0.L domain is therefore `{0, 0x02000003, 0x02000005}`.

On NEO, the operation can replace four pointer fields, dereference the pointer held in the first of those fields, derive and update a word in the state object, and invoke a post-success helper when `post_flag` is non-zero. The post helper's own return value is ignored.

## Callers and adversarial checks

Direct ROM-wide absolute `JSR`, absolute `JMP`, and direct `BSR.W` searches are negative in all three ROMs. SmartApplet matches examined outside normal syscall stub tables did not establish an executable direct caller. No independent BetaWise/neo-re prototype was found.

The closure deliberately preserves the real AS3000/NEO divergence. It does not infer six physically consumed arguments for the AS3000 stub, does not reinterpret the four pointer slots as scalar values, does not expose the ignored post-helper result, and does not assign vendor names to the operation or status values.

## Regression status

Emulator-first regression is specified, not executed: AS3000 must return `0x02000003` regardless of argument values; NEO tests cover the state guard, normal zero return, null/non-null replacement for each pointer slot, derived-state update, `post_flag` behavior, and invariance to upper bits of the flag slot.

## 2026-09-04 source-first re-audit

Canonical ROM identities and exact A2DC handler fingerprints reproduced 3/3. The AS3000 body is exactly eight bytes with SHA-256 `1d505a3190d211bccea40a1b5cf3527b3871eb19b66c4ae800f506bade5e024d` and literal code equivalent to loading `0x02000003` into `D0.L` followed by `RTS`. NEO 2005 reproduces SHA-256 `df6019e1520c059712aa260c3bad78f12181ff7f7cc1cd84d55bb5d1d6e48a1a`; NEO 2013 reproduces `591183aad2e6c5f0cb80bfdb99af26d63498d05d544d3ce31e219dfaca4d874a`, both at 0x90 bytes with terminal `RTS`.

A fresh aligned whole-ROM control-transfer scan reproduced zero absolute `JSR`, zero absolute `JMP`, and zero direct `BSR.W` callers to A2DC in every generation. Static structural regression: **12/12 PASS** (3 canonical ROM identities, 3 exact handler fingerprints/platform forms, 3 terminal size/epilogue checks, 3 negative direct-xref sets).

No source or manual evidence contradicted the platform split, common SDK signature, or cross-platform return domain `{0, 0x02000003, 0x02000005}`. Dynamic/emulator regression remains **specified / not executed**.
