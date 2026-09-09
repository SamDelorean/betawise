# ABI trace — A3DC / index 247

Public traceability summary for the source-first OS3K ABI reaudit.

- Disposition: `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO`.
- Source-first anchors: current BetaWise `os3k/syscall.c` and `ioma8/neo-re` both map index 247 to the neutral placeholder `SYS_A3DC`. This recovers the slot identity only; neither source supplies a semantic vendor name or callable prototype.
- Correction: an earlier workpaper recorded the `neo-re` search as negative. A fresh correlation pass found the explicit index-247 placeholder, so that negative-search statement is superseded.
- AS3000 (2005): non-callable/non-pointer table value.
- NEO (2005): non-callable/non-pointer table value.
- NEO (2013): callable handler at runtime `0x00433C18`; length `0x196`; SHA-256 `0859a060b3ed12b97329ba6f75ecaf4596e11443479dab8817ae670ea6d9d200`.
- Fresh primary replay on the three canonical ROMs reproduced all three canonical ROM hashes, the three A3DC table values, the NEO 2013 handler boundary/hash, its 140-instruction CFG, the independently delimited following `0x32`-byte routine, and the direct xref sets.
- CFG: 140 instructions, 12 direct branches, one RTS; the following `0x32`-byte routine before A3E0 is independently delimited and excluded.
- Physical ABI: seven 32-bit stack slots. Effective properties independently supported by handler and callers are: slot 1 `pointer/full32`; slot 2 low8; slot 3 pointer/full32; slot 4 full32/pointer-like nullable; slot 5 full32/pointer-like nullable; slot 6 pointer/full32 nullable; slot 7 low16. Semantic names remain unknown.
- Critical retraction: a prior revision used Manager selector `0x18` as evidence that slot 1 accepted scalar values `0/3/4/5`. That evidence is invalid. The Manager literal `0x00400420` is a physical Small-ROM runtime entry, not syscall opcode `A420`; therefore Manager `0x18` does not call A420 and supplies no evidence about A3DC slot 1.
- The standalone A3DC body uses slot 1 as an address/base object, and the independently demonstrated direct callers supply pointer/object values. The earlier `not pointer-required` correction is withdrawn.
- Direct firmware xrefs remain the independently verified set: 4 JSR + 4 BSR.W, 0 JMP. No additional A420-mediated Manager path is claimed.
- A3E0/A3E4/A3E8 wrappers propagate A3DC D0 without local redefinition.
- A3DC terminal D0 is propagated from a private helper chain and is not locally normalized; observed consumers do not prove contractual width, signedness, or semantic meaning. The unknown return contract is therefore an evidence limit, not a human blocking condition.
- The earlier search for an A3DC helper responsible for entering Small ROM is terminated: it was derived from the false `Manager18 -> A420 -> A3DC` chain, not from an independent A3DC caller.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- Independent semantic symbol/prototype: not recovered.
- Static regression: the standalone A3DC suite remains `38/38 PASS / OVERALL PASS`. A later 33/33 Manager18/A420/A3DC regression is retained only as historical evidence of the superseded category error and must not be used to type A3DC slot 1.
- Dynamic/emulator-first regression: specified, not executed.
- Public callable header: intentionally not published.

Private ROM extraction, disassembly, caller traces, helper-chain analysis and regression workpapers remain outside the repository.
