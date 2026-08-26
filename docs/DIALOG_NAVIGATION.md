# Dialog geometry and navigation — A100 / A104

This note records the reconstructed layout and input contract for `A100 / DialogDraw` and `A104 / DialogRun`.

The core behavior below is supported by direct analysis of System 3 firmware for AS3000 (November 2005), NEO (November 2005), and NEO (July 2013), with the original March 2000 AS3000 `DialogModule` object providing independent historical lineage for the same grid model.

## Confidence

- `A100 / DialogDraw` core layout: **A — confirmed**.
- `A104 / DialogRun` Home/End/arrow navigation and normal exit-key interaction: **A — confirmed**.
- Exact visual appearance of the selection/border glyph and pathological invalid geometry: still execution-level regression work.
- Raw dialog event/exit codes `0x64` through `0x67`: observed internally but **semantics unresolved**. They must not be assigned public names yet.

## DialogInit geometry state

```c
void DialogInit(bool single, uint8_t row_first, uint8_t row_last, uint8_t col);
```

Direct handler analysis clarifies the `single` parameter. It controls the initial column mode:

```c
columns = single ? 1 : 0;
```

A stored `columns == 0` means that `DialogDraw` will auto-calculate the number of columns. A non-zero value supplied by the single-column path is retained.

The row range is inclusive:

```c
visible_rows = row_last - row_first + 1;
```

The `col` value is the horizontal character budget used by the dialog layout calculations. `DialogAddItem` already clips item presentation length against that budget.

The remaining initialization state includes:

```c
item_count = 0;
exit_key_count = 0;
first_visible = 1;
current_choice = 1;
```

These are logical firmware state fields; their physical RAM addresses are deliberately not part of the SDK.

## A100 / DialogDraw layout algorithm

`DialogDraw` returns immediately when there are no items.

For a non-empty dialog, the analyzed firmware scans the stored rendered length of every item and computes:

```c
item_width = max_rendered_item_length + 3;
```

If the dialog was initialized in single-column mode, `columns` is already 1 and remains 1. Otherwise the stored zero value triggers automatic calculation:

```c
columns = col / item_width;
```

The firmware uses its normal signed integer division helper. For normal positive dialog geometry the result forms the number of item cells that fit horizontally.

The resulting layout is a row-major grid. With `columns = C`, insertion-order choices are arranged conceptually as:

```text
1        2        ... C
C+1      C+2      ... 2C
2C+1     2C+2     ... 3C
...
```

### Visible window

The dialog tracks the 1-based index of the first visible item. The visible capacity is:

```c
visible_capacity = visible_rows * columns;
```

and the last visible item is equivalent to:

```c
last_visible = min(first_visible + visible_capacity - 1, item_count);
```

The drawing path selects/normalizes `first_visible`, clears or refreshes the configured row range as needed, then renders each visible row. Items within each row are drawn left-to-right in insertion order.

### Cursor placement

The final selected-cell column follows:

```c
cursor_col = 1 + ((current_choice - 1) % columns) * item_width;
```

The row follows the selected item's offset from the visible window:

```c
cursor_row = row_first + (current_choice - first_visible) / columns;
```

The draw helpers use hidden-cursor mode while refreshing and restore visible-cursor mode when positioning the selected cell.

### Cell presentation

The per-cell rendering path combines the already-characterized metadata:

```text
selection/border presentation
literal marker glyph
optional generated shortcut label
item text
optional file-size character-count annotation
```

The literal `marker`, generated shortcut label, and `file_size` suffix contracts are documented separately in `DIALOG_API.md` and `DIALOG_MARKER.md`.

The exact visual selection/border glyph is intentionally left as an emulator/hardware presentation regression rather than promoted to a portable ABI promise.

## Viewport scrolling

The move-cursor helper keeps `current_choice` inside the visible item window.

If the selection moves above the current window:

```c
if (current_choice < first_visible) {
    first_visible -= columns;
    redraw_visible_rows();
}
```

If it moves below the current window:

```c
if (current_choice > last_visible) {
    first_visible += columns;
    redraw_visible_rows();
}
```

Therefore ordinary vertical movement scrolls one **item row** at a time, i.e. in increments of `columns` items.

Home/End and shortcut jumps can move farther than one row. Their full-redraw paths re-establish a viewport containing the new selection.

## A104 / DialogRun public navigation contract

`DialogRun` obtains a key and dispatches the six normal navigation keys before the general shortcut/exit-key scan.

### Home

```c
if (item_count != 0 && current_choice != 1) {
    current_choice = 1;
    clear_dialog_rows();
    DialogDraw();
}
```

No wrapping occurs.

### End

```c
if (item_count != 0 && current_choice != item_count) {
    current_choice = item_count;
    clear_dialog_rows();
    DialogDraw();
}
```

No wrapping occurs.

### Right

Movement occurs only when an item exists immediately to the right in the same grid row:

```c
if (item_count != 0 &&
    (current_choice % columns) != 0 &&
    current_choice + 1 <= item_count) {
    ++current_choice;
    move_cursor_and_scroll_if_needed();
}
```

Right does **not** wrap to the beginning of the next row.

### Left

Movement occurs only when an item exists immediately to the left in the same grid row:

```c
if (item_count != 0 &&
    columns != 1 &&
    (current_choice % columns) != 1) {
    --current_choice;
    move_cursor_and_scroll_if_needed();
}
```

Left does **not** wrap to the end of the previous row. In single-column mode it has no selection effect.

### Up

```c
if (item_count != 0 && current_choice > columns) {
    current_choice -= columns;
    move_cursor_and_scroll_if_needed();
}
```

Up moves to the item in the same grid column on the previous row. It does not wrap.

### Down

```c
if (item_count != 0 && current_choice + columns <= item_count) {
    current_choice += columns;
    move_cursor_and_scroll_if_needed();
}
```

Down moves to the item in the same grid column on the next row. In an incomplete final row, it does nothing when there is no item directly below the current selection. It does not wrap.

These rules are structurally the same in the analyzed AS3000 2005, NEO 2005, and NEO 2013 handlers. The historical AS3000 `DialogMenuGetInput` independently exhibits the same grid-oriented Right/Left/Up/Down model.

## Navigation keys versus exit keys

A subtle but important part of the contract is dispatch order.

The six navigation keys are consumed by their navigation handlers **before** the normal per-item shortcut and exit-key scans. Consequently, adding one of these keys through `DialogAddExitKey` does not make the ordinary navigation key terminate `DialogRun` through the normal exit-key path:

```text
KEY_HOME
KEY_END
KEY_LEFT
KEY_RIGHT
KEY_UP
KEY_DOWN
```

should be treated as navigation controls, not normal configurable exit keys.

For other keys, the established order is:

```text
1. scan item shortcuts;
2. if a shortcut matches, select and redraw;
3. scan configured exit keys;
4. if an exit key matches, return the original 16-bit key value.
```

Thus the already-documented shortcut-plus-exit-key case selects/redraws first, then returns.

## Return value

```c
short DialogRun(void);
```

The firmware preserves the 16-bit key/event value on its normal return path. This remains compatible with existing BetaWise callers that treat the result as key-code data including possible modifier bits.

## Internal codes 0x64–0x67

At the beginning of `DialogRun`, the firmware performs special checks for raw exit/event bytes:

```text
0x64
0x65
0x66
0x67
```

These participate in internal System 3 event handling and can influence non-key return paths. Their exact semantic names have **not** been established.

Do not:

- add guessed `Key_e` names for these values;
- document them as ordinary keyboard keys;
- build hardware probes around them yet.

They should remain an explicit unresolved subproblem for later firmware/emulator tracing.

This also means the raw firmware accepts a one-byte dialog exit/event value more generally than the public `Key_e` type suggests. The current SDK signature remains unchanged until those internal codes are understood well enough to justify an API change.

## Emulator regression matrix

A future emulator should make the following state observable around `A100` and `A104`:

```text
item_count
item_width
columns
row_first / row_last / col
first_visible
current_choice
last_visible
key returned by GetKey
current_choice before/after dispatch
first_visible before/after dispatch
DialogRun return value
```

Useful deterministic regressions include:

```text
single=true      -> columns == 1
single=false     -> columns == col / item_width after draw
RIGHT at row end -> no wrap
LEFT at row start-> no wrap
UP on first row  -> no movement
DOWN with no item directly below -> no movement
HOME             -> choice 1
END              -> choice item_count
vertical move beyond viewport -> first_visible changes by columns
navigation key also in exit list -> navigation, not ordinary exit
ordinary shortcut + exit key -> select/redraw, then return
```

The internal `0x64`–`0x67` paths should be traced only after normal keyboard navigation is stable.

## Remaining work

The main unresolved items are now presentation or internal-event details rather than the public grid-navigation contract:

- exact semantics/names of internal dialog codes `0x64`–`0x67`;
- exact visual selection/border glyph behavior across AS3000 and NEO displays;
- pathological invalid geometry such as zero/underflowing row ranges;
- behavior after deliberately forcing an invalid `current_choice`;
- other firmware generations not yet inspected.
