# A190 / index100 — bounded file-position operation

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered ABI

```c
int32_t SYS_A190(uint16_t file_id, int32_t offset, uint8_t origin);
```

The original vendor symbol has not been recovered, so the neutral `SYS_A190` name is retained.

## Confirmed behavior

`A190` is part of the System 3 File API. It resolves `file_id` through the same runtime descriptor resolver used by the established File API, then interprets `origin` as follows:

- `0`: candidate position = `offset`;
- `1`: candidate position = current cursor + `offset`;
- `2`: candidate position = current file size + `offset`;
- any other value: return `-2` without changing the cursor.

For a valid origin, the candidate is clamped to the closed interval `[0, current_size]`; this service does not seek beyond EOF. The resulting value is stored as the descriptor cursor, the descriptor synchronization helper is invoked so registered live cursor mirrors observe the change, and the resulting position is returned in `D0.L`.

The 0/1/2 behavior is mechanically equivalent to set/current/end positioning. Those descriptive terms are used here only to explain the behavior; no original vendor constant names have been recovered.

## Correlation with the reconstructed File API

Existing File API reconstruction establishes the relevant descriptor fields and helper relationships:

- descriptor `+0x04` is the current file size;
- descriptor `+0x14` is the cursor;
- the descriptor synchronization helper propagates cursor and other registered state to live mirrors exposed through the file-info service.

An independently reconstructed System 3 caller invokes `A190` with three arguments equivalent to `(file_id, state_position, 0)`, confirming both the three-slot ABI and the absolute-positioning interpretation of origin zero.

## Error and boundary behavior

- an invalid `origin` returns `-2` before cursor mutation or synchronization;
- negative valid-origin results clamp to zero;
- values above current size clamp to EOF;
- descriptor-resolution failure bypasses cursor mutation. The exact complete set of resolver failure values is intentionally not renamed or expanded here.

This is therefore not a byte-for-byte clone of standard C `fseek`: normal success returns the resulting position rather than merely a success/failure status.

## Generation comparison

AS3000 2005, NEO 2005 and NEO 2013 have the same 0x8E-byte control-flow structure. Raw differences are limited to private-call displacements and a diagnostic/source-line literal; after normalizing those operands, the bodies are identical.

Exactly one direct absolute firmware caller was found in each canonical ROM, and all three callers build the same argument pattern described above.

## Regression status

Static regression: **EXECUTED — 73/73 PASS**.

It verifies canonical ROM hashes, vector index100, complete handler span and instruction grammar, argument-slot widths, all three origin paths, invalid-origin handling, lower/upper clamping, cursor storage, descriptor synchronization, return behavior, caller count, caller argument construction and normalized three-generation equivalence.

Dynamic/emulator regression: **SPECIFIED / NOT EXECUTED**. Useful future cases include all three valid origins, positive/negative offsets, both clamps, invalid origin, unresolved tokens, active-file and clipboard tokens, and live-mirror cursor verification.

## Confidence

- **CONFIRMED:** three-argument ABI, descriptor resolution, origin behavior, bounds, cursor update, live-mirror synchronization, return path, caller shape and three-generation equivalence.
- **STRONG INFERENCE:** this is the File API's seek/set-position family operation.
- **UNKNOWN:** original vendor symbol, original origin constant names, and the complete nominal taxonomy of resolver errors.

Raw firmware, private offsets and disassembly remain outside the public repository.
