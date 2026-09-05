# ABI trace — A414 / index 261

Source-first traceability summary for the OS3K ABI audit.

- Disposition: `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO`.
- Historical anchors: the historical BetaWise `syscall.c` and `ioma8/neo-re` both map index 261 to the neutral placeholder `SYS_A414`; neither source recovers a vendor semantic name or independent prototype.
- AS3000 (2005): non-callable/non-pointer table value `0x5F70004E`.
- NEO (2005): non-callable/non-pointer table value `0x847C005E`.
- NEO (2013): callable handler at runtime `0x0043425C`, file offset `0x02425C`.
- Handler boundary: `0x3C` bytes / 23 instructions. The following independent `0x3C`-byte routine before A418 is excluded from A414.
- Physical ABI: two 32-bit slots; `arg1` is consumed full-width and only `arg2.low8` is semantically consumed.
- Confirmed dataflow: equality of `arg2.low8` against a private state byte is normalized to a longword 0/1 flag; the derived record address is `arg1 + 4 + sign_extend_8(arg2.low8) * 0x12C`.
- Private dependency: A414 calls a private helper with `[derived_record, equality_flag]` and propagates helper D0 without local normalization.
- Direct firmware xrefs: zero JSR/JMP/BSR to A414 in the canonical NEO 2013 ROM.
- Official SmartApplet exposure: structurally absent from the previously validated late-table corpus.
- Return contract: `DESCONOCIDO`. `void` is not promoted from setter-like appearance, and no concrete scalar width/signedness is justified without a consumer or independent prototype.
- Static regression: historical `17/17 PASS`; fresh source-first revalidation `29/29 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: `ESPECIFICADA / NO EJECUTADA`.
- Public callable header: intentionally not published.

Private ROM extraction, raw handler bytes, extensive disassembly and regression workpapers remain outside the repository.
