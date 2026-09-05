# ABI trace — A3DC / index 247

Public traceability summary for the source-first OS3K ABI reaudit.

- Disposition: `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO`.
- Source-first anchors: current BetaWise `os3k/syscall.c` and current `ioma8/neo-re` both map index 247 to the neutral placeholder `SYS_A3DC`. This recovers the slot identity only; neither source supplies a semantic vendor name or callable prototype.
- Correction: an earlier workpaper recorded the `neo-re` search as negative. A fresh correlation pass found the explicit index-247 placeholder, so that negative-search statement is superseded.
- AS3000 (2005): non-callable/non-pointer table value.
- NEO (2005): non-callable/non-pointer table value.
- NEO (2013): callable handler at runtime `0x00433C18`; length `0x196`; SHA-256 `0859a060b3ed12b97329ba6f75ecaf4596e11443479dab8817ae670ea6d9d200`.
- Fresh primary replay on the three canonical ROMs reproduced all three canonical ROM hashes, the three A3DC table values, the NEO 2013 handler boundary/hash, its 140-instruction CFG, the independently delimited following `0x32`-byte routine, and the direct xref sets.
- CFG: 140 instructions, 12 direct branches, one RTS; the following `0x32`-byte routine before A3E0 is independently delimited and excluded.
- Physical ABI: seven 32-bit slots with effective widths full32, low8, full32, full32, full32, full32, low16; pointer/nullable behavior is demonstrated for the full-width address-like slots but semantic names remain unknown.
- Direct firmware xrefs: 4 JSR + 4 BSR.W, 0 JMP.
- A3E0/A3E4/A3E8 wrappers propagate A3DC D0 without local redefinition.
- A3DC terminal D0 is propagated from a private helper chain and is not locally normalized; observed consumers do not prove contractual width, signedness, or semantic meaning. The unknown return contract is therefore an evidence limit, not a human blocking condition.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent semantic symbol/prototype: not recovered.
- Static regression: existing full suite `OVERALL PASS`, 38/38 assertions; fresh directed primary replay also passed.
- Dynamic/emulator-first regression: specified, not executed.
- Public callable header: intentionally not published.

Private ROM extraction, disassembly, caller traces, helper-chain analysis and regression workpapers remain outside the repository.
