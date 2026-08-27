# System 3 / OS3K Dialog API closure — A0F0–A110

This document consolidates the current reconstructed contract for the public
Dialog API used by BetaWise.  The goal is practical: a developer should be able
to build a menu, predict its layout/navigation, interpret the returned choice,
and know which edge cases remain unsafe or internal.

The normal public behavior described here is confidence **A** at ABI/mechanical
level from direct System 3 handler analysis, with continuity checked against the
historical AS3000 DialogModule where applicable.  Exact visual presentation and
several internal event codes remain regression/open-detail topics.

## API overview

```c
void DialogInit(bool single, uint8_t row_first, uint8_t row_last, uint8_t col);
int DialogAddItem(char *text, uint8_t text_len, char marker,
                  int id, Key_e shortcut_key, size_t file_size);
int DialogAddExitKey(Key_e key);
void DialogSetChoice(uint8_t index);
void DialogDraw(void);
short DialogRun(void);
char DialogGetChoice(void);
int DialogGetChoiceId(void);
int DialogGetItemId(uint8_t index);
```

The normal choice/index model is **1-based**.

## 1. `DialogInit` / A0F0

`DialogInit` resets the shared dialog state:

- item count -> 0;
- exit-key count -> 0;
- current choice -> 1;
- stores `row_first`, `row_last`, and `col`;
- initializes the layout column state from `single`.

`single` is not merely a generic boolean display option:

- `single != 0` fixes `columns = 1`;
- `single == 0` leaves `columns = 0`, telling `DialogDraw` to calculate how
  many columns fit from the item width and the `col` character budget.

`row_first..row_last` is an inclusive vertical range.

## 2. `DialogAddItem` / A0F4

```c
int DialogAddItem(char *text, uint8_t text_len, char marker,
                  int id, Key_e shortcut_key, size_t file_size);
```

System 3 stores parallel metadata for each item.  The analyzed modern capacity
is **64 items**; insertion returns `0`, while a full table returns `-1`.

This differs from the much older AS3000 DialogModule, which used a smaller
25-item design.  Historical limits are therefore not projected into OS3K.

### `text` and `text_len`

The text pointer and explicit length provide the item label used by the dialog
renderer.  The length contributes to rendered width/layout calculations.

### `marker`

`marker` is a literal one-byte glyph supplied by the caller.  System 3 stores
the low byte and `DialogDraw` sends the same byte directly to `PutChar`.

It has **no universal enum semantics** in the dialog subsystem.  Official code
uses values including:

- space (`' '`), the usual visually empty marker;
- `'*'`;
- `'+'`;
- `'x'`.

The meaning of those symbols belongs to the application.  Do not create global
`DIALOG_MARKER_*` meanings that the firmware itself does not define.

### `id`

`id` is caller-defined 32-bit metadata.  It is stored separately from the
1-based item index and is returned by `DialogGetChoiceId` / `DialogGetItemId`.
It allows the displayed ordering to be separated from application-level IDs.

### `shortcut_key`

The shortcut is stored as a byte-sized `Key_e` after validation.

Validation/rendering behavior:

1. System 3 tries `TranslateKeyToChar`.
2. If the key translates to a visible character, the dialog subsystem can
   generate a label equivalent to `[c]`.
3. If no character is produced, `KEY_FILE_1` through `KEY_FILE_8` are handled
   specially and render as `[F1]` through `[F8]`.
4. Other unsupported/non-renderable values are normalized to `KEY_NONE`
   (`0xFF`).

The caller should **not** manually include `[c]` or `[F1]` in `text`; the dialog
subsystem generates shortcut decoration itself.

During `DialogRun`, shortcut matching compares only the stored shortcut byte
with the low byte of the current key/event.  High-byte modifier flags such as
Ctrl/Cmd/Alt/Shift do not participate in that comparison.

Duplicate shortcuts are not rejected.  The scan continues through all items,
so if several items use the same shortcut the **last matching item remains
selected** after the scan.

### `file_size`

`file_size` is display metadata representing a **character count**, not a file
I/O length that controls storage.

Special rendering:

| Value | Rendered suffix |
| ---: | --- |
| `(size_t)-1` | no size suffix |
| `0` | ` (empty)` |
| `1` | ` (1 char)` |
| `>1` | ` (N chars)` with thousands grouped by commas |

Examples include ` (1,000 chars)` and ` (1,234,567 chars)`.

The field is consumed by the drawing/formatting path and was not found in
selection, navigation, or file I/O logic.  `(size_t)-1` is therefore the
explicit sentinel for “do not display a size”.

## 3. `DialogAddExitKey` / A0F8

```c
int DialogAddExitKey(Key_e key);
```

The modern table accepts up to **15 exit keys**.

- success -> `0`;
- table full -> `-1`.

Exit-key processing order matters and is described under `DialogRun`.

## 4. `DialogSetChoice` / A0FC

```c
void DialogSetChoice(uint8_t index);
```

The handler writes the low byte directly into `current_choice`.  It performs
**no range validation itself**.

Normal application code should therefore set a valid 1-based choice before
calling functions that assume valid state.  Deliberately invalid choices are an
emulator-first edge case, not a normal hardware probe.

## 5. `DialogDraw` / A100

If there are no items, `DialogDraw` returns without drawing the menu.

### Width and columns

The renderer determines the maximum rendered item length and calculates:

```text
item_width = max_rendered_item_length + 3
```

If the dialog already has a non-zero `columns` value (for example because
`single != 0` produced one column), that value is retained.  Otherwise:

```text
columns = col / item_width
```

Items are arranged **row-major**: choices `1..columns` are the first logical
row, the next `columns` choices are the second, and so on.

### Visible window

```text
visible_rows = row_last - row_first + 1
last_visible = min(first_visible + visible_rows * columns - 1,
                   item_count)
```

`first_visible` and `current_choice` use 1-based item numbering.

### Selected cursor position

For a valid visible choice:

```text
cursor_col = 1 + ((current_choice - 1) % columns) * item_width
cursor_row = row_first + (current_choice - first_visible) / columns
```

The renderer normally hides the cursor while updating the display and restores
it at the selected cell after rendering.

### Scrolling

The display helper keeps `current_choice` inside the visible window.  Ordinary
vertical scrolling changes `first_visible` by exactly `columns`, i.e. one
logical row of items at a time.

The exact visual appearance of selection/markers is a regression target; the
layout relationships above are part of the closed mechanical contract.

## 6. `DialogRun` / A104

```c
short DialogRun(void);
```

The routine returns a 16-bit key/event value on normal exit.  It manages both
navigation and the generic shortcut/exit-key tables.

### Built-in navigation

The public navigation keys explicitly handled by the modern handlers are:

- `KEY_HOME`;
- `KEY_END`;
- `KEY_RIGHT`;
- `KEY_LEFT`;
- `KEY_UP`;
- `KEY_DOWN`.

Navigation does **not wrap**.

#### Home

If items exist and choice is not already 1:

```text
current_choice = 1
redraw dialog
```

#### End

If items exist and choice is not already the last item:

```text
current_choice = item_count
redraw dialog
```

#### Right

Move by `+1` only when:

- the selection is not already in the last column of its logical row; and
- the next item exists.

It does not wrap into the next row.

#### Left

Move by `-1` only when the selection is not in the first column.  No wrap to the
previous row occurs.  A single-column dialog naturally does not move left.

#### Up

If `current_choice > columns`, subtract `columns`.  Otherwise remain in place.

#### Down

Add `columns` only when an item exists directly below:

```text
current_choice + columns <= item_count
```

Thus a selection in an incomplete final row does not move down into a missing
cell.

### Ordering: navigation, shortcuts, exit keys

The six built-in navigation keys are dispatched **before** the generic shortcut
and exit-key scans.  Registering a navigation key as an exit key does not turn
its normal navigation path into an ordinary exit.

For other keys the order is:

1. scan item shortcuts;
2. if a shortcut matches, update `current_choice` and redraw;
3. scan registered exit keys;
4. if the same key is also an exit key, return after the selection/redraw.

This ordering explains why a key may act as both shortcut and exit: the
selection occurs first, then `DialogRun` returns.

### Internal event codes 0x64–0x67

The handler contains special paths for event bytes `0x64`, `0x65`, `0x66`, and
`0x67`.  They are used by internal System 3 paths but their semantic names are
not yet closed.

They must remain **unnamed** and must not be added to `Key_e` merely because the
numeric cases are visible.  Deliberate exploration belongs in the emulator
first.

## 7. Choice and ID getters

### `DialogGetChoice` / A108

```c
char DialogGetChoice(void);
```

Returns the current 1-based choice byte directly.

### `DialogGetChoiceId` / A10C

```c
int DialogGetChoiceId(void);
```

Equivalent to:

```text
return item_ids[current_choice - 1]
```

The handler has **no explicit bounds check**.  It assumes the dialog state has a
valid current choice.

### `DialogGetItemId` / A110

```c
int DialogGetItemId(uint8_t index);
```

This getter does validate the 1-based index:

- `1..item_count` -> `item_ids[index - 1]`;
- otherwise -> `0`.

For valid dialog state the following is a useful regression invariant:

```c
DialogGetChoiceId() == DialogGetItemId(DialogGetChoice())
```

## 8. Recommended usage

A conventional menu sequence is:

```c
DialogInit(false, first_row, last_row, width_chars);
DialogAddExitKey(KEY_ENTER);
DialogAddExitKey(KEY_ESC);

DialogAddItem("ALPHA", 5, ' ', 100, KEY_A, (size_t)-1);
DialogAddItem("BETA",  4, ' ', 200, KEY_B, (size_t)-1);
DialogAddItem("GAMMA", 5, ' ', 300, KEY_G, (size_t)-1);

DialogSetChoice(1);
DialogDraw();
short key = DialogRun();

if (key != KEY_ESC) {
    int id = DialogGetChoiceId();
    /* use application-defined id */
}
```

The actual exit-key set depends on the application.  The important sequencing
rule is to initialize state, add items/exits, establish a valid initial choice,
draw, then run.

## 9. Historical continuity

The early AS3000 DialogModule already contained the conceptual ancestors of
menu initialization, item insertion, display, input, cursor movement and
selection.  Modern OS3K retained that model but expanded it:

- item capacity increased from the older 25-item implementation to 64;
- caller-defined 32-bit `id` metadata was added;
- per-item shortcut metadata was added;
- display-only `file_size` metadata was added;
- modern layout/navigation logic is therefore not safely inferred solely from
  the 2000 source.

Historical source is used as genealogy; the modern firmware handlers determine
the current ABI.

## 10. Validation and regression status

The mechanical contract above comes from direct handler and caller analysis.
A DialogProbe baseline exists in the project history, but the presence of a
probe does not imply that every matrix case has already been executed.

Useful emulator regressions include:

- one-column and auto-column layout;
- Right/Left without wrap;
- Up/Down by `columns`;
- incomplete-last-row Down no-op;
- Home/End and viewport update;
- shortcut + exit-key ordering;
- duplicate shortcuts (last match wins);
- marker passthrough to `PutChar`;
- `file_size` rendering for `-1, 0, 1, 999, 1000, 1234567`;
- `DialogGetChoiceId == DialogGetItemId(DialogGetChoice())` for valid state.

Invalid `current_choice` and internal event values 0x64–0x67 should be explored
in the emulator before any deliberate hardware test.
