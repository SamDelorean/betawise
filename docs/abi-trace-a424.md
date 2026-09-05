# ABI trace — A424 / index 265

Source-first traceability summary for the OS3K ABI audit.

- Disposition: `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / PUBLICADO / RETORNO_CONTRACTUAL=DESCONOCIDO`.
- Historical anchors: historical BetaWise `syscall.c` and `ioma8/neo-re` both map index 265 to the neutral placeholder `SYS_A424`; neither source recovers a vendor semantic name or independent prototype.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: callable handler at runtime `0x00434348`, file offset `0x024348`.
- Handler: `0x5A` bytes / 29 instructions / one terminal RTS; SHA-256 `cfd73e3dd4facd3f85122c02e7149d739a9370891e219c25a308deae80232bda`. The larger vector interval before A428 contains independent later routines and is not used as the handler boundary.
- Physical ABI: two 32-bit external slots; effective consumption is `arg1` full32 and `arg2` low8.
- Private mechanics: the routine operates through firmware globals at `0x0000F412/0x0000F416/0x0000F41A`. Its paths compare/update the full32 state, derive a stride-`0x12C` record address using the effective byte, and call private helpers `0x004346C8` and `0x0044254E`. These globals/helpers remain semantically unnamed.
- Return mechanics: D0 is path-dependent and non-uniform. One path leaves entry/residual D0, another leaves arithmetic D0, and the non-null update path leaves D0 from the final helper.
- Direct firmware xrefs: zero JSR/JMP/BSR.B and exactly one BSR.W from A3DC at runtime `0x00433C40`. That caller explicitly constructs the two arguments and immediately executes `MOVEQ #0,D0` after A424, so it does not consume the return.
- Adversarial checks: `void` is a very strong intent inference from non-use/non-uniform D0 but is not a positive C ABI contract; a scalar return is even less justified. `RETORNO_CONTRACTUAL=DESCONOCIDO` is therefore an evidence limit, not a human-work blocker.
- Official SmartApplet exposure: structurally absent (`0/41`) for this late slot.
- Static regression: historical `34/34 PASS`; fresh source-first primary revalidation `36/36 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: `ESPECIFICADA / NO EJECUTADA`.
- Public callable header: intentionally not published.

Private ROM extraction, raw handler bytes, extensive disassembly, caller traces and regression workpapers remain outside the repository.
