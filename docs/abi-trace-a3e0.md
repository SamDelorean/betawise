# ABI trace — A3E0 / index 248

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in contractual return intent.
- AS3000 (2005): non-callable/non-pointer table value.
- NEO (2005): non-callable/non-pointer table value.
- NEO (2013): callable wrapper at runtime `0x00433DE0`; length `0x2A`; SHA-256 `3cef0d7b09e46780fff32fc628d204ef79e836d3015560302ebf6e17658bea45`.
- External physical ABI: three 32-bit slots with effective widths full32, low8, full32.
- Wrapper adaptation to A3DC: forwards external slots 1–3 and supplies `0,0,0,0x37` for the remaining A3DC slots.
- Direct firmware xrefs to A3E0: none found.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- D0 is propagated unchanged from A3DC; no independent consumer fixes return width or signedness.
- Independent symbol/prototype: not recovered.
- Static regression: PASS.
- Dynamic/emulator-first regression: not executed.
- Public callable header: intentionally not published.

Private ROM extraction, stack normalization and disassembly remain outside the repository.
