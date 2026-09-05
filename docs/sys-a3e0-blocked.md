# A3E0 — contractual-limit disposition

Status: **mechanically reconstructed under SOURCE_FIRST; contractual return intent remains unknown**.

This public-safe note records only ABI conclusions. It does not publish firmware bytes or a callable prototype. The remaining unknown is an evidence limit, not a human blocking condition.

## Source-first correlation

Current BetaWise `os3k/syscall.c` and current `ioma8/neo-re` both map index 248 to the neutral placeholder `SYS_A3E0`. This confirms slot identity only; neither source provides a semantic vendor name or prototype.

## Generational availability

- AlphaSmart 3000 (2005): A3E0 table value is non-callable/non-pointer.
- NEO (2005): A3E0 table value is non-callable/non-pointer.
- NEO (2013): A3E0 resolves to a real wrapper.

The NEO 2013 wrapper is exactly `0x2A` bytes, SHA-256 `3cef0d7b09e46780fff32fc628d204ef79e836d3015560302ebf6e17658bea45`, decodes to 15 instructions, has one BSR.W to A3DC and one terminal ordinary return.

## Physical ABI

A3E0 receives three physical 32-bit stack slots. Their effective use is:

1. full 32-bit pointer-like value;
2. low 8 bits;
3. full 32-bit pointer-like value.

It adapts these into the seven-slot A3DC ABI, forwarding the three external values and supplying four fixed values (`0`, `0`, `0`, and `0x37`). The wrapper sign-extends the low byte of slot 2 before forwarding, but A3DC consumes only that low byte; this does not justify choosing signed versus unsigned as a contractual type. No semantic names are assigned to the arguments or constants.

## Return ambiguity

A3E0 calls A3DC, removes exactly seven forwarded slots, and returns without redefining D0. It therefore propagates A3DC's mechanical D0 unchanged.

A3DC's terminal D0 comes from a private helper chain and the observed consumers do not establish contractual width, signedness or semantic meaning. A3E0 adds no independent consumer evidence: a fresh direct-xref scan found 0 JSR, 0 JMP and 0 direct BSR to A3E0, and the official SmartApplet tables do not expose this late slot.

Consequently neither `void` nor a concrete scalar return type is published.

## Validation

The prior static regression remains PASS. A fresh source-first directed replay against the three canonical ROMs passed **16/16 assertions**, covering canonical hashes, table values, wrapper bytes/boundary/hash, decode/RTS, A3DC BSR target, seven-slot cleanup, dataflow and direct-xref negative search. Dynamic/emulator-first validation remains specified but not executed.

## Disposition

A3E0 is `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO`. No callable header is published. The ascending reaudit may continue to A3E4 without human intervention.
