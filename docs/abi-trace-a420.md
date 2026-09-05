# ABI trace — A420 / index 264

Source-first traceability summary for the OS3K ABI audit.

- Disposition: `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / PUBLICADO / RETORNO_CONTRACTUAL=DESCONOCIDO`.
- Historical anchors: historical BetaWise `syscall.c` and `ioma8/neo-re` both map index 264 to the neutral placeholder `SYS_A420`; neither source recovers a vendor semantic name or independent prototype.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: callable handler at runtime `0x00433DAE`, file offset `0x023DAE`.
- Handler: `0x32` bytes / 17 instructions / one terminal RTS; SHA-256 `b6eb5df5a0e11fad59e68e74dacac6d2bd1655faaf720dc5df82b67e59626392`.
- Physical ABI: three 32-bit external slots. Effective consumption is `arg1` full32, `arg2` low8, and `arg3` as a readable pointer to at least four consecutive longwords.
- Wrapper mapping: A420 synthesizes A3DC's seven internal slots as `[arg1, sign_extend_8(low8(arg2)), arg3[0], arg3[1], arg3[2], arg3[3], 0]`, calls A3DC at `0x00433C18`, and cleans exactly `0x1C` bytes.
- Return mechanics: A420 does not redefine D0 after A3DC and therefore propagates A3DC's raw D0 to RTS. A3DC itself has no independently demonstrated contractual return width/signedness, so A420 cannot safely promote `void` or any concrete scalar result.
- Direct firmware xrefs: zero JSR/JMP/BSR.W/BSR.B to A420 in the canonical NEO 2013 ROM. Official SmartApplet exposure is structurally absent (`0/41`) for this late slot.
- Adversarial checks: A420 is not the tail of A3DC; the two routines have independent boundaries and RTS instructions. `void` remains an intent inference only, while a scalar contract is likewise unsupported in the absence of a consumer or independent prototype.
- Static regression: historical `23/23 PASS` plus an independent `36/36 PASS`; fresh source-first primary revalidation `32/32 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: `ESPECIFICADA / NO EJECUTADA`.
- Public callable header: intentionally not published.

Private ROM extraction, raw handler bytes, extensive disassembly and regression workpapers remain outside the repository.
