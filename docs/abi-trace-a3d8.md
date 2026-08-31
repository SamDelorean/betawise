# ABI trace — A3D8 / index 246

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in contractual return intent.
- AlphaSmart 3000 (2005): non-callable/non-pointer table value.
- NEO (2005): non-callable/non-pointer table value.
- NEO (2013): callable handler, length `0x178`, SHA-256 `15c33d2963c3ada1e1dc15669cf7a4b109c213f95f5b37896b10825f1c94f837`.
- Physical ABI: four 32-bit slots; effective widths are low16, low8, full32, full32/pointer-like.
- Mechanical result: `D0.B` is exactly in `{0, 3, 5, 8}` at reachable exits; upper D0 is not uniformly contractual because the zero path clears only the low byte.
- Direct firmware callers to A3D8: none found.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent symbol/prototype: not recovered.
- Static regression: `OVERALL PASS`, 29/29 assertions.
- Dynamic/emulator-first regression: not executed.
- Public callable header: intentionally not published.

Private evidence, ROM extraction, disassembly, helper/global correlation and regression workpapers remain outside the repository.
