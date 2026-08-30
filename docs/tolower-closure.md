# A3A8 / `tolower` closure

A3A8 (index 234) is mechanically reconstructed as:

```c
int tolower(int c);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004DDD28`, file `+0x01DD28`
- NEO 2005: runtime `0x005E0446`, file `+0x020446`
- NEO 2013: runtime `0x00436844`, file `+0x026844`

The complete handler is 0x1A bytes and byte-identical across all three ROMs (SHA-256 `467c88933f60a216b6234efaf42d20120662dd892d48470aacac33e4bb87f837`). It consumes one full 32-bit argument. Values in the inclusive ASCII uppercase interval `0x41..0x5A` are returned plus `0x20`; every other 32-bit value is returned unchanged. The operation has no helper calls, globals, tables or memory writes.

This is an ASCII/C-locale implementation of `tolower`. In particular, `-1` (`0xFFFFFFFF`) is outside the uppercase interval and is preserved unchanged.

## Caller correlation

The official SmartApplet corpus is complete at 41/41. The physical A-line target (`A378 + 0x18`) yields 39 executable A3A8 calls in 13 applets; 17 table-bearing applets are negative and the remaining 11 corpus members are structural negatives without the table.

Callers construct one 32-bit slot. Many sites zero-extend a source byte before the call; paired comparison sites call A3A8 for two characters and compare the full returned longwords. Every positive site either consumes or explicitly preserves `D0.L`, independently confirming an `int`-sized return rather than a byte-only result.

The canonical firmware contains exactly six direct absolute JSRs to A3A8 in each generation, with no direct JMP or BSR entry use. These firmware sites show the same case-normalization pattern on byte-derived characters.

## Adversarial alternatives

- `toupper` is rejected because the only transformation is uppercase to lowercase by adding `0x20`.
- `isupper` or another predicate is rejected because the handler returns the transformed/original character, not a boolean.
- A byte-only conversion contract is rejected by the handler's full-long comparisons and by caller consumption of `D0.L`.
- A locale/table-driven converter is rejected because the handler contains no lookup or global state; its mapping is explicitly ASCII-only.

The historical BetaWise `int tolower(int c)` declaration is secondary corroboration only. The contract follows from ROM mechanics and callers.

## Validation status

Static regression over all three canonical ROMs and the complete official SmartApplet corpus executed with **OVERALL PASS**. It checks canonical ROM hashes, exact handler bytes/hash, direct firmware xrefs (6/6/6 JSR), the exact 39-call/13-applet corpus, full-long return consumption, edge-case mechanics, and neighboring detector controls A3A0=2 and A3A4=11.

Dynamic emulator-first regression is specified but not executed; its minimum matrix includes `@`, `A`, `Z`, `[`, lowercase input, zero, EOF `-1`, and values outside the byte range.

Status: **MECÁNICA_CERRADA A / PUBLICADO**. The existing `os3k.h` prototype already matches the reconstructed contract, so no header modification is required.
