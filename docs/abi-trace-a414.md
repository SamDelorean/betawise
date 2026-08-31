# ABI trace — A414 / index 261

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in contractual return intent.
- AS3000 (2005): non-callable/non-pointer table value `0x5F70004E`.
- NEO (2005): non-callable/non-pointer table value `0x847C005E`.
- NEO (2013): callable handler at runtime `0x0043425C`, file offset `0x02425C`.
- Handler: `0x3C` bytes / 23 instructions, SHA-256 `cdeb3c3d86e7e425dc8bc1c327c1d75e23c24b1ae1626b5099cf1217bb71e473`; a second independent `0x3C`-byte routine before A418 is excluded.
- Physical ABI: two 32-bit slots; `arg1` full-width, `arg2` effective low-8-bit consumption.
- Dataflow: equality of `arg2.low8` against private byte state `0x0000F41A` becomes a longword 0/1 flag; record pointer = `arg1 + 4 + sign_extend_8(arg2.low8) * 0x12C`.
- Private dependency: helper `0x004349BE`; A414 propagates helper D0 without local normalization.
- Direct firmware xrefs: zero JSR/JMP/BSR to A414.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent symbol/prototype: not recovered.
- Return intent remains unknown; neither `void` nor a concrete scalar return is justified.
- Static regression: `17/17 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.
- Public callable header: intentionally not published.

Private ROM extraction, disassembly and regression workpapers remain outside the repository.
