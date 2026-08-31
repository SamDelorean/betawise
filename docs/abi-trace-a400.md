# ABI trace — A400 / index 256

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in contractual return intent.
- AS3000 (2005): non-callable/non-pointer table value.
- NEO (2005): non-callable/non-pointer table value.
- NEO (2013): callable handler at runtime `0x004343A2`, file offset `0x0243A2`.
- Handler: `0x2DA` bytes, 244 instructions, 52 direct branches, one RTS; SHA-256 `407349319b3fbf0c0f5245126a20ed31a45649267c79e6a218655dbfb6a29e7d`.
- Physical ABI: one 32-bit slot with effective low-16-bit consumption.
- Direct firmware xrefs: one JSR, zero JMP/BSR to A400.
- The demonstrated caller passes a zero-extended word and ignores D0 after cleanup.
- The implementation uses private globals and record state including a `0x12C` stride and several private helpers; semantic names are intentionally not assigned.
- D0 is not uniformly normalized across all exits, so a concrete scalar return is not justified. `void` remains a strong but unconfirmed inference.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent symbol/prototype: not recovered.
- Static regression: `OVERALL PASS`, 29/29 assertions.
- Dynamic/emulator-first regression: not executed.
- Public callable header: intentionally not published.

Private ROM extraction, disassembly, caller traces, helper analysis and regression workpapers remain outside the repository.
