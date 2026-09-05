# ABI trace — A3E8 / index250

Disposition: **MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO**.

## Source-first correlation

Current BetaWise `os3k/syscall.c` and `ioma8/neo-re` `smartapplets/betawise-sdk/syscall.c` both map index 250 to the neutral placeholder `SYS_A3E8`. This establishes slot identity only. Neither source supplies a semantic vendor name or callable prototype, so no stronger public name is assigned.

## Primary mapping

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5DEC004E` | non-pointer / unresolved; no callable contract |
| NEO 2005 | `0x8300005E` | non-pointer / unresolved; no callable contract |
| NEO 2013 | `0x00433E34` | callable wrapper, file offset `0x023E34` |

A3EC begins at `0x00433E5E`, fixing A3E8's exact length at `0x2A`. Fresh primary replay reproduced the canonical hashes, all three slot values, the handler boundary and handler SHA-256 `f43513dd2bde4eaf3f6dddd145bccae33c9072d4ee7d35862e8963f8de200363`. The NEO 2013 wrapper has 15 decoded instructions, one terminal RTS and one BSR.W to A3DC at `0x00433C18`.

## Wrapper relation

A3E8 receives three physical 32-bit stack slots and calls A3DC with seven effective slots:

`A3DC(arg1, sign_extend_8(low8(arg2)), 0, 0, arg3, 0, 0x37)`

The implementation sign-extends the selected byte internally before forwarding it, but A3DC consumes only the low byte; public signedness is therefore not inferred. The wrapper removes exactly `0x1C` bytes after A3DC and does not redefine D0.

This mapping refutes identity with A3E0 and A3E4: A3E8 fixes A3DC slots 3 and 4 to zero and forwards its third external slot as A3DC slot 5.

## Xrefs and caller dataflow

Fresh aligned NEO 2013 sweep reproduced exactly seven direct absolute JSR sites to A3E8:

`0x43E122`, `0x43E2F2`, `0x43E3D6`, `0x43E4FC`, `0x43E608`, `0x43E848`, `0x43E8F6`.

Direct JMP, BSR.W and BSR.B references are zero. At every JSR, arg1 is the full longword loaded from absolute global `0x00012CA0`, arg2 is zero, and arg3 is pointer-like. The seven third-slot sources comprise A2, an immediate address, a value loaded from `4(A0)`, or another immediate address. Larger nearby stack cleanups include arguments retained across adjacent calls and do not change A3E8's demonstrated three-slot ABI.

The validated official SmartApplet corpus does not physically expose this post-A3BC slot; that negative result is not generalized into a claim that indirect invocation is impossible.

## Return limit

D0 is mechanically propagated from A3DC, but A3DC's own return contract remains unresolved and the seven direct A3E8 callers do not provide positive explicit evidence fixing return width, signedness or semantic meaning. `void` is also not promoted to fact merely because those callers do not visibly consume D0.

Accordingly, `RETORNO_CONTRACTUAL=DESCONOCIDO` is an evidence limit, not a human blocking condition. No callable header is published.

## Validation

Prior static regression: **29/29 PASS**. Fresh directed source-first replay on 2026-09-05: **39/39 PASS**, covering canonical hashes, slot values, A3EC boundary, handler bytes/boundary/hash, decode/RTS, A3DC branch target using the 68k extension-word PC base, seven-slot cleanup and dataflow, exact seven direct JSR sites, negative JMP/BSR search, common caller arguments and all seven pointer-like third-slot source forms.

Dynamic/emulator-first regression remains **specified but not executed**.

Private ROM extraction, exact firmware bytes and extensive disassembly/workpapers remain outside the repository.
