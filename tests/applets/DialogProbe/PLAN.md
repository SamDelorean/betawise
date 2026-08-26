# DialogProbe staged validation plan

The existing `DialogProbe.c` is the baseline and should remain stable until it can be executed in the emulator. Follow-up experiments should be additive and change one uncertain dimension at a time.

## Stage 0 — build/ABI inspection

Already available for the baseline build:

- source `.c`;
- preprocessed `.i` when built with saved temporaries;
- compiler assembly `.s`;
- object `.o` locally;
- final `.OS3KApp`.

Use these artifacts to verify that the caller emits the expected six stack arguments for `A0F4` and the expected A-line stubs.

## Stage 1 — normal lifecycle regression

Sequence:

```c
DialogInit(0, 1, 4, 40);
DialogAddExitKey(KEY_ENTER);
DialogAddExitKey(KEY_ESC);
DialogAddItem("ALPHA", 5, ' ', 100, KEY_A, (size_t)-1);
DialogAddItem("BETA",  4, ' ', 200, KEY_B, (size_t)-1);
DialogAddItem("GAMMA", 5, ' ', 300, KEY_G, (size_t)-1);
DialogSetChoice(1);
DialogDraw();
run_key = DialogRun();
choice = DialogGetChoice();
choice_id = DialogGetChoiceId();
item_id = DialogGetItemId(choice);
```

Direct AS3000 and NEO firmware analysis now establishes the expected metadata relation:

```c
choice_id == item_id
```

for a valid current choice. This is no longer a speculative A10C experiment; it is an emulator/hardware regression expectation.

Record independently for emulator, AS3000 and NEO:

- visual text and marker placement;
- initial selected item;
- arrow navigation;
- Enter return key;
- Escape return key;
- returned choice index;
- `DialogGetChoiceId()`;
- `DialogGetItemId(choice)`;
- `DialogAddItem` return values.

## Stage 2 — bounds/capacity regression

Firmware analysis of the November 2005 AS3000 and NEO handlers establishes:

- `DialogAddItem`: maximum 64 items; `0` success, `-1` when full;
- `DialogAddExitKey`: maximum 15 keys; `0` success, `-1` when full;
- `DialogGetItemId(index)`: returns `0` outside `1..item_count`;
- `DialogGetChoiceId()`: no explicit bounds check.

These should be verified in the emulator before intentionally stressing physical hardware. Do not set an invalid current choice on hardware merely to test the unchecked A10C path.

## Stage 3 — shortcut-key regression

Firmware analysis has resolved the core shortcut contract, so this stage is now a regression test.

Expected behavior:

- `A0F4` validates the low-byte shortcut using the internal label helper.
- If `TranslateKeyToChar(shortcut_key)` returns a non-zero character, `DialogDraw` renders a localized `"[c]"` label automatically.
- `KEY_FILE_1` through `KEY_FILE_8` are special-cased and render `"[F1]"` through `"[F8]"`.
- Other non-translatable shortcut values are normalized to `KEY_NONE` (`0xFF`).
- Pressing a shortcut changes the current 1-based choice and redraws the dialog.
- A shortcut does not exit `DialogRun` unless the same key was also registered with `DialogAddExitKey`.
- Ctrl/Cmd/Alt/Shift/Caps-Lock high-byte modifier flags do not affect the raw shortcut-byte comparison.
- Duplicate shortcuts are not rejected; because the scan continues, the last matching item becomes the final selection.

The baseline ALPHA/BETA/GAMMA probe already supplies three distinct shortcut keys and can verify selection-without-exit. Add separate follow-up cases for a file key and for shortcut+exit-key behavior; keep each variation isolated. Modifier tests should compare the same raw key with and without one high-byte modifier while keeping all other dialog state identical.

## Stage 4 — marker regression

Firmware analysis has resolved the marker contract. The third argument is stored as a raw byte and `DialogDraw` passes it directly to `A010 / PutChar`. This behavior is present in AS3000 2005, NEO 2005 and NEO 2013 firmware. Official code uses visible markers including `'*'`, `'+'`, and `'x'` as well as the normal blank space.

Keep text, ID, shortcut and file size fixed and vary only the marker in separate runs:

- `' '` — blank marker position;
- `'*'` — literal asterisk;
- `'+'` — literal plus sign;
- `'x'` — literal lowercase x.

The emulator trace should show the `A0F4` third-argument byte and the same byte reaching `PutChar` during drawing. Changing the marker must not change choice, ID, shortcut behavior, exit behavior or file-size metadata.

Do not use arbitrary control bytes on physical hardware merely to discover behavior. The raw firmware forwards them to `PutChar`; any useful rendering of non-printable characters may depend on font/display details.

## Stage 5 — file_size rendering regression

Firmware analysis has resolved the semantics, so this stage is now a rendering regression rather than a discovery experiment. Keep text, marker, ID and shortcut fixed and vary only `file_size` in separate emulator runs.

Expected cases from the 2005 `DialogModule` formatter:

- `(size_t)-1` — no size annotation;
- `0` — `" (empty)"`;
- `1` — `" (1 char)"`;
- `999` — `" (999 chars)"`;
- `1000` — `" (1,000 chars)"`;
- `1234567` — `" (1,234,567 chars)"`.

Run these first in the emulator. The baseline `DialogProbe` should remain unchanged at `-1`; a follow-up probe should change one value at a time. Physical-hardware validation can follow once the emulator behavior is stable.

## Stage 6 — geometry/navigation edge cases

Only after normal operation is stable, characterize:

- width clipping;
- unusual row/column geometry;
- duplicate shortcuts/IDs;
- zero-length text;
- navigation at first/last item;
- exact `DialogRun` return behavior for each exit path.

## Emulator trace requirements

For every dialog trap, the emulator should be able to log:

```text
trap / symbolic name
caller PC
SP on entry
known arguments
return register/value
selected-choice state before and after
```

For `A0F4`, decode all six arguments. For marker regressions, correlate argument 3 with the `PutChar` call reached through `DialogDraw`. For `A108`, `A10C`, and `A110`, correlate the return with the current dialog state and enforce the confirmed metadata relation for valid choices.
