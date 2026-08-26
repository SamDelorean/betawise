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

## Stage 1 — minimal lifecycle

Goal: validate the dialog family without relying on `A10C`.

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
item_id = DialogGetItemId(choice);
```

Record independently for emulator, AS3000 and NEO:

- visual text and marker placement;
- initial selected item;
- arrow navigation;
- Enter return key;
- Escape return key;
- returned choice index;
- `DialogGetItemId(choice)`;
- `DialogAddItem` return values.

## Stage 2 — A10C isolation

Only after Stage 1 is understood, call `DialogGetChoiceId()` and compare its value with both:

- `DialogGetChoice()`;
- `DialogGetItemId(DialogGetChoice())`.

This isolates A10C rather than allowing a failure there to contaminate the basic dialog test.

## Stage 3 — shortcut-key behavior

Keep all other arguments fixed. Use three unique shortcuts and test:

- whether shortcuts activate/select an item;
- whether a shortcut exits `DialogRun` or only moves the choice;
- whether shortcut letters are rendered automatically;
- behavior with modifiers/Caps Lock;
- whether the full label or an omitted-leading-letter convention is expected.

## Stage 4 — marker behavior

Keep IDs, shortcuts and file sizes fixed. Compare markers:

- `' '`;
- `0xD7`;
- `0x10`.

Treat marker bytes first as literal glyphs because that behavior is confirmed in the historical dialog implementation. Record any later OS3K-specific behavior separately.

## Stage 5 — file_size behavior

Do not vary this parameter until the earlier stages are stable. Start from `(size_t)-1`, then use values selected from actual official callers if a non-`-1` case is identified in firmware/app code. Avoid arbitrary values until a plausible semantic hypothesis exists.

## Stage 6 — capacity and error behavior

Only after normal operation is stable, characterize:

- item capacity;
- duplicate shortcuts/IDs;
- zero-length text;
- width clipping;
- invalid initial choices;
- return values from `DialogAddItem` and `DialogAddExitKey`.

The historical implementation had a 25-item limit and returned `-1` when full. This is a historical baseline, not yet a guaranteed later-OS3K contract.

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

For `A0F4`, a useful trace should decode all six arguments. For `A104`, `A108`, `A10C`, and `A110`, record the returned value and correlate it with the current dialog state.
