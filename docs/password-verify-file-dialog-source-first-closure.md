# A1E0 / PasswordVerifyFileDialog — source-first closure

Status: mechanically and semantically closed from canonical firmware evidence after source-first correlation.

```c
uint8_t PasswordVerifyFileDialog(
    uint16_t file_id,
    uint8_t try_once,
    uint8_t allow_escape,
    uint8_t allow_special_navigation,
    uint8_t allow_file_switching,
    uint16_t *selected_file_out);
```

## Source-first correlation

The recovered early AlphaSmart PasswordModule supplies the historical anchor `Boolean PasswordVerifyFileDialog(UInt8 fileNumber, Boolean tryOnce)`: prompt for the file password, accept the master password or the file password, report failure and optionally retry, and allow switching among file slots. The historical signature is not assumed to be the modern ABI.

The existing reconstructed System 3 caller corpus expands this to six ABI slots. Representative official patterns are:

- AlphaWord Plus: `[file_id, 1, 1, 0, 0, NULL]`;
- AlphaWord Plus dynamic path: `[file_id, 0, 1, dynamic, 1, &selected]`;
- AlphaQuiz: `[FileGetCurrentFile(), 0, 1, 1, 0, NULL]`.

Those sources were treated as hypotheses and rechecked against the public handler in the three canonical ROMs.

## Canonical firmware confirmation

A1E0 is vector index 120. Re-extracted canonical entry points are:

- AS3000 2005: `0x004E2BA0`;
- NEO 2005: `0x005E54FC`;
- NEO 2013: `0x0043B8FE`.

A1E4 starts exactly `0x4EE` bytes later in every image, so A1E0 has a `0x4EE`-byte public handler 3/3. The first 32 and last 32 bytes are identical across all three generations; same-position byte identity is about 88.4–88.6% before relocation normalization.

The fresh primary-byte regression also confirms, at the same offsets in all three handlers:

- the special `0xFE` interaction value comparison;
- comparison against `KEY_APPLETS` (`0x29`);
- an exact-`1` flag comparison;
- a single terminal `RTS`;
- 43 absolute internal `JSR` instructions.

These invariants independently support the previously reconstructed navigation and flag-controlled interaction flow rather than a simple two-argument legacy wrapper.

## Confirmed contract

1. `file_id` is resolved through the common modern File API resolver.
2. The interaction operates on the selected descriptor's file password and also accepts the master password.
3. `try_once == 1` selects the one-failed-attempt path; other values retain retry behavior.
4. `allow_escape == 1` enables the Escape/cancel path.
5. `allow_special_navigation == 1` enables the special navigation path that includes the private `0xFE` value and `KEY_APPLETS`.
6. Non-zero `allow_file_switching` permits switching among eligible file descriptors in the current group.
7. `selected_file_out` is optional. When supplied, it is initialized to `0x00FF` and an accepted completion writes the canonical token of the descriptor that ultimately completed authorization.
8. Normal authorization returns low byte `1`; cancellation/failure returns low byte `0`.

Only the low byte of `D0` is treated as contractual.

## Layers

Official caller / BetaWise wrapper surface -> A-line trap A1E0 -> vector index 120 -> A1E0 handler -> common File API resolver -> password/UI helpers -> selected descriptor / optional selected-token output.

The historical two-argument function and the modern six-argument System 3 ABI are therefore conceptually continuous but not ABI-identical.

## Cross-generation comparison

AS3000 2005, NEO 2005 and NEO 2013 preserve the same handler span and control/data structure. Raw differences are dominated by relocated private call/data targets and metadata; the prologue, epilogue, navigation constants and decisive branch structure remain aligned. No semantic divergence was found in the revalidation.

## Refutation

- Refuted treating the early two-argument prototype as the modern ABI: the System 3 contract is six-slot and official callers exercise the added flags/output.
- Refuted reducing all flags to generic non-zero booleans: at least `try_once`, `allow_escape` and the special-navigation gate have exact-`1` control tests in the reconstructed flow; `allow_file_switching` is intentionally documented separately as non-zero.
- Refuted interpreting the return as a raw string-comparison result: callers and the handler use it as accepted/cancelled authorization state.
- No stronger vendor naming than `PasswordVerifyFileDialog` is invented; the name is retained because the historical conceptual continuity is direct while the ABI evolution is explicitly documented.

## Classification

**CONFIRMED:** six-argument modern ABI; File API resolution; master-or-file password authorization; one-attempt, Escape, special-navigation and file-switch paths; optional selected-token output; low-byte authorization return; 3/3 handler length and structural equivalence.

**INFERENCIA FUERTE:** continuity of the modern A1E0 service with the recovered historical `PasswordVerifyFileDialog` symbol.

**DESCONOCIDO:** public symbolic name of the private `0xFE` action and names of several internal UI/helpers.

## Regression

Static primary-byte regression was **EXECUTED: 41/41 PASS** against the three canonical ROMs. It checks SHA-256 identity, image bounds, A1E0/A1E4 boundaries, exact `0x4EE` span, canonical prologue/epilogue, terminal-RTS count, internal absolute-JSR count, aligned `0xFE`, `0x29` and exact-1 comparisons, and cross-ROM structural identity thresholds.

Dynamic regression is **SPECIFIED / NOT EXECUTED**. It should run emulator-first with disposable password/file state and cover correct file password, master password, incorrect password with retry and try-once, Escape disabled/enabled, special-navigation disabled/enabled, file switching, NULL/non-NULL `selected_file_out`, and final selected-token reporting.

Firmware bytes, complete disassembly and ROM-derived workpapers remain private.