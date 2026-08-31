# ABI trace — A3F8 / index254

Disposition: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO only in `RETORNO_CONTRACTUAL=DESCONOCIDO`**.

## Primary mapping

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5A06004E` | non-pointer / no callable contract |
| NEO 2005 | `0x7F08005E` | non-pointer / no callable contract |
| NEO 2013 | `0x00434048` | callable handler, file offset `0x024048` |

A3FC begins at `0x0043409C`, fixing A3F8 at `0x54` bytes. The handler decodes to 28 instructions with one terminal RTS and SHA-256 `8d7755d3568883fe8e3bdb655fd2ff183e692e357da85637cbc0a246223935e3`.

## ABI and stack rebasing

The physical ABI contains four longword slots:

`[arg1_pointer, low8(arg2), low8(arg3), low8(arg4)]`

The routine derives `primary = arg1 + 4 + signext8(arg2) * 0x12C`. Apparent repeated byte reads at the same stack displacement execute after successive pushes; rebased to the entry frame they select arg4, arg3 and arg2.

The private helper call is mechanically equivalent to:

`helper_0x4337B0(primary+0x112, signext8(arg3), signext8(arg4), primary+0xD0)`

These implementation extensions do not establish public signedness.

## Xrefs and return

Aligned NEO 2013 sweep found zero direct JSR, JMP, BSR.W or BSR.B references to A3F8. The official SmartApplet corpus does not physically expose this late slot, yielding 0/41 executable A-line callers.

A3F8 returns without redefining D0 after helper `0x004337B0`. That helper is not a coherent full-width scalar producer across all paths: its null route leaves zero, while a non-null route can reload only the low byte without normalizing the upper 24 bits before RTS. With no positive consumer or independent prototype, neither `void` nor a concrete scalar return type is confirmed.

## Regression

Static regression: **20/20 PASS, OVERALL PASS**.

Dynamic/emulator-first regression: **not executed**.
