# A3A8 / `tolower` closure

A3A8 (index 234) is reconstructed as:

```c
int tolower(int c);
```

Status: **CLOSED A / SOURCE_FIRST / PUBLISHED**.

## Source-first anchors

The name and prototype were treated as hypotheses before checking firmware:

- historical/preserved `os3k.h` / `os3k.pdf`: `int tolower(int c)`;
- BetaWise `os3k/syscall.c`: index 234 is mapped to `tolower`;
- the DebugTool source uses `tolower(pBuffer[1])` as a character-normalization call while parsing a `0x` prefix;
- current `master` and `sdk/abi-automation` preserve the same A3A8 mapping. No public closure document exists on `master` or `sdk/abi-foundation`, so this branch remains the non-destructive publication surface.

These references recover the expected identity/signature, but they are not primary proof. In particular, the separate A160 service is only annotated historically as another lowercasing/international-character routine and must not be conflated with the C-library-compatible A3A8 service.

## Primary firmware confirmation

A fresh source-first revalidation on 2026-09-05 re-read the private canonical ROMs from Drive and reproduced their full SHA-256 identities before extracting A3A8:

- AS3000 2005: runtime `0x004DDD28`, file `+0x01DD28`
- NEO 2005: runtime `0x005E0446`, file `+0x020446`
- NEO 2013: runtime `0x00436844`, file `+0x026844`

The complete handler is 0x1A bytes and byte-identical across all three ROMs (handler SHA-256 `467c88933f60a216b6234efaf42d20120662dd892d48470aacac33e4bb87f837`). It reads one full 32-bit stack argument, performs signed full-long comparisons against the inclusive ASCII uppercase interval `0x41..0x5A`, returns the input plus `0x20` when inside that interval, and otherwise returns the original longword unchanged. There are no helper calls, globals, lookup tables or memory writes.

Thus the primary mechanics **CONFIRM** an ASCII/C-locale implementation of `tolower`. EOF `-1` (`0xFFFFFFFF`) is preserved because it is outside the uppercase interval.

The same fresh pass also reproduced exactly six direct absolute JSR references to the A3A8 handler in each canonical ROM and zero direct absolute JMP references.

## Caller correlation

The existing reproducible full-corpus regression remains valid: the official SmartApplet corpus is 41/41, with 39 executable A3A8 calls in 13 applets. Seventeen table-bearing applets are negative and the remaining eleven corpus members are structural negatives without the table.

Positive callers construct one 32-bit slot. Many zero-extend a source byte before the call; paired comparison sites normalize two characters and compare the returned longwords. Every positive site either consumes or explicitly preserves `D0.L`, independently supporting an `int`-sized return rather than a byte-only result. DebugTool supplies an additional source-level anchor: it calls `tolower` while accepting either `0x`/`0X`-style numeric input.

## Adversarial refutation

- `toupper` is rejected because the only transformation is uppercase to lowercase by adding `0x20`.
- `isupper` or another predicate is rejected because the handler returns the transformed/original character rather than a boolean.
- A byte-only conversion contract is rejected by the full-long stack load/comparisons, verbatim longword fallback and caller consumption of `D0.L`.
- A locale/table-driven converter is rejected because the handler contains no lookup, helper or mutable/global state.
- Conflation with A160 is rejected: A160 is a distinct syscall with separate historical annotation and semantics; the firmware slot under audit here is A3A8/index234.

No attempted refutation displaced `int tolower(int c)`.

## Validation status

The private full static regression previously executed with **OVERALL PASS**. It checks all three canonical ROM hashes, exact handler identity, firmware xrefs (6/6/6 direct JSR), the exact 39-call/13-applet corpus, full-long return consumption, edge mechanics, and neighboring detector controls A3A0=2 and A3A4=11.

A fresh targeted primary regression on 2026-09-05 was **EXECUTED / PASS** for canonical ROM hashes, handler extraction/hash/identity and direct JSR/JMP xrefs. The already-reproducible complete caller regression was not needlessly repeated.

Dynamic emulator-first regression remains **SPECIFIED / NOT EXECUTED**. Its minimum matrix includes `@`, `A`, `Z`, `[`, lowercase input, zero, EOF `-1`, and values outside the byte range.

The existing `os3k.h` prototype already matches the confirmed ABI, so no header modification is required. Firmware, raw handler bytes, extensive disassembly and private corpus artifacts remain in Drive and are intentionally not published here.
