# Dialog API — A0F0 through A110

This document tracks the System 3 / OS3K dialog syscall family as one subsystem. Focused evidence notes live in [`DIALOG_MARKER.md`](DIALOG_MARKER.md) and [`DIALOG_NAVIGATION.md`](DIALOG_NAVIGATION.md).

## Current mapping

| Trap | BetaWise name | Confidence | Core contract |
| --- | --- | --- | --- |
| `A0F0` | `DialogInit` | A | initializes dialog state and layout mode |
| `A0F4` | `DialogAddItem` | A | six-argument item insertion with marker, ID, shortcut and character-count metadata |
| `A0F8` | `DialogAddExitKey` | A | appends one of up to 15 normal exit keys |
| `A0FC` | `DialogSetChoice` | A | directly writes the 1-based current choice |
| `A100` | `DialogDraw` | A for core layout | computes grid geometry, normalizes viewport and renders visible items |
| `A104` | `DialogRun` | A for public keyboard/navigation path | Home/End/grid arrows, shortcuts and normal exit-key processing |
| `A108` | `DialogGetChoice` | A | returns current 1-based choice |
| `A10C` | `DialogGetChoiceId` | A | returns caller ID for current choice |
| `A110` | `DialogGetItemId` | A | returns caller ID for an indexed item, with bounds checking |

The remaining dialog uncertainties are presentation edge cases and internal System 3 event codes, not the normal public grid/navigation contract.

## Historical lineage

The original AS3000 `DialogModule` predates the later SmartApplet ABI but already contains the same conceptual subsystem:

```text
DialogMenuInit
DialogMenuAddItem
DialogMenuAddExitICode
DialogMenuSetCursorItemNumber
DialogMenuDisplay
DialogMenuGetInput
DialogMenuGetCursorItemNumber
DialogMenuGetItem
```

The historical `DialogMenuAddItem` accepted only:

```c
int DialogMenuAddItem(char *text, uint8_t text_len, char marker);
```

Historical behavior includes insertion-order items, a literal marker glyph, a 1-based selection model, grid display/navigation and a 25-item capacity. Later OS3K keeps the underlying model but expands item capacity and adds `id`, `shortcut_key` and `file_size` metadata.

## A0F0 — DialogInit

```c
void DialogInit(bool single, uint8_t row_first, uint8_t row_last, uint8_t col);
```

**Confidence: A.** Direct firmware analysis establishes the initialization state:

```c
item_count = 0;
exit_key_count = 0;
first_visible = 1;
current_choice = 1;
columns = single ? 1 : 0;
```

`row_first` and `row_last` form an inclusive display-row range. `col` is the horizontal character budget used by the grid layout. A stored `columns == 0` means `DialogDraw` will auto-calculate the number of columns; `single != 0` therefore means fixed **single-column mode**.

## A0F4 — DialogAddItem

```c
int DialogAddItem(char* text,
                  uint8_t text_len,
                  char marker,
                  int id,
                  Key_e shortcut_key,
                  size_t file_size);
```

**Confidence: A for the normal six-argument contract in the analyzed firmware.**

### text / text_len

The label pointer and explicit label length descend directly from the historical interface. The later implementation stores a clipped presentation length appropriate to the dialog width.

### marker

`marker` is a raw one-byte literal glyph. `A0F4` stores the byte unchanged and the drawing path later passes it directly to `A010 / PutChar`. Official later-OS3K callers use at least `' '`, `'*'`, `'+'`, and `'x'`. The dialog subsystem does not assign application state semantics to the glyph. See [`DIALOG_MARKER.md`](DIALOG_MARKER.md).

### id

`id` is caller-provided 32-bit metadata, independent of the insertion-order choice index. It is stored in a parallel per-item array and is the value returned by `A10C` and `A110`.

### shortcut_key

`shortcut_key` is an optional one-byte per-item key. The firmware validates it through the same formatter used by `DialogDraw`:

- a key translated by `A164 / TranslateKeyToChar` generates `[c]`;
- `KEY_FILE_1` through `KEY_FILE_8` generate `[F1]` through `[F8]`;
- unsupported values normalize to `KEY_NONE` (`0xFF`).

`DialogRun` compares the stored shortcut with the raw low byte of the input key. A match selects the item and redraws but does not exit unless the same key is also configured as an exit key. High-byte Ctrl/Cmd/Alt/Shift/Caps-Lock flags do not participate in shortcut matching. Duplicate shortcuts are not rejected; because the item scan continues, the last matching item becomes the final selection.

### file_size

Despite the historical BetaWise name, this field is a **character count**, not a byte count. It is display metadata consumed by `DialogDraw`:

```text
(size_t)-1 -> no size annotation
0          -> " (empty)"
1          -> " (1 char)"
>1         -> " (N chars)"
```

Large values are grouped with thousands commas. The field does not control file I/O or navigation.

### capacity / return

In the November 2005 AS3000 and NEO System 3 implementations:

```text
maximum items = 64
success       = 0
full          = -1
```

The historical 25-item limit must not be projected onto later OS3K.

## A0F8 — DialogAddExitKey

```c
int DialogAddExitKey(Key_e key);
```

**Confidence: A.** The analyzed firmware stores up to **15** one-byte exit keys. It returns `0` when a key is appended and `-1` when the array is full.

Normal exit keys are tested after ordinary shortcut processing. Navigation keys are a special case described under `DialogRun`: Home/End/Left/Right/Up/Down are consumed by the navigation dispatcher before the normal exit-key scan.

The firmware also recognizes raw internal dialog event/exit bytes `0x64`–`0x67`. Their meanings are unresolved and they must not yet receive public SDK names.

## A0FC — DialogSetChoice

```c
void DialogSetChoice(uint8_t index);
```

**Confidence: A.** Writes the low byte directly into `current_choice`; no syscall-level range validation is performed. Normal usage is 1-based.

## A100 — DialogDraw

```c
void DialogDraw(void);
```

**Confidence: A for the core layout algorithm.** AS3000 2005, NEO 2005 and NEO 2013 implement the same logical grid model, and the historical AS3000 `DialogMenuDisplay` independently preserves the same structure.

When items exist, the firmware computes:

```c
item_width = max_rendered_item_length + 3;
```

If the dialog is not in fixed single-column mode, `columns` is calculated as:

```c
columns = col / item_width;
```

The grid is row-major. The inclusive visible row count and visible item capacity are:

```c
visible_rows = row_last - row_first + 1;
visible_capacity = visible_rows * columns;
```

The dialog tracks a 1-based `first_visible` item and normalizes that viewport so the selected item can be displayed. The final selected-cell coordinates follow the established layout:

```c
cursor_col = 1 + ((current_choice - 1) % columns) * item_width;
cursor_row = row_first + (current_choice - first_visible) / columns;
```

The visible-row renderer lays out cells left-to-right, using the already-characterized marker, generated shortcut label, text and optional character-count annotation.

The exact visual selection/border glyph remains an emulator/hardware presentation detail rather than a portable ABI promise. Full formulas and scrolling behavior are documented in [`DIALOG_NAVIGATION.md`](DIALOG_NAVIGATION.md).

## A104 — DialogRun

```c
short DialogRun(void);
```

**Confidence: A for the normal public keyboard/navigation path.** The firmware returns a 16-bit key/event value on normal exit and implements a non-wrapping row-major grid.

### Navigation

`KEY_HOME` selects item 1; `KEY_END` selects `item_count`. Both perform a full dialog refresh when the selection changes.

Right moves by `+1` only when there is an item immediately to the right in the same row:

```c
(current_choice % columns) != 0 && current_choice + 1 <= item_count
```

Left moves by `-1` only when not at the first column and when `columns != 1`:

```c
(current_choice % columns) != 1
```

Up and Down move vertically by exactly one grid row:

```c
UP:   current_choice -= columns;   // only if choice > columns
DOWN: current_choice += columns;   // only if choice + columns <= item_count
```

There is no horizontal or vertical wrapping. Down does nothing when the final row has no item directly beneath the current selection.

### Scrolling

For ordinary cursor movement, the viewport shifts by one item row when required:

```c
current_choice < first_visible -> first_visible -= columns
current_choice > last_visible  -> first_visible += columns
```

where:

```c
last_visible = min(first_visible + visible_rows * columns - 1, item_count);
```

Home, End and shortcut jumps may move farther; their redraw path re-establishes a viewport containing the new selection.

### Shortcut and exit order

Navigation is dispatched before the general shortcut/exit-key loop. Therefore adding `KEY_HOME`, `KEY_END`, `KEY_LEFT`, `KEY_RIGHT`, `KEY_UP`, or `KEY_DOWN` with `DialogAddExitKey` does not make the normal navigation key exit through that later scan.

For non-navigation keys the established order is:

```text
shortcut scan -> selection/redraw if matched -> exit-key scan -> return if matched
```

Thus a key that is both an item shortcut and an exit key first selects/redraws the item and then returns.

### Internal event codes

`DialogRun` contains special logic for raw bytes `0x64`, `0x65`, `0x66`, and `0x67`. These are internal System 3 dialog event/exit codes. Their semantic names remain unresolved and BetaWise intentionally does not expose guessed enum values for them.

See [`DIALOG_NAVIGATION.md`](DIALOG_NAVIGATION.md) for the detailed reconstruction and emulator regression matrix.

## A108 — DialogGetChoice

```c
char DialogGetChoice(void);
```

**Confidence: A.** Returns the current 1-based choice byte directly.

## A10C — DialogGetChoiceId

```c
int DialogGetChoiceId(void);
```

**Confidence: A.** Equivalent for a valid state to:

```c
return item_ids[current_choice - 1];
```

The handler performs no explicit bounds check.

## A110 — DialogGetItemId

```c
int DialogGetItemId(uint8_t index);
```

**Confidence: A.** Uses the same ID array as A10C but validates its 1-based index:

```c
if(index < 1 || index > item_count)
    return 0;
return item_ids[index - 1];
```

Therefore, for a valid current choice:

```c
DialogGetChoiceId() == DialogGetItemId(DialogGetChoice())
```

## Logical state relationship

The analyzed firmware generations preserve the same conceptual dialog state despite different internal RAM addresses:

```text
item_count
item_width
first_visible
columns
col
row_first
row_last
current_choice
exit_key_count
exit_key[]
marker[]
text_ptr[]
rendered_item_length[]
shortcut_key[]
item_id[]
item_file_size_chars[]
```

These addresses are firmware internals and must never become portable BetaWise constants.

## Emulator regression targets

The emulator should eventually trace `A0F0–A110` with enough state to assert the known contract. In particular it should expose `item_width`, `columns`, `first_visible`, `current_choice`, viewport boundaries and `DialogRun` key dispatch alongside the existing marker/shortcut/ID/file-size checks.

The public grid behavior is no longer a discovery question. Emulator execution should be used to validate the reconstructed contract and reveal platform-specific visual details.

## Remaining questions

The main unresolved dialog issues are now narrow:

1. semantic names and exact behavior of internal event/exit codes `0x64–0x67`;
2. exact visual selection/border rendering across AS3000 and NEO;
3. pathological invalid geometry or deliberately invalid `current_choice` states;
4. keyboard-layout edge cases for unusual translated shortcuts;
5. whether unrelated System 3 generations change capacities or internal presentation details.

The normal `DialogAddItem`, grid geometry, Home/End/arrow navigation, shortcut, exit-key, choice and ID contracts are sufficiently characterized to treat the family as an SDK-ready API with the caveats above.
