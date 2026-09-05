# ABI trace — A3E0 / index 248

Public traceability summary for the source-first OS3K ABI reaudit.

- Disposition: `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO`.
- Source-first anchors: current BetaWise `os3k/syscall.c` and current `ioma8/neo-re` both map index 248 to the neutral placeholder `SYS_A3E0`. This recovers slot identity only; neither source supplies a semantic vendor name or callable prototype.
- AS3000 (2005): non-callable/non-pointer table value.
- NEO (2005): non-callable/non-pointer table value.
- NEO (2013): callable wrapper at runtime `0x00433DE0`; length `0x2A`; SHA-256 `3cef0d7b09e46780fff32fc628d204ef79e836d3015560302ebf6e17658bea45`.
- Fresh primary replay reproduced all three canonical ROM hashes and A3E0 table values, the NEO 2013 wrapper bytes/boundary/hash, 15-instruction decode, unique terminal RTS, its sole BSR.W target to A3DC, seven-slot cleanup, wrapper dataflow, and the negative direct-xref search.
- External physical ABI: three 32-bit slots with effective widths full32, low8, full32. Semantic argument names remain unknown.
- Wrapper adaptation to A3DC: forwards external slots 1–3 and supplies `0,0,0,0x37` for the remaining A3DC slots. The second external slot is sign-extended from its low byte before forwarding, but A3DC consumes only its low byte; signedness is therefore not promoted to a contractual claim.
- Direct firmware xrefs to A3E0: none found (0 JSR, 0 JMP, 0 direct BSR); this is a reproducible negative result, not a claim that indirect invocation is impossible.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- D0 is propagated unchanged from A3DC; no independent consumer fixes contractual return width, signedness, or semantic meaning. This unknown is an evidence limit, not a human blocking condition.
- Independent semantic symbol/prototype: not recovered.
- Static regression: prior suite PASS; fresh directed source-first replay PASS (16/16 assertions in this reaudit pass).
- Dynamic/emulator-first regression: specified, not executed.
- Public callable header: intentionally not published.

Private ROM extraction, exact firmware bytes, stack-normalization workpapers and disassembly remain outside the repository.
