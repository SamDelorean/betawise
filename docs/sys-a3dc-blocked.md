# A3DC — contractual-limit disposition

Status: **mechanically reconstructed under SOURCE_FIRST; contractual return intent remains unknown**.

This public-safe note records only ABI conclusions and does not publish firmware bytes or a callable prototype. The remaining unknown is an evidence limit, not a human blocking condition.

## Source-first correlation

Current BetaWise `os3k/syscall.c` and current `ioma8/neo-re` both map index 247 to the neutral placeholder `SYS_A3DC`. This confirms the slot identity only; neither source provides a semantic vendor name or prototype.

A prior private workpaper recorded the `neo-re` search as negative. A fresh correlation pass found the explicit placeholder mapping, so that negative-search statement is superseded. The correction does not change the firmware-derived contract limits.

## Generational availability

- AlphaSmart 3000 (2005): A3DC table value is non-callable/non-pointer.
- NEO (2005): A3DC table value is non-callable/non-pointer.
- NEO (2013): A3DC resolves to a real handler.

The NEO 2013 handler is exactly `0x196` bytes, SHA-256 `0859a060b3ed12b97329ba6f75ecaf4596e11443479dab8817ae670ea6d9d200`, with 140 decoded instructions and one ordinary return. The remaining bytes before the next A-line entry form a separate `0x32`-byte routine and are not part of A3DC.

## Physical ABI

Seven physical 32-bit stack slots are demonstrated by the handler and firmware call sites. Their effective use is:

1. full 32-bit pointer-like value;
2. low 8 bits;
3. full 32-bit pointer-like value;
4. full 32-bit nullable/pointer-like value;
5. full 32-bit nullable/pointer-like value;
6. full 32-bit nullable/pointer-like value;
7. low 16 bits.

Semantic argument names remain unknown.

## Return ambiguity

Eight direct firmware calls to A3DC were identified: four absolute JSRs and four BSR.W calls. Three neighboring A-line handlers, A3E0/A3E4/A3E8, pass seven slots to A3DC and return without redefining D0, so D0 is mechanically propagated and should not be assumed dead or `void`.

However, A3DC does not normalize D0 locally. Its terminal D0 is propagated through a private helper chain, and the firmware consumers examined do not establish byte/word/long width, signedness, or semantic meaning before D0 is overwritten. The official SmartApplet tables do not expose this late slot, and no independent semantic prototype or recovered vendor symbol has been established.

For that reason neither `void` nor a concrete scalar return type is published.

## Validation

The existing reproducible static regression passed **38/38 assertions** (`OVERALL PASS`). A fresh directed replay against the three canonical ROMs reproduced the canonical hashes, table values, handler boundary/hash/CFG, separate neighbor boundary and direct xref sets. Dynamic/emulator-first validation remains specified but not executed.

## Disposition

A3DC is `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO`. No callable header is published. The ascending reaudit may continue to A3E0 without human intervention.
