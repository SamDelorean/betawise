# ABI trace — A3E4 / index249

Disposition: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO only in `RETORNO_CONTRACTUAL=DESCONOCIDO`**.

## Primary mapping

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5A06004E` | non-pointer / unresolved; no callable contract |
| NEO 2005 | `0x7F08005E` | non-pointer / unresolved; no callable contract |
| NEO 2013 | `0x00433E0A` | callable wrapper, file offset `0x023E0A` |

The NEO 2013 handler boundary is fixed by A3E8 at `0x00433E34`. A3E4 length is `0x2A`, with 15 instructions and one terminal RTS. Handler SHA-256: `fa5b2875538ed5022b1c694280fea225efd6e16b4e42118ee236b46173517896`.

## Wrapper relation

A3E4 receives three 32-bit physical slots and calls A3DC with seven slots. Effective mapping:

`A3DC(arg1, low8(arg2), 0, arg3, 0, 0, 0x37)`

The wrapper then removes `0x1C` bytes and returns. D0 is not modified after A3DC.

The second external slot is narrowed to its low byte by the callee-visible behavior; no signedness is claimed publicly.

## Xrefs and corpus

Direct NEO 2013 references to A3E4 found by aligned ROM sweep:

- absolute JSR: 0
- absolute JMP: 0
- BSR.W: 0
- BSR.B: 0

The validated official SmartApplet corpus does not physically expose this post-A3BC slot, so no executable A-line caller is present in the 41-app corpus. This is not generalized into a claim that no indirect caller could exist outside the available sources.

## Adversarial conclusion

A3E4 is not identical to A3E0. A3E0 forwards external arg3 into A3DC slot 3; A3E4 fixes slot 3 to zero and forwards external arg3 into A3DC slot 4.

Although D0 is mechanically propagated from A3DC, neither a caller nor an independent prototype establishes contractual width, signedness, or semantic type. `void` is also not selected because the wrapper deliberately leaves the A3DC result intact.

Static regression: **21/21 PASS**. Dynamic regression: **not executed**.
