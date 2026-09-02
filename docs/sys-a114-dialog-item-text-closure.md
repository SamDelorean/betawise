# A114 / index69 — SYS_A114 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

No original AlphaSmart vendor name has been recovered for this trap. BetaWise upstream and `ioma8/neo-re` both retain the neutral symbol `SYS_A114`, so this closure deliberately keeps that name.

## Recovered contract

```c
char *SYS_A114(uint8_t index, uint8_t *effective_len_out);
```

`index` is the low byte of the first 32-bit ABI argument slot. `effective_len_out` is a required pointer supplied in the second 32-bit slot.

For a valid 1-based dialog item index, firmware:

1. subtracts one from `index`;
2. reads the corresponding effective/rendered-length byte maintained by the Dialog subsystem and stores it through `effective_len_out`;
3. indexes the parallel dialog text-pointer table with the same item index;
4. returns that stored 32-bit text pointer.

The routine does **not** validate `index`, and it performs the output byte store without a null-pointer check. Therefore `1 <= index <= item_count` and a valid output pointer are caller preconditions.

## Correlation with the Dialog subsystem

The two state arrays used by `A114` are the same arrays populated by `DialogAddItem`:

- the 64-entry text-pointer array;
- the 64-entry effective/rendered-length byte array.

The length byte is also consumed by `DialogDraw` when determining dialog item width and layout. This distinguishes it from the separate marker, shortcut, ID and file-size arrays maintained by the same subsystem.

The returned pointer is the original text pointer stored for the item, not an ID and not the length value itself.

## Generation comparison

The canonical AS3000 2005, NEO 2005 and NEO 2013 implementations have the same 0x2C-byte semantic body after normalizing exactly the two relocated array-base operands. No generation-specific semantic difference was found.

There are no direct absolute internal JSR callers to the handler in any of the three canonical System 3 ROMs; its only raw handler-address reference in each ROM is the A-line vector-table entry. An AlphaWordPlus 2005 scan likewise finds `A114` only inside its contiguous A-line stub table, not as an additional direct call site.

## Regression status

Static regression: **EXECUTED — 67/67 PASS**.

It verifies canonical ROM hashes, index69 vector resolution, full handler grammar through the first RTS, ABI argument positions, both dialog state arrays, unconditional output behavior, absence of range checks, absence of internal handler references, and normalized three-generation equivalence.

Dynamic/emulator regression: **SPECIFIED / NOT EXECUTED**. A safe future probe can add one known item and verify that `SYS_A114(1, &len)` returns the same text pointer while exposing the effective length stored by the dialog subsystem. Invalid-index probes should not be performed on physical hardware because the firmware routine does not bounds-check the index.

## Confidence

- **CONFIRMED:** index69/A114 identity, two-slot ABI, 1-based index arithmetic, required output pointer, returned text pointer, effective-length output byte, lack of range/null checks, DialogAddItem/DialogDraw state correlation, and three-generation equivalence.
- **STRONG INFERENCE:** the functional intent is a dialog-item text/length accessor.
- **UNKNOWN:** original AlphaSmart function name or private typedefs.

Raw firmware, offsets, complete disassembly and binary audit material remain private.