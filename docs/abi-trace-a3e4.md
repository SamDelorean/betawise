# ABI trace — A3E4 / index249

Disposition: **MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO**.

## Source-first correlation

Current BetaWise `os3k/syscall.c` and current `ioma8/neo-re` both map index 249 to the neutral placeholder `SYS_A3E4`. This establishes slot identity only; neither source supplies a semantic vendor name or callable prototype.

## Primary mapping

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5A06004E` | non-pointer / unresolved; no callable contract |
| NEO 2005 | `0x7F08005E` | non-pointer / unresolved; no callable contract |
| NEO 2013 | `0x00433E0A` | callable wrapper, file offset `0x023E0A` |

Fresh primary replay reproduced the three canonical ROM hashes and slot values. The NEO 2013 handler boundary is fixed by A3E8 at `0x00433E34`; A3E4 is `0x2A` bytes, 15 instructions, one BSR.W to A3DC and one terminal RTS. Handler SHA-256: `fa5b2875538ed5022b1c694280fea225efd6e16b4e42118ee236b46173517896`.

## Wrapper relation

A3E4 receives three 32-bit physical slots and calls A3DC with seven slots. Effective mapping:

`A3DC(arg1, low8(arg2), 0, arg3, 0, 0, 0x37)`

The wrapper removes exactly `0x1C` bytes after A3DC and returns without redefining D0. The second external slot is mechanically sign-extended from its low byte before forwarding, but A3DC consumes only that low byte; signedness is therefore not promoted to a contractual claim.

This mapping also refutes identity with A3E0: A3E0 forwards external slot 3 into A3DC slot 3, whereas A3E4 fixes A3DC slot 3 to zero and forwards external slot 3 into A3DC slot 4.

## Xrefs and corpus

Fresh aligned NEO 2013 sweep found no direct references to A3E4: 0 absolute JSR, 0 absolute JMP and 0 direct BSR. The validated official SmartApplet corpus does not physically expose this post-A3BC slot. This negative result is reproducible but is not generalized into a claim that indirect invocation is impossible.

## Return limit

D0 is mechanically propagated from A3DC, but neither a caller nor an independent semantic prototype establishes contractual width, signedness or meaning. `void` is also not selected because the wrapper preserves A3DC's D0. `RETORNO_CONTRACTUAL=DESCONOCIDO` is an evidence limit, not a human blocking condition.

## Validation

Prior static regression: **21/21 PASS**. Fresh directed source-first replay: **16/16 PASS** covering canonical hashes, slot values, wrapper bytes/boundary/hash, decode/RTS, A3DC BSR target, seven-slot cleanup, dataflow and direct-xref negative search. Dynamic/emulator-first regression remains specified but not executed.

No callable header is published. Private ROM extraction, exact firmware bytes and disassembly/workpapers remain outside the repository.
