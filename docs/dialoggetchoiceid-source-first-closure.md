# A10C / index67 — DialogGetChoiceId — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
int DialogGetChoiceId(void);
```

`DialogGetChoiceId` is the direct A-line syscall at index 67 (`A10C`). The existing BetaWise declaration is ABI-correct; no wrapper correction is required.

## Confirmed behavior

The canonical AS3000 2005, NEO 2005 and NEO 2013 implementations all perform the same operation after relocation normalization:

1. zero-extend the dialog `current_choice` byte;
2. convert the normal 1-based choice to a zero-based array index;
3. scale that index by four bytes;
4. load the corresponding 32-bit entry from the dialog item-ID array;
5. return that full longword in `D0`.

Equivalent semantics:

```c
return item_ids[current_choice - 1];
```

The handler has no argument slots and performs no explicit range check against `item_count`. A valid dialog choice is therefore a caller precondition.

## Producer/consumer correlation

`DialogAddItem` (`A0F4`) stores its user-supplied `id` argument as a 32-bit value in the same per-item array that `DialogGetChoiceId` reads. `DialogSetChoice`/`DialogGetChoice` (`A0FC`/`A108`) independently identify the state byte used as the selector.

This distinguishes the two getters cleanly:

- `DialogGetChoice()` returns the 1-based selected item index as a byte.
- `DialogGetChoiceId()` returns the selected item's user-defined 32-bit ID.

## Generation comparison

The A10C handler is 0x18 bytes in all three canonical generations. The only differences are relocation of `current_choice` and the item-ID array base. After normalizing those operands, the handlers are byte-identical.

Direct absolute firmware callers found during the private audit: 10 in AS3000 2005, 11 in NEO 2005 and 11 in NEO 2013. Several callers immediately consume the full `D0` longword, independently supporting the public `int` return width.

## Regression status

Static regression: **EXECUTED — 56/56 PASS**.

It verifies canonical ROM hashes, vector neighborhood 66..68, exact 0x18 handler bounds, complete instruction grammar, `current_choice` and item-ID operands, producer/consumer correlation with `A0F4`, caller/reference counts and normalized three-generation equivalence.

Dynamic DialogProbe regression: **SPECIFIED / NOT EXECUTED**. A minimal probe should add items with distinct 32-bit IDs, select each item in turn and verify that `DialogGetChoiceId()` returns the exact corresponding longword.

## Confidence

- **CONFIRMED:** identity, no-argument ABI, full 32-bit return, 1-based-to-zero-based index conversion, x4 scaling, item-ID array lookup, lack of range validation, A0F4/A108 state correlation and three-generation equivalence.
- **STRONG INFERENCE:** normal callers must maintain `current_choice` within `1..item_count`.
- **UNKNOWN:** meaningful behavior after deliberately corrupting the dialog selection state; this is outside the normal API contract.

Raw firmware, private disassembly, ROM offsets and audit corpus remain outside the public repository.
