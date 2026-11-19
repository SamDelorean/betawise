# DialogProbe staged validation plan

The existing `DialogProbe.c` is the baseline and should remain stable until it can be executed in the emulator. Follow-up experiments should be additive and change one dimension at a time.

## Stage 0 — build/ABI inspection

Available or expected build artifacts:

- source `.c`;
- preprocessed `.i` when built with saved temporaries;
- compiler assembly `.s`;
- object `.o` locally;
- final `.OS3KApp`.

Verify the six stack arguments for `A0F4` and the expected A-line stubs.

## Stage 1 — normal lifecycle regression

Baseline sequence:

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

For valid state the emulator must satisfy:

```c
choice_id == item_id
```

Record visual output, initial selection, navigation, exit return, choice, ID getters and insertion return values independently for emulator, AS3000 and NEO.

## Stage 2 — bounds/capacity regression

Confirmed in the November 2005 AS3000 and NEO handlers:

- `DialogAddItem`: maximum 64 items; `0` success, `-1` full;
- `DialogAddExitKey`: maximum 15 entries; `0` success, `-1` full;
- `DialogGetItemId(index)`: `0` outside `1..item_count`;
- `DialogGetChoiceId()`: no explicit bounds check.

Stress invalid choice state only in the emulator until its behavior is understood.

## Stage 3 — shortcut-key regression

Expected behavior:

- translated shortcut -> generated `[c]` label;
- File 1–8 -> `[F1]`–`[F8]`;
- unsupported shortcut -> `KEY_NONE`;
- shortcut match -> select and redraw;
- shortcut alone -> does not exit;
- same key as shortcut + normal exit key -> select/redraw, then return;
- high-byte Ctrl/Cmd/Alt/Shift/Caps-Lock modifiers do not alter the raw shortcut-byte match;
- duplicate shortcuts resolve to the last matching item.

The baseline ALPHA/BETA/GAMMA case already supplies three distinct shortcuts. Add file-key, shortcut+exit and duplicate-shortcut cases separately.

## Stage 4 — marker regression

The third `A0F4` argument is a literal byte forwarded to `PutChar` by the drawing path. Keep all other item metadata fixed and compare:

```text
' ' -> blank marker
'*' -> '*'
'+' -> '+'
'x' -> 'x'
```

Changing the marker must not alter choice, ID, shortcut, exit behavior or file-size metadata.

## Stage 5 — file_size rendering regression

Keep all other item metadata fixed and vary only `file_size`:

```text
(size_t)-1 -> no annotation
0          -> " (empty)"
1          -> " (1 char)"
999        -> " (999 chars)"
1000       -> " (1,000 chars)"
1234567    -> " (1,234,567 chars)"
```

## Stage 6 — geometry/navigation regression

The grid contract is now known from direct firmware analysis and historical `DialogModule` comparison. This stage should assert it rather than discover it.

### Layout expectations

For a non-empty dialog:

```c
item_width = max_rendered_item_length + 3;
visible_rows = row_last - row_first + 1;
```

With `DialogInit(true, ...)`:

```c
columns == 1
```

With `DialogInit(false, ...)`, after `DialogDraw`:

```c
columns == col / item_width
```

The grid is row-major. The selected-cell coordinates should track:

```c
cursor_col = 1 + ((choice - 1) % columns) * item_width;
cursor_row = row_first + (choice - first_visible) / columns;
```

### Navigation matrix

Build a follow-up probe with enough equal-width items to produce multiple columns and more rows than the visible viewport. Then verify one key at a time:

```text
HOME  -> choice 1
END   -> choice item_count
RIGHT -> +1 only when an item exists to the right in the same row
LEFT  -> -1 only when an item exists to the left in the same row
UP    -> -columns when an item exists above
DOWN  -> +columns when an item exists directly below
```

Expected boundary rules:

- Right does not wrap to the next row;
- Left does not wrap to the previous row;
- Up on the first item row does nothing;
- Down on an incomplete final row does nothing if no item exists in the same column;
- single-column mode makes Left/Right ineffective for selection;
- ordinary vertical movement crossing the viewport changes `first_visible` by exactly `columns` items.

### Navigation versus exit keys

Navigation is dispatched before the normal shortcut/exit-key scan. Verify in the emulator that registering any of these through `DialogAddExitKey` does not turn its normal navigation path into an ordinary exit:

```text
KEY_HOME
KEY_END
KEY_LEFT
KEY_RIGHT
KEY_UP
KEY_DOWN
```

Do not use this as a physical-hardware stress test until the emulator reproduces the known handler behavior.

### Home/End and viewport

Home and End can jump farther than one visible row. Verify that their full-redraw path re-establishes a viewport containing the selected endpoint.

## Stage 7 — unresolved internal event codes

`DialogRun` contains special handling for raw dialog exit/event bytes:

```text
0x64  0x65  0x66  0x67
```

Their semantic names are not established. Do **not** add them to `Key_e`, guess labels, or probe them on physical hardware yet. Future emulator tracing should first determine their triggering conditions and return semantics.

## Stage 8 — presentation/pathological edge cases

After normal grid behavior is stable, characterize:

- exact selection/border glyph rendering;
- unusually small or otherwise invalid geometry;
- zero-length item text;
- deliberately invalid current choice in emulator only;
- keyboard-layout-specific shortcut characters;
- differences in visual rendering between AS3000 and NEO.

## Emulator trace requirements

For every dialog trap record:

```text
trap / symbolic name
caller PC
SP on entry
known arguments
return register/value
item_count
item_width
columns
row_first / row_last / col
first_visible
current_choice before/after
last_visible
```

For `A0F4`, decode all six arguments. For `A100`, expose the computed grid/viewport state. For `A104`, log the incoming key, dispatch branch, selection/viewport changes and final return. For `A108`, `A10C`, and `A110`, enforce the confirmed metadata relation for valid choices.
