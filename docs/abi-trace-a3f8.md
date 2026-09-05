# ABI trace — A3F8 / index254

Disposition: **MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / `RETORNO_CONTRACTUAL=DESCONOCIDO`**. The unknown return contract is an evidence boundary, not a human-blocking condition.

## Source-first correlation

Historical BetaWise `syscall.c` and `ioma8/neo-re` expose index 254 only as neutral placeholder `SYS_A3F8`; neither provides a vendor symbol or semantic prototype. No semantic name is synthesized.

## Primary mapping — freshly revalidated

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5A06004E` | non-pointer / no callable contract |
| NEO 2005 | `0x7F08005E` | non-pointer / no callable contract |
| NEO 2013 | `0x00434048` | callable handler, file offset `0x024048` |

The canonical ROM hashes were rechecked directly before analysis. A3FC begins at `0x0043409C`, fixing A3F8 at `0x54` bytes. Handler SHA-256 is `8d7755d3568883fe8e3bdb655fd2ff183e692e357da85637cbc0a246223935e3`.

## ABI and stack rebasing

The physical ABI contains four longword slots:

`[arg1_pointer, low8(arg2), low8(arg3), low8(arg4)]`

The routine derives `primary = arg1 + 4 + signext8(arg2) * 0x12C`. Three apparent byte reads from the same `+0x1B(SP)` displacement occur after successive pushes and therefore rebase to arg4, arg3, and arg2 respectively.

The private helper call is mechanically equivalent to:

`helper_0x4337B0(primary+0x112, signext8(arg3), signext8(arg4), primary+0xD0)`

These extensions are implementation mechanics only and do not establish public signedness.

## Xrefs and adversarial return check

A fresh aligned NEO 2013 sweep finds zero direct JSR, JMP, or BSR.W targets to A3F8. This confirms the earlier negative-xref result without excluding indirect use.

A3F8 returns without redefining D0 after helper `0x004337B0`. That helper is not a coherent full-width scalar producer across all paths: its null route yields zero, while a non-null route can reload only the low byte without normalizing the upper 24 bits before RTS. Therefore a concrete scalar return contract is not justified. `void` remains an INFERENCIA MUY FUERTE, not CONFIRMADO, and no callable header is emitted.

## Regression state

Historical static regression: **20/20 PASS**.

Fresh primary directed regression: **22/22 PASS**, rechecking all three ROM hashes, all slot values, A3FC boundary, handler length/hash, stride/subfield/helper byte anchors, three stack-rebased byte loads, negative direct xrefs, and the shared helper behavior.

Dynamic/emulator-first regression: **ESPECIFICADA / NO EJECUTADA**.
