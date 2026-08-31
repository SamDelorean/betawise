# ABI trace — A40C / index 259

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in contractual return intent.
- AS3000 (2005): non-callable/non-pointer table value `0x5EF2004E`.
- NEO (2005): non-callable/non-pointer table value `0x83FE005E`.
- NEO (2013): callable handler at runtime `0x00434218`, file offset `0x024218`.
- Handler: `0x18` bytes, SHA-256 `7adc103ddd74746e5968cd2c0529be03a32db9a28cc8a899c43dc7549528c9d2`.
- Physical ABI: one 32-bit slot with effective low-8-bit consumption; the byte is zero-extended before the private helper call.
- Private dependency: helper `0x0044259A` receives a firmware-owned longword loaded from `0x0000F416` and the zero-extended byte; A40C propagates helper D0 without local normalization.
- Direct firmware xrefs: one JSR at `0x0043E34C`, zero direct JMP/BSR.
- Demonstrated caller value: `1`; the caller does not consume A40C D0 before it is later clobbered.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent symbol/prototype: not recovered.
- Return intent remains unknown: `void` is strongly suggested by observed use but not independently confirmed; a concrete scalar return is likewise unjustified.
- Static regression: `20/20 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.
- Public callable header: intentionally not published.

Private ROM extraction, disassembly, caller traces, helper analysis and regression workpapers remain outside the repository.
