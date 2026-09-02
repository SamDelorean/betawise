# A0FC / index63 — DialogSetChoice — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
void DialogSetChoice(uint8_t index);
```

`DialogSetChoice` is the direct A-line syscall at index 63 (`A0FC`). BetaWise does not add a separate C wrapper for this function.

## Confirmed behavior

The canonical AS3000 2005, NEO 2005 and NEO 2013 firmware implementations have the same semantic body after normalizing the relocated state address:

1. consume the low byte of the first 32-bit ABI argument slot;
2. store it directly into the dialog `current_choice` byte;
3. return.

The handler has no comparison, range check, clamp, helper call, drawing action, or explicit public return value.

Therefore the raw contract is a literal byte setter. A valid dialog-choice index is a caller precondition rather than something enforced by `A0FC`.

## Choice numbering

Normal Dialog API use is 1-based. This is supported independently by:

- `DialogInit`, which initializes `current_choice` to 1;
- historical DebugTool use, where `DialogSetChoice(choice)` establishes the default selection before `DialogDraw`;
- the DialogProbe test model, which maps its first three inserted items to choices 1, 2 and 3.

This does **not** mean the raw setter rejects 0 or an out-of-range byte: firmware stores any 8-bit value verbatim. Behavior of later dialog operations after an invalid value is outside this syscall's contract.

## Setter/getter correlation

`DialogGetChoice` (`A108`, index66) reads the same state byte that `DialogSetChoice` writes. Together with the initialization performed by `DialogInit`, this confirms that the state is the dialog's current choice rather than an unrelated flag.

## Generation comparison

No semantic difference was found among AS3000 2005, NEO 2005 and NEO 2013. The only handler difference is relocation of the absolute `current_choice` state address.

Direct absolute firmware callers found during the private audit: 3 in AS3000 2005, 4 in NEO 2005 and 5 in NEO 2013.

## Regression status

Static regression: **EXECUTED — 51/51 PASS**.

It verifies canonical ROM hashes, the vector-table neighborhood, index63 resolution, full two-instruction handler grammar, ABI argument byte selection, setter/getter state correlation, caller counts and normalized three-generation equivalence.

Dynamic visual/keyboard regression: **SPECIFIED / NOT EXECUTED**. A minimal future probe is `DialogInit` + three items + `DialogSetChoice(2)` + `DialogDraw`, followed by verification that the second item is initially selected and `DialogGetChoice()` reports 2.

## Confidence

- **CONFIRMED:** identity, ABI signature, direct byte-store semantics, state target, lack of validation/helpers, setter/getter relationship, three-generation equivalence.
- **STRONG INFERENCE:** callers should supply a valid 1-based choice in normal Dialog API use.
- **UNKNOWN:** downstream behavior of `DialogDraw`/`DialogRun` when the state byte contains an invalid choice value.

Raw firmware, private disassembly, ROM offsets and audit corpus remain outside the public repository.
