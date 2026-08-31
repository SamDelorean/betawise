# A3EC — neutral blocked disposition

Status: **mechanically reconstructed; callable contract blocked only on return intent/type**.

## Generational availability

- AlphaSmart 3000 (2005): table value `0x5E18004E`, non-pointer/non-callable.
- NEO (2005): table value `0x832C005E`, non-pointer/non-callable.
- NEO (2013): runtime `0x00433E5E`, file offset `0x023E5E`.

The NEO 2013 handler is exactly `0x102` bytes, 90 decoded 68000 instructions and one terminal RTS. Handler SHA-256: `c9f87e8436886c22b1602112155aca4f9c5ce99c7634c2e8cb5b30bcf279ad00`.

## Physical ABI

The prologue proves three physical stack slots with effective use:

`[pointer/full32, low8, pointer/full32]`

The third pointer is explicitly nullable. The second byte is sign-extended internally for index arithmetic, but no external signedness is claimed.

The routine indexes a structure rooted at the first pointer using stride `0x12C`, interacts with an internal table rooted at `0x0000F43E` using stride `0x56`, and invokes private helpers. These mechanics are documented without assigning vendor names, enums or semantic field labels.

## Return ambiguity

The final operation calls an internal helper at `0x004349BE`, then restores registers and returns without redefining D0. That helper itself propagates D0 from deeper private calls rather than normalizing a clearly typed result.

No direct JSR/JMP/BSR firmware xref to A3EC was found, and the validated official SmartApplet corpus does not physically expose this late slot. There is therefore no positive consumer evidence fixing return width, signedness or intent. Neither `void` nor a concrete scalar type is published.

## Validation

Static structural/ABI regression: **26/26 assertions, OVERALL PASS**. Dynamic/emulator-first validation was not executed.

Disposition: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`**. No callable header is published.
