# SYS_A410 / index 260 — blocked contractual return

Forward-sweep disposition for the OS3K A-line ABI.

## Disposition

`MECÁNICA_RECONSTRUIDA A / BLOQUEADO` exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`.

- AS3000 (2005): table value `0x5F46004E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2005): table value `0x8452005E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2013): callable at runtime `0x00434230` (file offset `0x024230`).
- Handler length: `0x2C` bytes; SHA-256 `f88065fbf44e56a0b1f154d4706ba5b746066aac616080f6c4a27dc393677ba0`.
- Physical ABI: one 32-bit stack slot; only its low 8 bits have demonstrated effect.
- The handler uses private state rooted at the firmware-owned longword stored at `0x0000F416`. A private byte at offset `+0x111` selects a record with stride `0x1A`; the external low byte is ORed into byte offset `+0x10` of that selected record.
- D0 is not normalized: the handler overwrites only D0.B with the external low byte and leaves the upper 24 bits inherited from entry state. No subsequent instruction establishes a return value.
- Direct NEO 2013 firmware xrefs to A410: zero JSR, zero JMP, zero BSR.
- Official SmartApplet exposure is structurally absent from the validated late-table corpus.
- No independent symbol or prototype was recovered.
- `void SYS_A410(uint8_t value)` is a strong inference from setter-like behavior, but without a positive caller/prototype contract it is not promoted to confirmed ABI. A scalar return is likewise unjustified because D0 is residual and non-normalized.
- Static regression: `14/14 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.

No callable header is published for A410. Semantic names for the argument, private records and state remain unknown unless independently demonstrated.

Private ROM bytes, disassembly and regression workpapers remain outside the repository.
