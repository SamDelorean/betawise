# ABI trace — A3F4 / index253

Disposition: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO only in `RETORNO_CONTRACTUAL=DESCONOCIDO`**.

## Primary mapping

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5EC6004E` | non-pointer / no callable contract |
| NEO 2005 | `0x83D2005E` | non-pointer / no callable contract |
| NEO 2013 | `0x00433FE2` | callable handler, file offset `0x023FE2` |

A3F8 begins at `0x00434048`, fixing A3F4 at `0x66` bytes. The handler decodes to 35 instructions with one terminal RTS and SHA-256 `0f3e2c9544939309eca0dcb9bb981561956df36a1b1e47aa2e12802c1b8e17c3`.

## ABI and stack rebasing

The physical ABI contains five longword slots:

`[arg1_pointer, low8(arg2), low8(arg3), low8(arg4), low8(arg5)]`

Four instructions use the same apparent displacement `+0x1B(SP)`, but execute after different numbers of pushes. Rebased to the entry stack frame, their handler offsets `+0x1E`, `+0x2E`, `+0x38`, `+0x46` select arg5, arg4, arg3 and arg2 respectively.

The routine constructs a primary record from `arg1+4+signext8(arg2)*0x12C`, a secondary subrecord from `+0x34+zeroext8(arg5)*0x1A`, and calls private helper `0x004337B0` with:

`[primary+0x11E, signext8(arg3), signext8(arg4), secondary]`

These extensions are implementation mechanics, not public type claims.

## Xrefs and return

Aligned NEO 2013 sweep found zero direct JSR, JMP, BSR.W or BSR.B references to A3F4. The validated official SmartApplet corpus does not physically expose this post-A3BC slot.

D0 is mechanically propagated from helper `0x004337B0`, but that producer is not a coherent full-width scalar source across all paths: its non-null route can reach RTS after reloading only a low byte without normalizing the upper 24 bits. Without a positive consumer or independent prototype, neither `void` nor a concrete scalar return type is confirmed.

## Regression note

The first static run was **20/21 FAIL** solely because expected instruction offsets for the repeated `+0x1B(SP)` loads were transcribed incorrectly. The expected list was corrected from `[0x1E,0x2C,0x36,0x4A]` to the firmware-demonstrated `[0x1E,0x2E,0x38,0x46]`; the repeated run is **21/21 PASS, OVERALL PASS**. The failed preliminary run is explicitly excluded from closure evidence.

Dynamic regression: **not executed**.
