# A3FC — neutral blocked disposition

Status: **mechanically reconstructed; callable return contract blocked**.

This note records only public-safe ABI conclusions. It does not publish firmware bytes or a callable prototype.

## Generational availability

- AlphaSmart 3000 (2005): the A3FC table value is non-callable/non-pointer.
- NEO (2005): the A3FC table value is non-callable/non-pointer.
- NEO (2013): A3FC resolves to a real handler.

The NEO 2013 handler is exactly `0x162` bytes and contains an 8-entry internal dispatch table. The apparent interval up to A400 also contains independent routines, so it is not treated as one handler.

## Physical ABI

Four 32-bit stack slots are present:

1. a full 32-bit pointer-like value;
2. a value consumed through its low byte and sign-extended for record indexing;
3. a low-byte selector, accepted only for values 0 through 7;
4. a low-byte value whose bit 0 selects the bit value written.

The handler selects a fixed-stride record and updates one bit in one byte. The selector-to-bit mapping is `0→7, 1→6, 2→5, 3→0, 4→4, 5→3, 6→2, 7→1`. No vendor semantic names are assigned to the record, selector, or bitfield.

## Caller evidence

Two direct NEO 2013 firmware callers were found. Both pass the same pointer/index/selector shape and use the final low-byte argument to set or clear the selected bit. Neither caller supplies positive evidence that D0 is a contractual return value. Official SmartApplet A-line tables do not expose this late slot.

## Return ambiguity

Valid paths modify only portions of D0 while performing the side effect; upper bits are not uniformly normalized. The out-of-range-selector path leaves a different residual pattern. A `void` contract is therefore a strong behavioral hypothesis, but it is not promoted to a confirmed vendor/C contract without an independent prototype, symbol, or consuming caller.

Consequently no callable header is published.

## Validation

Static structural/ABI regression passed `27/27` assertions. Dynamic/emulator-first validation was not executed.

## Disposition

A3FC remains a neutral mechanically reconstructed routine, blocked specifically on **contractual return type/intent**.
