# ABI trace — A3F4 / index253

Disposition: **MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / `RETORNO_CONTRACTUAL=DESCONOCIDO`**. The unknown return contract is an evidence boundary, not a human-blocking condition.

## Source-first correlation

Historical BetaWise `syscall.c` and `ioma8/neo-re` expose index 253 only as neutral placeholder `SYS_A3F4`. Neither source provides a vendor symbol or semantic prototype, so no semantic name is synthesized.

## Primary mapping — freshly revalidated

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5EC6004E` | non-pointer / no callable contract |
| NEO 2005 | `0x83D2005E` | non-pointer / no callable contract |
| NEO 2013 | `0x00433FE2` | callable handler, file offset `0x023FE2` |

The canonical ROM hashes were rechecked directly before analysis. A3F8 starts at `0x00434048`, fixing A3F4 at `0x66` bytes. Handler SHA-256 is `0f3e2c9544939309eca0dcb9bb981561956df36a1b1e47aa2e12802c1b8e17c3`.

## ABI and stack rebasing

The physical ABI contains five longword slots:

`[arg1_pointer, low8(arg2), low8(arg3), low8(arg4), low8(arg5)]`

Four instructions use apparent displacement `+0x1B(SP)` after different push depths. Rebased to the entry frame, handler offsets `+0x1E`, `+0x2E`, `+0x38`, and `+0x46` select arg5, arg4, arg3, and arg2 respectively.

The routine forms a primary record from `arg1+4+signext8(arg2)*0x12C`, a secondary subrecord from `+0x34+zeroext8(arg5)*0x1A`, and invokes private helper `0x004337B0` with:

`[primary+0x11E, signext8(arg3), signext8(arg4), secondary]`

These extensions are implementation mechanics, not public type claims.

## Xrefs and adversarial return check

A fresh aligned NEO 2013 sweep finds zero direct JSR, JMP, or BSR.W targets to A3F4. This confirms the earlier negative-xref result without treating it as proof against indirect use.

D0 is mechanically propagated from helper `0x004337B0`, but the helper's non-null path can reload only its low byte and reach RTS without normalizing the upper 24 bits. Consequently a coherent full-width scalar return is not supported. `void` remains an INFERENCIA MUY FUERTE from known use, but is not promoted to CONFIRMADO and no callable header is emitted.

## Regression state

The historical preliminary **20/21 FAIL** was caused solely by incorrectly transcribed stack-rebased offsets and remains excluded from closure evidence. The corrected historical run is **21/21 PASS**.

Fresh primary directed regression: **23/23 PASS**, rechecking ROM hashes, slot values, A3F8 boundary, handler length/hash, stack/stride/helper byte anchors, four rebased argument loads, negative direct xrefs, and helper low-byte behavior.

Dynamic/emulator-first regression: **ESPECIFICADA / NO EJECUTADA**.
