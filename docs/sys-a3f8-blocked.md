# A3F8 — neutral blocked disposition

Status: **mechanically reconstructed; callable contract blocked only on return intent/type**.

## Generational availability

- AlphaSmart 3000 (2005): table value `0x5A06004E`, non-pointer/non-callable.
- NEO (2005): table value `0x7F08005E`, non-pointer/non-callable.
- NEO (2013): runtime `0x00434048`, file offset `0x024048`.

The NEO 2013 handler is exactly `0x54` bytes, 28 decoded 68000 instructions, with one terminal RTS. Handler SHA-256: `8d7755d3568883fe8e3bdb655fd2ff183e692e357da85637cbc0a246223935e3`.

## Physical ABI

A3F8 has four physical 32-bit stack slots with effective use:

`[arg1_pointer, low8(arg2), low8(arg3), low8(arg4)]`

The handler derives `primary = arg1 + 4 + signext8(arg2) * 0x12C`. Repeated byte loads using the same apparent stack displacement are executed after different pushes and therefore rebase to arg4, arg3 and arg2 respectively.

It invokes private helper `0x004337B0` with values derived from `primary+0x112`, arg3, arg4 and `primary+0xD0`, then returns without redefining D0.

## Return ambiguity

Aligned NEO 2013 analysis found no direct JSR, JMP, BSR.W or BSR.B xrefs to A3F8. The validated official SmartApplet corpus does not physically expose this post-A3BC slot.

The private helper leaves D0 as zero on one path, while another path can reload only the low byte without normalizing the upper 24 bits before RTS. A3F8 propagates that value mechanically, but there is no positive caller/consumer or independent prototype establishing a scalar return contract.

`void` remains a very strong intent inference, not a confirmed contract. No concrete scalar type is published.

## Validation

Static regression passes **20/20 assertions, OVERALL PASS**. Dynamic/emulator-first validation was not executed.

Disposition: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`**. No callable header is published.
