# ABI trace — A410 / index 260

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `CERRADO A / SOURCE_FIRST / PUBLICADO`, with contractual return intent explicitly unknown.
- AS3000 (2005): non-callable/non-pointer table value `0x5F46004E`.
- NEO (2005): non-callable/non-pointer table value `0x8452005E`.
- NEO (2013): callable handler at runtime `0x00434230`, file offset `0x024230`.
- Handler: `0x2C` bytes, SHA-256 `f88065fbf44e56a0b1f154d4706ba5b746066aac616080f6c4a27dc393677ba0`.
- Physical ABI: one 32-bit slot with effective low-8-bit consumption.
- Private-state effect: byte `+0x111` selects a stride-`0x1A` record rooted through firmware-owned state at `0x0000F416`; the external byte is ORed into selected-record offset `+0x10`.
- D0 is not normalized: only D0.B is overwritten with the external byte, while the upper 24 bits remain inherited from entry state.
- Direct firmware xrefs: zero JSR/JMP/BSR to A410.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent symbol/prototype: BetaWise and ioma8/neo-re retain only neutral `SYS_A410` at index 260; no vendor semantic name recovered.
- Return intent remains unknown: `void` is strongly suggested by setter-like behavior but not independently confirmed; a scalar return is not justified from residual D0.
- Static regression: historical `14/14 PASS`; fresh directed SOURCE_FIRST regression `22/22 PASS` on 2026-09-05.
- Dynamic/emulator-first regression: specified, not executed.
- Public callable header: intentionally not published.

## SOURCE_FIRST revalidation

The current pass rehashed all three canonical ROMs and reproduced the generational split and the exact NEO 2013 body. The routine consumes only the low byte of its sole physical slot. Firmware-owned state at `0x0000F416` and byte `+0x111` select a record at stride `0x1A`; the argument byte is ORed into offset `+0x10` of that selected record.

No direct firmware caller and no late SmartApplet caller were found. Because the byte load into D0 leaves its upper 24 bits inherited and no consumer fixes a return interpretation, `RETORNO_CONTRACTUAL=DESCONOCIDO` remains the correct evidence boundary. Setter-like `void` is a very strong inference only, not a published callable contract.

Private ROM extraction, disassembly and regression workpapers remain outside the repository.
