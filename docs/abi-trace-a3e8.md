# ABI trace — A3E8 / index250

Disposition: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO only in `RETORNO_CONTRACTUAL=DESCONOCIDO`**.

## Primary mapping

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5DEC004E` | non-pointer / no callable contract |
| NEO 2005 | `0x8300005E` | non-pointer / no callable contract |
| NEO 2013 | `0x00433E34` | callable wrapper, file offset `0x023E34` |

A3EC begins at `0x00433E5E`, fixing A3E8's exact length at `0x2A`. The handler has 15 instructions and one terminal RTS. SHA-256: `f43513dd2bde4eaf3f6dddd145bccae33c9072d4ee7d35862e8963f8de200363`.

## Wrapper relation

A3E8 receives three physical longword stack slots and calls A3DC with seven effective slots:

`A3DC(arg1, low8(arg2), 0, 0, arg3, 0, 0x37)`

The implementation sign-extends the selected byte internally before forwarding it, but A3DC consumes only the low byte; public signedness is therefore not inferred. The wrapper removes exactly `0x1C` bytes after A3DC and does not redefine D0.

## Xrefs

The NEO 2013 ROM contains exactly seven direct absolute JSR sites to A3E8:

`0x43E122`, `0x43E2F2`, `0x43E3D6`, `0x43E4FC`, `0x43E608`, `0x43E848`, `0x43E8F6`.

Direct JMP, BSR.W and BSR.B references are zero. At every JSR, arg1 is the value loaded from absolute global `0x00012CA0`, arg2 is zero, and arg3 is pointer-like. Larger nearby stack cleanups include arguments retained across adjacent calls and do not alter A3E8's demonstrated three-slot ABI.

## Adversarial conclusion

A3E8 is distinct from A3E0 and A3E4 by the location of its third external argument in A3DC's seven-slot frame: A3E8 forwards it as A3DC slot 5.

D0 is mechanically propagated from A3DC. The seven direct callers add no positive explicit evidence fixing return width, signedness, or intent. Therefore neither `void` nor a concrete scalar return type is published.

Static regression: **29/29 PASS**. Dynamic regression: **not executed**.
