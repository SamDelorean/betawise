# A3E0 — neutral blocked disposition

Status: **mechanically reconstructed; callable contract blocked**.

This note records only public-safe ABI conclusions. It does not publish firmware bytes or a callable prototype.

## Generational availability

- AlphaSmart 3000 (2005): A3E0 table value is non-callable/non-pointer.
- NEO (2005): A3E0 table value is non-callable/non-pointer.
- NEO (2013): A3E0 resolves to a real wrapper.

The NEO 2013 wrapper is exactly `0x2A` bytes, SHA-256 `3cef0d7b09e46780fff32fc628d204ef79e836d3015560302ebf6e17658bea45`, and terminates with one ordinary return.

## Physical ABI

A3E0 receives three physical 32-bit stack slots. Their effective use is:

1. full 32-bit pointer-like value;
2. low 8 bits;
3. full 32-bit pointer-like value.

It adapts these into the seven-slot A3DC ABI, forwarding the three external values and supplying four fixed values (`0`, `0`, `0`, and `0x37`). No semantic names are assigned to these arguments or constants.

## Return ambiguity

A3E0 calls A3DC, removes the seven forwarded slots, and returns without redefining D0. It therefore propagates A3DC's mechanical D0 unchanged.

A3DC itself is blocked on contractual return width/intent because its final D0 comes from a private helper chain and no consumer establishes byte/word/long width or signedness. A3E0 adds no independent evidence: no direct firmware caller of A3E0 was found, the official SmartApplet tables do not expose this late slot, and no independent prototype or recovered vendor symbol was found in the reference codebases checked.

Consequently neither `void` nor a concrete scalar return type is published.

## Validation

Static structural/ABI regression passed. Dynamic/emulator-first validation was not executed.

## Disposition

A3E0 remains a neutral mechanically reconstructed wrapper, blocked specifically on **contractual return type/intent**. No callable header is published.
