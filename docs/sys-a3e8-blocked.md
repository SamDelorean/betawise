# A3E8 — neutral blocked disposition

Status: **mechanically reconstructed; callable contract blocked only on return intent/type**.

## Generational availability

- AlphaSmart 3000 (2005): table value `0x5DEC004E`, non-pointer/non-callable.
- NEO (2005): table value `0x8300005E`, non-pointer/non-callable.
- NEO (2013): A3E8 resolves to runtime `0x00433E34`, file offset `0x023E34`.

The NEO 2013 wrapper is exactly `0x2A` bytes, 15 instructions, with one terminal RTS. Handler SHA-256: `f43513dd2bde4eaf3f6dddd145bccae33c9072d4ee7d35862e8963f8de200363`.

## Physical ABI

A3E8 receives three physical 32-bit stack slots. It adapts them to the seven-slot A3DC ABI as:

`[arg1, low8(arg2), 0, 0, arg3, 0, 0x37]`

No semantic names or signedness are assigned beyond what the firmware proves. D0 is left untouched after A3DC and therefore mechanically propagates A3DC's result.

## Caller evidence

NEO 2013 contains seven direct absolute JSR sites to A3E8. In all seven, the first argument is loaded from the same absolute global, the second argument is zero, and the third value is pointer-like. No direct JMP/BSR reference was found. The official SmartApplet corpus does not physically expose this late slot.

The seven firmware sites do not provide positive explicit consumption of D0 before subsequent flow/calls. That negative evidence is insufficient to declare the function `void`, while A3DC itself remains blocked on contractual return width/intent. A concrete scalar return type is likewise unjustified.

## Validation and disposition

Static structural/ABI regression: **29/29 assertions, OVERALL PASS**. Dynamic/emulator-first validation was not executed.

A3E8 therefore remains **MECÁNICA_RECONSTRUIDA A / BLOQUEADO exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`**. No callable header is published.
