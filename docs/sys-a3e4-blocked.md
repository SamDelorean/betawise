# A3E4 — neutral blocked disposition

Status: **mechanically reconstructed; callable contract blocked only on return intent/type**.

This note records only public-safe ABI conclusions. It does not publish firmware bytes or a callable prototype.

## Generational availability

- AlphaSmart 3000 (2005): A3E4 table value is non-callable/non-pointer.
- NEO (2005): A3E4 table value is non-callable/non-pointer.
- NEO (2013): A3E4 resolves to a real wrapper at runtime `0x00433E0A`.

The NEO 2013 wrapper is exactly `0x2A` bytes, SHA-256 `fa5b2875538ed5022b1c694280fea225efd6e16b4e42118ee236b46173517896`, and terminates with one ordinary return.

## Physical ABI

A3E4 receives three physical 32-bit stack slots. Their effective use is full32, low8, full32.

It adapts those into the seven-slot A3DC ABI as:

`[arg1, low8(arg2), 0, arg3, 0, 0, 0x37]`

The wrapper cleans exactly seven forwarded slots after the A3DC call. No semantic names are assigned to the arguments or fixed value.

## Return ambiguity

A3E4 returns without redefining D0 after A3DC, so it mechanically propagates A3DC's D0. A3DC itself remains blocked on contractual return width/intent, and A3E4 contributes no independent return evidence: no direct firmware caller of A3E4 was found, the validated official SmartApplet corpus does not expose this late slot, and no independent prototype or recovered vendor symbol was found.

Consequently neither `void` nor a concrete scalar return type is published.

## Validation

Static structural/ABI regression: **21/21 assertions, OVERALL PASS**. Dynamic/emulator-first validation was not executed.

## Disposition

A3E4 remains a neutral mechanically reconstructed wrapper, blocked specifically on **contractual return type/intent**. No callable header is published.
