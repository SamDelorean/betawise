# A194 / index101 — file-position query

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered ABI

```c
int32_t SYS_A194(uint16_t file_id);
```

The original vendor symbol has not been recovered, so the neutral `SYS_A194` name is retained.

## Confirmed behavior

`A194` resolves `file_id` through the same System 3 File API descriptor resolver used by adjacent file services. If resolution succeeds, it returns the descriptor cursor value. Existing File API reconstruction identifies that cursor as the 32-bit field mirrored by the file-info service.

If descriptor resolution fails, no descriptor field is dereferenced or mutated; the function returns the resolver-provided auxiliary status/value instead.

The handler does not change the cursor.

## Caller correlation

Exactly one direct absolute firmware caller exists in each canonical ROM. All three equivalent callers invoke `A194` with `file_id == 0`, where File API reconstruction has already established token zero as the currently active descriptor. They then save the returned value into a state field.

The same private state path later supplies that position to `A190(..., position, 0)`. This provides independent caller-level confirmation that `A194` is the query side of the File API position pair.

## Generation comparison

AS3000 2005, NEO 2005 and NEO 2013 use the same 0x36-byte control-flow structure. Differences are limited to private-call displacements and a diagnostic/source-line literal; normalizing those operands produces the same body.

## Regression status

Static regression: **EXECUTED — 40/40 PASS**.

It verifies canonical ROM hashes, vector index101, handler span and grammar, file-id argument width, shared resolver use, cursor-field read, return path, negative mutation check, caller counts, active-token caller shape, caller storage of the result and normalized three-generation equivalence.

Dynamic/emulator regression: **SPECIFIED / NOT EXECUTED**. Future cases should cover normal file tokens, token zero/current active descriptor, clipboard token, unresolved tokens, and coherence with `A190` after position changes.

## Confidence

- **CONFIRMED:** one-argument ABI, descriptor resolution, cursor read on success, no cursor mutation, caller behavior and three-generation equivalence.
- **STRONG INFERENCE:** this is the File API's tell/get-position operation.
- **UNKNOWN:** original vendor symbol and the complete nominal resolver error taxonomy.

Raw firmware, private offsets and disassembly remain outside the public repository.
