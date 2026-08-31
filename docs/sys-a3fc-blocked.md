# A3FC — neutral blocked disposition

Status: **mechanically reconstructed; callable return contract blocked**.

This note records only public-safe ABI conclusions. It does not publish firmware bytes or a callable prototype.

## Generational availability

- AlphaSmart 3000 (2005): the A3FC table value is non-callable/non-pointer.
- NEO (2005): the A3FC table value is non-callable/non-pointer.
- NEO (2013): A3FC resolves to a real handler at runtime `0x0043409C`.

The NEO 2013 handler is exactly `0x162` bytes, has one terminal RTS, and contains an 8-word internal dispatch table. The apparent interval up to A400 also contains independent routines, so it is not treated as one handler. Handler SHA-256: `b039c5eb7824a83caa675ee779cf4ec5a02f85439509b883b81b85f20ad29bb8`.

## Physical ABI

Four 32-bit stack slots are present:

1. a full 32-bit pointer-like value;
2. a value consumed through its low byte and sign-extended for record indexing;
3. a low-byte selector, accepted only for values 0 through 7;
4. a low-byte value whose bit 0 selects the bit value written.

The handler selects a fixed-stride `0x12C` record and updates one bit in the byte at record-relative offset `0x10D`. The dispatch table maps the selector monotonically to the reversed bit number:

`0→7, 1→6, 2→5, 3→4, 4→3, 5→2, 6→1, 7→0`.

This corrects the initial concurrent publication, which had transcribed the physical case layout rather than following the jump-table targets for selectors 3–7. No vendor semantic names are assigned to the record, selector, or bitfield.

## Caller evidence

Two direct NEO 2013 firmware callers were found. They invoke the routine with the same pointer/index/selector shape and use the final low-byte argument to set and clear selector `5`, respectively. Neither caller consumes D0 before another operation supersedes it. Official SmartApplet A-line tables do not expose this late slot, so executable SmartApplet callers are structurally `0/41`.

## Return ambiguity

Valid paths modify only the low byte of D0 while performing the side effect; upper bits are not uniformly normalized. The out-of-range-selector path leaves a different residual pattern. A `void` contract is therefore a very strong behavioral hypothesis, but it is not promoted to a confirmed vendor/C contract without an independent prototype, symbol, or consuming caller.

Consequently no callable header is published.

## Validation

An extended static structural/ABI regression passes `36/36` assertions, including the exact jump-table words and selector targets, all eight bit paths, canonical ROM hashes, vector slots, handler boundary/hash, direct xrefs and both caller argument shapes. Dynamic/emulator-first validation was not executed.

## Disposition

A3FC remains a neutral mechanically reconstructed routine, blocked specifically on **contractual return type/intent**.
