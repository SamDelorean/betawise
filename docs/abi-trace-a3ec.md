# ABI trace — A3EC / index251

Disposition: **MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO**.

## Source-first correlation

Current BetaWise `os3k/syscall.c` and `ioma8/neo-re` `smartapplets/betawise-sdk/syscall.c` both map index 251 to the neutral placeholder `SYS_A3EC`. This establishes slot identity only; neither source supplies an independent semantic vendor name or callable prototype.

## Primary mapping

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5E18004E` | non-pointer / unresolved; no callable contract |
| NEO 2005 | `0x832C005E` | non-pointer / unresolved; no callable contract |
| NEO 2013 | `0x00433E5E` | callable handler, file offset `0x023E5E` |

A3F0 begins at `0x00433F60`, fixing A3EC at `0x102` bytes. Fresh primary replay reproduced all canonical ROM hashes, the three slot values, the A3F0 boundary and handler SHA-256 `c9f87e8436886c22b1602112155aca4f9c5ce99c7634c2e8cb5b30bcf279ad00`. The project-created 68000 decoder independently reproduced 90 instructions across the complete handler and one terminal RTS at `+0x100`.

## ABI and mechanics

A3EC receives three physical 32-bit slots:

`[arg1_pointer, low8(arg2), arg3_pointer]`

The third pointer is nullable. The implementation derives records as `arg1 + 4 + index * 0x12C`, uses private table base `0x0000F43E` with stride `0x56`, accesses record fields at `+0xD8/+0xDC`, and selects private helpers based on internal state bytes. These are mechanical descriptions only; no vendor field or enum names are assigned.

The handler also references private helpers at `0x00432848`, `0x00442A1A` and `0x00442A98`. Its final helper at `0x004349BE` receives a record pointer and a Boolean derived from comparison of arg2 with global byte `0x0000F41A`. A3EC returns after cleanup/register restoration without redefining D0, so the final helper's D0 is mechanically propagated.

Although the implementation sign-extends the low byte of arg2 for indexed arithmetic, no independent caller/prototype establishes signedness as an external contract.

## Xrefs and adversarial result

Fresh aligned NEO 2013 sweep reproduced zero direct JSR, JMP, BSR.W or BSR.B references to A3EC. The validated official SmartApplet corpus does not physically include this post-A3BC slot. This negative result is not generalized into a claim that indirect invocation is impossible.

A3EC is not a trivial wrapper analogous to A3E0/A3E4/A3E8: it contains its own state/table/helper control flow and a substantially larger 90-instruction body.

## Return limit

D0 from the final helper reaches the return path mechanically, but there is no positive caller consumption or independent semantic prototype fixing return width, signedness or intent. Therefore neither `void` nor a concrete scalar return type is published.

`RETORNO_CONTRACTUAL=DESCONOCIDO` is an evidence limit, not a human blocking condition. No callable header is published.

## Validation

Prior static regression: **26/26 PASS**. Fresh directed source-first replay on 2026-09-05: **26/26 PASS**, plus a separate fresh decoder replay reproducing the 90-instruction/0x102-byte boundary. Coverage includes canonical hashes, generation slots, A3F0 boundary, handler length/hash, argument-load anchors, final helper branch target and cleanup, direct-xref negative search, and table/global/stride/helper anchors.

Dynamic/emulator-first regression remains **specified but not executed**.

Private ROM extraction, exact firmware bytes and extensive disassembly/workpapers remain outside the repository.
