# ABI trace — A3EC / index251

Disposition: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO only in `RETORNO_CONTRACTUAL=DESCONOCIDO`**.

## Primary mapping

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5E18004E` | non-pointer / no callable contract |
| NEO 2005 | `0x832C005E` | non-pointer / no callable contract |
| NEO 2013 | `0x00433E5E` | callable handler, file offset `0x023E5E` |

A3F0 begins at `0x00433F60`, fixing A3EC at `0x102` bytes. The handler decodes to 90 instructions with one terminal RTS and SHA-256 `c9f87e8436886c22b1602112155aca4f9c5ce99c7634c2e8cb5b30bcf279ad00`.

## ABI and mechanics

A3EC receives three physical 32-bit slots:

`[arg1_pointer, low8(arg2), arg3_pointer]`

The third pointer is nullable. The implementation derives records as `arg1+4+index*0x12C`, uses private table base `0x0000F43E` with stride `0x56`, accesses record fields at `+0xD8/+0xDC`, and selects private helpers based on internal state bytes. These are mechanical descriptions only; no vendor field or enum names are assigned.

The final private helper at `0x004349BE` receives a record pointer and a boolean derived from comparison of arg2 with global byte `0x0000F41A`. A3EC returns immediately after that helper apart from cleanup/register restoration, leaving D0 mechanically propagated.

## Xrefs and adversarial result

Aligned NEO 2013 sweep found zero direct JSR, JMP, BSR.W or BSR.B references to A3EC. The official SmartApplet corpus does not physically include this post-A3BC slot.

Consequently there is no positive consumer evidence fixing the contractual return. The propagated D0 prevents safely publishing `void` as a fact, while no concrete scalar width/signedness is established either.

Static regression: **26/26 PASS**. Dynamic regression: **not executed**.
