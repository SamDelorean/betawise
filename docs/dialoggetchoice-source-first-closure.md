# A108 / index66 — DialogGetChoice — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
char DialogGetChoice(void);
```

The syscall table places `DialogGetChoice` at index 66, therefore its A-line opcode is `A108`. No separate C wrapper is required for the recovered public contract.

## Confirmed behavior

The canonical AS3000 2005, NEO 2005, and NEO 2013 firmware implementations have the same semantic body after normalizing the relocated dialog-state address:

1. read the dialog `current_choice` byte into `D0.B`;
2. return.

The handler has no arguments, helper calls, validation, range clamp, or side effects.

Only the low byte of the return register is explicitly established by the handler. Code must therefore treat the byte-sized return value as contractual rather than assuming the untouched upper portion of `D0` carries meaningful state.

## Setter/getter correlation

`DialogSetChoice` (`A0FC`, index63) writes exactly the same `current_choice` state byte that `DialogGetChoice` reads. Together with `DialogInit`, which initializes the normal selection to 1, this forms a directly verified setter/getter relationship.

The raw getter itself does not enforce a valid 1-based range. If some earlier path stores an unusual byte value, `DialogGetChoice` returns that byte literally.

## Distinction from item IDs

`DialogGetChoice` returns the 1-based choice index state, not the caller-provided item ID. The next dialog syscall, `DialogGetChoiceId` (`A10C`), is the API that resolves the selected item's metadata ID.

## Cross-generation result

Each canonical implementation is exactly eight bytes long and differs only in the relocated absolute address of `current_choice`. After normalizing that address, the handlers are identical.

Direct absolute firmware callers found during the audit are 1 in AS3000 2005, 1 in NEO 2005, and 2 in NEO 2013. In each image, the only additional raw reference to the A108 entry is its A-line vector entry.

## Static regression

Static regression: **EXECUTED — 40/40 PASS**.

It verifies canonical ROM hashes, index66/index67 vector resolution, exact handler length, complete `MOVE.B abs.l,D0` + `RTS` grammar, the generation-specific `current_choice` address, direct correlation with the A0FC setter, caller/reference counts, and normalized three-generation equivalence.

## Confidence

- **CONFIRMED:** A108/index66 identity; zero-argument byte return contract; read-only `current_choice` semantics; same state byte as `DialogSetChoice`; no validation/helpers/side effects; three-generation equivalence; caller counts.
- **STRONG INFERENCE:** normal callers expect the 1-based dialog-choice domain established by the surrounding dialog subsystem; the getter itself does not enforce it.

Raw firmware, private offsets/disassembly, and regression corpus remain outside the public repository.
