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

## Stage 3 — shortcut-key behavior

Keep all other arguments fixed. Use three unique shortcuts and test:

- whether shortcuts activate/select an item;
- whether a shortcut exits `DialogRun` or only moves the choice;
- whether shortcut letters are rendered automatically;
- behavior with modifiers/Caps Lock;
- whether the full label or an omitted-leading-letter convention is expected.

## Stage 4 — marker behavior

Keep IDs, shortcuts and file sizes fixed. Compare markers from known real usage. Treat marker bytes first as literal glyphs because that behavior is confirmed in the historical dialog implementation. Record any later OS3K-specific behavior separately.

## Stage 5 — file_size behavior

Do not vary this parameter until the earlier stages are stable. Start from `(size_t)-1`, then use values selected from actual official callers if a non-`-1` case is identified in firmware/app code. Avoid arbitrary values until a plausible semantic hypothesis exists.

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

For `A0F4`, decode all six arguments. For `A108`, `A10C`, and `A110`, correlate the return with the current dialog state and enforce the confirmed metadata relation for valid choices.
