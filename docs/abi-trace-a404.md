# ABI trace — A404 / index 257

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `CERRADO A / SOURCE_FIRST / PUBLICADO`.
- Neutral contract: `uint8_t SYS_A404(void)`.
- AS3000 (2005): non-callable/non-pointer table value `0x5A06004E`.
- NEO (2005): non-callable/non-pointer table value `0x7F08005E`.
- NEO (2013): callable handler at runtime `0x004341FE`, file offset `0x0241FE`.
- Handler: `0x0C` bytes, three instructions, SHA-256 `a909402de74a82cc4556c04319727643ab86ea320b58244bf68762b4409d0b40`.
- Functional arguments: none.
- Handler result: private-state byte at offset `+0x111` is loaded into `D0.B`; upper D0 bits remain non-contractual.
- Direct firmware xrefs: one JSR at `0x0043E382`, zero direct JMP/BSR.
- Positive return-width evidence: caller immediately copies `D0.B` to `D7` and compares `D7` with `0x0C`.
- A408 is the contiguous mechanical setter counterpart for the same state byte; no semantic field name is assigned.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent symbol/prototype: no vendor semantic name recovered; historical BetaWise and ioma8/neo-re retain only `SYS_A404` at index 257.
- Static regression: historical `18/18 PASS`; fresh directed SOURCE_FIRST regression `21/21 PASS` on 2026-09-05.
- Dynamic/emulator-first regression: specified, not executed.

## SOURCE_FIRST revalidation

The current pass rehashed all three canonical ROMs and re-read the index-257 table entries directly. AS3000 and NEO 2005 remain non-callable at this slot; only NEO 2013 exposes a handler. Fresh extraction reproduces the exact 12-byte handler and hash above. The routine dereferences the private pointer stored at `0x0000F416`, reads exactly one byte at offset `+0x111`, and returns immediately.

The only direct NEO 2013 caller was independently relocated again. Its immediate post-call sequence consumes only `D0.B`, which closes the externally observable return width as a raw unsigned byte. `void`, word/long contractual returns, and a normalized boolean interpretation are therefore rejected. The meaning or vendor name of the private field remains unknown and is deliberately not invented.

Private ROM extraction, disassembly, xref traces and regression workpapers remain outside the repository.
