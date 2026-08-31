# SYS_A46C / index 283 — ABI trace

Status: **mechanically reconstructed (A), source-level identity blocked**.

## Availability

| Platform | Slot disposition |
|---|---|
| AlphaSmart 3000 (2005) | raw slot value `0x29E71500`; outside the demonstrated ROM/runtime window, so no callable contract is claimed |
| NEO (2005) | raw slot value `0x29E71500`; outside the demonstrated ROM/runtime window, so no callable contract is claimed |
| NEO (2013) | slot resolves to runtime `0x00435064` |

A46C is therefore callable only in the canonical NEO 2013 image. The official SmartApplet corpus exposes an A46C stub in all 16 EXTENDED applets; 14 LEGACY applets end before this range and 11 have no A-line table.

## Confirmed mechanical ABI

The NEO 2013 handler consumes one 32-bit physical stack slot. Only the low byte is used as a selector. The selector is range-checked against `0..10`; selectors above 10 return `0xFF` in `D0.low8`.

Only `D0.low8` has a demonstrated return contract. No meaning is assigned to the upper 24 bits.

The recovered mechanical selector behavior is:

| Selector | Confirmed effect / `D0.low8` result |
|---:|---|
| 0 | return previous byte at RAM `0x42C`, then write `1` there |
| 1 | return previous byte at `0x42C`, then write `0` there |
| 2 | return previous byte at `0x42C`, then toggle bit 0 there |
| 3 | return previous byte at `0x42C`; invoke a helper with literal `1` |
| 4 | return `0xFF` |
| 5 | return `0xFF` |
| 6 | write `1` to byte at RAM `0x4350`; return its previous value |
| 7 | write `0` to byte at RAM `0x4350`; return its previous value |
| 8 | return previous byte at `0x42C` while executing a four-helper state-reset/update chain |
| 9 | return current byte at `0x42C` without changing it |
| 10 | return `1` iff bytes at RAM `0x424` and `0x425` are equal, otherwise `0` |

No vendor enum names, subsystem names, or semantic names are assigned to these selectors or RAM locations.

## Handler structure

The NEO 2013 handler is 156 bytes and contains an internal 11-entry word jump table. Its dispatcher reads only the low byte of the incoming slot, doubles the selector, indexes the table, and jumps to the selected case. The private reproducible 68000 listing resolves 45 instructions plus the 22-byte table with zero undecoded opcodes.

Selector 8 preserves the prior `0x42C` byte through its complete helper chain. One helper in that chain calls A46C internally with selector 2; this internal call does not invalidate the outer selector-8 return because the saved register carrying the prior state is preserved/restored.

## Callers and xrefs

The canonical 41-SmartApplet corpus was covered explicitly for A46C. The 16 EXTENDED applets were rematerialized and rehashed and each contains the exact late tail `A450,A454,A468,A46C`; the 14 LEGACY and 11 no-table applets are structural negatives for this slot range.

Exactly nine executable A46C callers were recovered from the official AlphaWordPlus applet, with concrete selectors:

`9, 10, 8, 6, 6, 6, 6, 6, 6`.

The NEO 2013 firmware contains exactly ten direct executable references to the handler: nine absolute-long calls and one word-relative call. Their concrete selectors are:

`1, 1, 0, 9, 10, 9, 10, 9, 10, 2`.

The final selector-2 reference is the internal call from the selector-8 helper chain. Caller contexts independently confirm a single physical stack slot and byte-sized consumption of the return value.

## Publication boundary

The public symbol remains neutral: `SYS_A46C`. No vendor function name, selector enum, subsystem identity, or semantic global name is asserted. ROM bytes and extended disassembly remain private.

Private static regression revalidated the canonical NEO 2013 ROM hash, vector resolution, handler and helper hashes/boundaries, jump table, firmware references, caller arguments, all 16 EXTENDED applet hashes/stubs, the AlphaWordPlus callers, and the neutral mechanical selector model. An initial adversarial run exposed an overly narrow test assumption for the firmware selector-0 push form; after correcting the harness without weakening the check, the final run passed **168/168** checks. Dynamic regression has not been executed.
