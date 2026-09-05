# A3E4 — contractual-limit disposition

Status: **mechanically reconstructed under SOURCE_FIRST; contractual return intent remains unknown**.

This public-safe note records only ABI conclusions. It does not publish firmware bytes or a callable prototype. The remaining unknown is an evidence limit, not a human blocking condition.

## Source-first correlation

Current BetaWise `os3k/syscall.c` and current `ioma8/neo-re` both map index 249 to the neutral placeholder `SYS_A3E4`. This confirms slot identity only; neither source provides a semantic vendor name or prototype.

## Generational availability

- AlphaSmart 3000 (2005): A3E4 table value is non-callable/non-pointer.
- NEO (2005): A3E4 table value is non-callable/non-pointer.
- NEO (2013): A3E4 resolves to a real wrapper at runtime `0x00433E0A`.

The NEO 2013 wrapper is exactly `0x2A` bytes, SHA-256 `fa5b2875538ed5022b1c694280fea225efd6e16b4e42118ee236b46173517896`, decodes to 15 instructions, has one BSR.W to A3DC and one terminal ordinary return.

## Physical ABI

A3E4 receives three physical 32-bit stack slots with effective use full32, low8, full32. It adapts them into the seven-slot A3DC ABI as:

`[arg1, low8(arg2), 0, arg3, 0, 0, 0x37]`

The wrapper cleans exactly seven forwarded slots after A3DC. Its slot mapping is demonstrably distinct from A3E0. No semantic names are assigned to arguments or constants.

## Return ambiguity

A3E4 returns without redefining D0 after A3DC, so it mechanically propagates A3DC's D0. A3DC has no demonstrated contractual return width/signedness/meaning, and A3E4 adds no independent consumer evidence: fresh direct-xref scan found 0 JSR, 0 JMP and 0 direct BSR, while the validated official SmartApplet corpus does not expose this late slot.

Consequently neither `void` nor a concrete scalar return type is published.

## Validation

Prior static structural/ABI regression: **21/21 PASS**. Fresh directed source-first replay: **16/16 PASS**. Dynamic/emulator-first validation remains specified but not executed.

## Disposition

A3E4 is `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO`. No callable header is published. The ascending reaudit may continue to A3E8 without human intervention.
