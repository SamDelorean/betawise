# ABI trace — A3DC / index 247

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in contractual return intent.
- AS3000 (2005): non-callable/non-pointer table value.
- NEO (2005): non-callable/non-pointer table value.
- NEO (2013): callable handler at runtime `0x00433C18`; length `0x196`; SHA-256 `0859a060b3ed12b97329ba6f75ecaf4596e11443479dab8817ae670ea6d9d200`.
- CFG: 140 instructions, 12 direct branches, one RTS; the following `0x32`-byte routine before A3E0 is independently delimited and excluded.
- Physical ABI: seven 32-bit slots with effective widths full32, low8, full32, full32, full32, full32, low16; pointer/nullable behavior is demonstrated for the full-width address-like slots but semantic names remain unknown.
- Direct firmware xrefs: 4 JSR + 4 BSR.W, 0 JMP.
- A3E0/A3E4/A3E8 wrappers propagate A3DC D0 without local redefinition.
- A3DC terminal D0 is propagated from a private helper chain and is not locally normalized; observed consumers do not prove width/signedness.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent symbol/prototype: not recovered.
- Static regression: `OVERALL PASS`, 38/38 assertions.
- Dynamic/emulator-first regression: not executed.
- Public callable header: intentionally not published.

Private ROM extraction, disassembly, caller traces, helper-chain analysis and regression workpapers remain outside the repository.
