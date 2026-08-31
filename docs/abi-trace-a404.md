# ABI trace — A404 / index 257

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `MECÁNICA_CERRADA A / PUBLICADO`.
- Neutral contract: `uint8_t SYS_A404(void)`.
- AS3000 (2005): non-callable/non-pointer table value `0x5A06004E`.
- NEO (2005): non-callable/non-pointer table value `0x7F08005E`.
- NEO (2013): callable handler at runtime `0x004341FE`, file offset `0x0241FE`.
- Handler: `0x0C` bytes, three instructions, SHA-256 `a909402de74a82cc4556c04319727643ab86ea320b58244bf68762b4409d0b40`.
- Functional arguments: none.
- Handler result: private-state byte at offset `+0x111` is loaded into `D0.B`; upper D0 bits remain non-contractual.
- Direct firmware xrefs: one JSR at `0x0043E382`, zero direct JMP/BSR.
- Positive return-width evidence: caller immediately copies `D0.B` and compares the copied byte.
- A408 is the contiguous mechanical setter counterpart for the same state byte; no semantic field name is assigned.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent symbol/prototype: not recovered.
- Static regression: `OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.

Private ROM extraction, disassembly, xref traces and regression workpapers remain outside the repository.
