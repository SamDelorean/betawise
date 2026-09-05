# ABI trace — A40C / index 259

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `CERRADO A / SOURCE_FIRST / PUBLICADO`, with contractual return intent explicitly unknown.
- AS3000 (2005): non-callable/non-pointer table value `0x5EF2004E`.
- NEO (2005): non-callable/non-pointer table value `0x83FE005E`.
- NEO (2013): callable handler at runtime `0x00434218`, file offset `0x024218`.
- Handler: `0x18` bytes, SHA-256 `7adc103ddd74746e5968cd2c0529be03a32db9a28cc8a899c43dc7549528c9d2`.
- Physical ABI: one 32-bit slot with effective low-8-bit consumption; the byte is zero-extended before the private helper call.
- Private dependency: helper `0x0044259A` receives a firmware-owned longword loaded from `0x0000F416` and the zero-extended byte; A40C propagates helper D0 without local normalization.
- Direct firmware xrefs: one JSR at `0x0043E34C`, zero direct JMP/BSR.
- Demonstrated caller value: `1`; the caller does not consume A40C D0 before it is later clobbered.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent symbol/prototype: BetaWise and ioma8/neo-re retain only neutral `SYS_A40C` at index 259; no vendor semantic name recovered.
- Return intent remains unknown: `void` is strongly suggested by observed use but not independently confirmed; a concrete scalar return is likewise unjustified.
- Static regression: historical `20/20 PASS`; fresh directed SOURCE_FIRST regression `22/22 PASS` on 2026-09-05.
- Dynamic/emulator-first regression: specified, not executed.
- Public callable header: intentionally not published.

## SOURCE_FIRST revalidation

Fresh extraction from all three canonical ROMs reproduced the generational split, exact NEO 2013 handler and private-helper dependency. The sole external slot is physically 32 bits, but only its low byte participates: A40C zero-extends it, pushes that value with the firmware-owned longword at `0x0000F416`, calls private helper `0x0044259A`, cleans the two helper arguments and returns.

The private helper's first body again reaches RTS at `+0x68` (`0x6A` bytes). The only direct A40C caller was independently relocated and passes value `1`; it does not consume D0 before later code clobbers it. Accordingly `RETORNO_CONTRACTUAL=DESCONOCIDO` is retained as an evidence limit rather than an open-work blocker. No semantic name is assigned to the private longword, helper, state or value.

Private ROM extraction, disassembly, caller traces, helper analysis and regression workpapers remain outside the repository.
