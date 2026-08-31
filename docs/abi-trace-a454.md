# SYS_A454 / index 277 — ABI trace

Status: **mechanically reconstructed (A), source-level identity blocked**.

## Availability

- AlphaSmart 3000 (2005): slot 277 is `0xF4070301`; no callable contract is claimed.
- NEO (2005): slot 277 is `0xF4070301`; no callable contract is claimed.
- NEO (2013): slot 277 resolves to runtime `0x00434980`.

The NEO 2013 handler is a short four-slot wrapper. It does **not** use the `A1` six-byte aggregate-result convention observed in the immediately preceding A43C–A450 family.

## Confirmed mechanics

The four 32-bit physical slots are consumed as follows:

1. slot 1: selector low byte; only values 1 through 8 invoke the helper, and the helper receives selector-1;
2. slot 2: full 32-bit value, stored verbatim;
3. slot 3: only its low byte ultimately affects state;
4. slot 4: only its low byte ultimately affects state.

The helper addresses one of eight 8-byte records. For the selected record it stores the slot-3 low byte zero-extended as a word at offset 0, the slot-4 low byte zero-extended as a word at offset 2, and slot 2 as a longword at offset 4. The table and fields intentionally remain unnamed because their vendor/source-level semantics have not been recovered.

## Callers

The official SmartApplet corpus contains three executable A454 callers: one in Responder and two in Wireless File Transfer. Their observed argument tuples are `(1, pointer, 0, 2)`, `(1, pointer, 0, 11)`, and `(2, pointer, 0, 2)`. NEO 2013 firmware contributes one additional direct caller. All four callers discard `D0`.

The remaining applets are negative either by executable sweep or by structural absence of the extended A-line table.

## Return discipline

For valid selectors the helper leaves a mechanically observable value in `D0`, but the invalid-selector path does not initialize `D0`, and none of the recovered callers consumes it. Consequently no scalar return width or source-level return contract is published.

## Publication boundary

No vendor function name, table-field names, enum names, or callable C prototype are asserted. ROM bytes and extended disassembly remain in the private workpapers.
