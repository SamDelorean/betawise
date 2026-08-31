# A3DC — neutral blocked disposition

Status: **mechanically reconstructed; callable contract blocked**.

This public-safe note records only ABI conclusions and does not publish firmware bytes or a callable prototype.

## Generational availability

- AlphaSmart 3000 (2005): A3DC table value is non-callable/non-pointer.
- NEO (2005): A3DC table value is non-callable/non-pointer.
- NEO (2013): A3DC resolves to a real handler.

The NEO 2013 handler is exactly `0x196` bytes, SHA-256 `0859a060b3ed12b97329ba6f75ecaf4596e11443479dab8817ae670ea6d9d200`, with 140 decoded instructions and one ordinary return. The remaining bytes before the next A-line entry form a separate routine and are not part of A3DC.

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

Eight direct firmware calls to A3DC were identified: four absolute JSRs and four BSR.W calls. Three neighboring A-line handlers, A3E0/A3E4/A3E8, are wrappers that pass seven slots to A3DC and return without redefining D0, so D0 is mechanically propagated and should not simply be assumed dead or `void`.

However, A3DC does not normalize D0 locally. Its final D0 is propagated through a private helper chain, and the firmware consumers examined do not establish byte/word/long width or signedness before D0 is overwritten. The official SmartApplet tables do not expose this late slot, and no independent prototype or recovered vendor symbol was found in the reference codebases checked.

For that reason neither `void` nor a concrete scalar return type is published.

## Validation

A reproducible static regression passed **38/38 assertions** (`OVERALL PASS`). Dynamic/emulator-first validation was not executed.

## Disposition

A3DC remains a neutral mechanically reconstructed block, blocked specifically on **contractual return type/intent**. No callable header is published.
