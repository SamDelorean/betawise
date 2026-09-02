# A110 / index68 — DialogGetItemId — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
int DialogGetItemId(uint8_t index);
```

`DialogGetItemId` is the direct A-line syscall at index 68 (`A110`). The existing BetaWise declaration is ABI-correct.

## Confirmed behavior

Firmware validates the explicit 1-based item index before reading the per-item ID array:

```c
if (index < 1 || index > item_count)
    return 0;
return item_ids[index - 1];
```

The first argument occupies one 32-bit ABI slot; the handler consumes its low byte. Valid items return the full 32-bit ID previously supplied to `DialogAddItem`. Invalid indices return zero. The function does not alter dialog state.

## Dialog getter relationship

The surrounding Dialog API now has a mechanically confirmed distinction:

- `DialogGetChoice()` returns the current 1-based selection index.
- `DialogGetChoiceId()` directly looks up the ID for the current selection and assumes that selection is valid.
- `DialogGetItemId(index)` performs the same ID-array lookup for an explicit index, but first validates `1 <= index <= item_count`.

For a valid dialog state:

```c
DialogGetChoiceId() == DialogGetItemId(DialogGetChoice())
```

## Source correlation

Historical DebugTool code iterates choices beginning at 1 and tests `DialogGetItemId(choice) != 0`, matching the recovered 1-based bounds check and zero sentinel. `DialogAddItem` independently confirms that IDs are stored as 32-bit per-item values in the same array read by A110.

## Generation comparison

The handler is 0x2E bytes in AS3000 2005, NEO 2005 and NEO 2013. Only the relocated `item_count` and `item_ids` addresses differ; after normalizing those operands, all three implementations are byte-identical.

Direct internal firmware callers: none in AS3000 2005, none in NEO 2005, and one in NEO 2013. The negative searches are retained because this API is also used by SmartApplet-side code and does not require an internal System caller.

## Regression status

Static regression: **EXECUTED — 89/89 PASS**.

It verifies canonical hashes, the vector neighborhood, exact handler bounds and grammar, ABI byte offset, both range checks, `item_count`/`item_ids` correlations, caller/reference counts and normalized three-generation equivalence.

Dynamic DialogProbe regression: **SPECIFIED / NOT EXECUTED**. Minimum cases are index 0 -> 0, valid indices -> exact 32-bit IDs, index `item_count + 1` -> 0, plus the getter identity above.

## Confidence

- **CONFIRMED:** identity, ABI signature, 1-based indexing, lower/upper validation, zero invalid sentinel, 32-bit ID return, no side effects, producer/consumer correlations and three-generation equivalence.
- **UNKNOWN:** application-specific meaning of an intentionally assigned ID value of zero; callers may use their own protocol because zero is also the invalid-index sentinel.

Raw firmware, private disassembly, ROM offsets and audit corpus remain outside the public repository.
