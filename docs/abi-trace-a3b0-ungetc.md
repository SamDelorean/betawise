# ABI trace — A3B0 / `ungetc`

- Index: 236 / A3B0
- Status: **CLOSED A / SOURCE_FIRST / PUBLISHED**
- Contract: `int ungetc(int c, FILE *stream)`
- Source anchors: BetaWise `syscall.c` maps index 236 to `ungetc`; `os3k.h` declares the two-argument C contract. These are anchors only; firmware remains authoritative.
- Handler: 0x38 bytes in AS3000 2005 / NEO 2005 / NEO 2013; control flow is identical after relocation
- Canonical runtime/file entries: AS `0x004E0932/+0x020932`; NEO05 `0x005E328E/+0x02328E`; NEO13 `0x0043968C/+0x02968C`
- Raw handler SHA-256: AS `6a5f13d4a2d8532652f112159b0be5e2b38d1ca61d017e844f8483fd69138f40`; NEO05 `bec5321ea7419dc266571b89775b8b272bd29891141b8617563aa4f26e607191`; NEO13 `b425af6198ef615788ad53331bcd0f4512121dbf349855c0469d44dc64a06631`
- Relocation-normalized SHA-256: `200e119286c806c79998d99c9f2b1624b9ecdae6510ad8adc78d71f80f7a068a`
- ABI: `c` and `stream` are full 32-bit stack slots; result is returned in full `D0.L`
- Confirmed failure cases: `c == -1`, stream is not the firmware stdin descriptor, or shared input-buffer read index is zero -> return `-1` without pushback
- Confirmed success mechanics: decrement the shared A348/fgetc read index, write `c & 0xff` to the shared input buffer at the new index, and return the original full longword `c`
- Firmware-specific quirk: values outside the byte range are truncated only for storage; the original full longword is returned. Do not normalize this to stricter ISO-C behavior not observed in firmware.
- Globals relocate by generation: stdin descriptor AS `0x004EAC04`, NEO05 `0x005F152E`, NEO13 `0x0044F146`; input index/base AS `0x44CB/0x447B`, NEO05 `0x58FD/0x58AD`, NEO13 `0x118FB/0x118AB`
- Callers: official SmartApplet corpus 41/41 contains no direct A-line call; firmware has no direct JSR/JMP/BSR to the handler. The positive use is indirect: exactly two executable `PEA` references per ROM, installed by A354/fscanf and A374/scanf alongside A348/fgetc as callbacks into the scan engine.
- Adversarial alternatives rejected: `fgetc`/`getc`, generic buffer setter, void-return helper, generic multi-stream pushback, and proof by BetaWise name alone
- Static regression: **EXECUTED / OVERALL PASS**
- Dynamic emulator-first regression: **SPECIFIED / NOT EXECUTED**
- Existing `os3k.h` prototype already matches; no header change

Private ROM bytes, detailed disassemblies, caller manifests and regression artifacts remain in Drive and are not published here. See `docs/ungetc-closure.md` for the earlier public functional closure.
