# SYS_A40C / index 259 — blocked contractual return

Forward-sweep disposition for the OS3K A-line ABI.

## Disposition

`MECÁNICA_RECONSTRUIDA A / BLOQUEADO` exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`.

- AS3000 (2005): table value `0x5EF2004E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2005): table value `0x83FE005E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2013): callable at runtime `0x00434218` (file offset `0x024218`).
- Handler length: `0x18` bytes; SHA-256 `7adc103ddd74746e5968cd2c0529be03a32db9a28cc8a899c43dc7549528c9d2`.
- Physical ABI: one 32-bit stack slot; only its low 8 bits are consumed and locally zero-extended before the private helper call.
- The handler passes the firmware-owned longword loaded from absolute state `0x0000F416` together with the zero-extended byte to private helper `0x0044259A`, then propagates that helper's D0 without local normalization.
- Exactly one direct NEO 2013 firmware caller was identified, at runtime `0x0043E34C`; it supplies value `1` and does not consume A40C D0 before later code clobbers it.
- Official SmartApplet exposure is structurally absent from the validated late-table corpus.
- No independent symbol or prototype was recovered.
- `void SYS_A40C(uint8_t value)` is therefore a strong inference from observed use, but it is not promoted to a confirmed contract. No scalar return width/signedness is independently demonstrated either.
- Static regression: `20/20 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.

No callable header is published for A40C. Semantic names for the argument, private state and helper remain unknown unless independently demonstrated.

Private ROM bytes, disassembly, caller traces and detailed regression workpapers remain outside the repository.
