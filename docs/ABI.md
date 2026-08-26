# OS3K ABI

This document tracks the public-facing AlphaSmart System 3 / OS3K ABI exposed by BetaWise.

## Confidence levels

- **A — Confirmed:** backed by original source, direct firmware analysis, or hardware/emulator validation.
- **B — Strong:** coherent name and usage across multiple applets or reverse-engineering sources.
- **C — Probable:** reasonable inference that still needs validation.
- **D — Unknown:** only the A-line trap is known.

## A-line calling convention

BetaWise emits OS3K calls using the relation `0xA000 + 4 * index`. Names such as `SYS_A25C` therefore describe the A-line opcode, not a ROM address.

## Confirmed entry: A138 / ShowBatteryPercentage

```c
void ShowBatteryPercentage(uint8_t time_seconds);
```

**Confidence: A.**

- `time_seconds == 0`: displays the full battery-capacity screen and waits for a key before returning.
- `time_seconds > 0`: displays the battery indicator for the requested number of seconds.
- Firmware reads the argument as one byte, multiplies non-zero values by 100, and passes the result to `SleepCentiseconds` (A0D4).
- The same argument semantics are present in AS3000 and NEO System 3 firmware.

The historical AS3000 `PowerShowBatteryPercentage` had an earlier row parameter. Historical lineage and later OS3K ABI must not be conflated.

## Dialog API: A0F0–A110

The dialog family is documented in [`DIALOG_API.md`](DIALOG_API.md), with focused notes in [`DIALOG_MARKER.md`](DIALOG_MARKER.md) and [`DIALOG_NAVIGATION.md`](DIALOG_NAVIGATION.md).

| Trap | Name | Confidence summary |
| --- | --- | --- |
| `A0F0` | `DialogInit` | A — state initialization and single-column/auto-column mode confirmed |
| `A0F4` | `DialogAddItem` | A — six-argument ABI, marker, ID, shortcut, character-count metadata, capacity and return |
| `A0F8` | `DialogAddExitKey` | A — 15-entry normal exit-key array, 0/-1 return |
| `A0FC` | `DialogSetChoice` | A — direct current-choice byte write |
| `A100` | `DialogDraw` | A for core row-major grid layout and viewport/cursor calculations |
| `A104` | `DialogRun` | A for Home/End/arrows, no-wrap grid navigation, shortcuts and normal exit-key ordering |
| `A108` | `DialogGetChoice` | A — current-choice byte |
| `A10C` | `DialogGetChoiceId` | A — current item's 32-bit caller ID, no explicit bounds check |
| `A110` | `DialogGetItemId` | A — indexed caller-ID getter with bounds check |

### DialogAddItem metadata

`marker` is a literal byte stored unchanged and later passed to `A010 / PutChar`. Official code uses blank and visible glyphs including `' '`, `'*'`, `'+'`, and `'x'`; the dialog subsystem does not interpret them as state flags.

`id` is a caller-supplied 32-bit value independent of insertion-order choice. For valid state:

```c
DialogGetChoiceId() == DialogGetItemId(DialogGetChoice())
```

`shortcut_key` is a validated one-byte shortcut. Translatable keys render `[c]`; File 1–8 render `[F1]`–`[F8]`; unsupported values become `KEY_NONE`. A shortcut selects/redraws but exits only if the same ordinary key is also registered as an exit key. Duplicate shortcuts resolve to the last matching item.

`file_size` is a per-item **character count** used only for presentation: `(size_t)-1` suppresses the suffix, `0` shows `" (empty)"`, `1` shows `" (1 char)"`, and larger values render grouped decimal `" (N chars)"`.

### Dialog geometry

`DialogInit(single, row_first, row_last, col)` initializes `columns` as:

```c
columns = single ? 1 : 0;
```

A zero column count means auto-layout. `DialogDraw` computes:

```c
item_width = max_rendered_item_length + 3;
columns = col / item_width;   // when auto-layout is active
visible_rows = row_last - row_first + 1;
```

Items are arranged row-major. The selected-cell coordinates are equivalent to:

```c
cursor_col = 1 + ((current_choice - 1) % columns) * item_width;
cursor_row = row_first + (current_choice - first_visible) / columns;
```

The viewport advances or retreats by exactly one grid row (`columns` items) when ordinary vertical movement crosses the visible boundary.

### DialogRun navigation

The normal public key path is confirmed across AS3000 2005, NEO 2005 and NEO 2013 firmware:

```text
HOME  -> choice 1
END   -> choice item_count
RIGHT -> +1 only within the same row; no wrap
LEFT  -> -1 only within the same row; no wrap
UP    -> -columns when an item exists above
DOWN  -> +columns when an item exists below
```

In an incomplete final row, Down does nothing when there is no item directly below. Navigation keys are dispatched before the general shortcut/exit-key scan, so adding Home/End/Left/Right/Up/Down as normal exit keys does not make their ordinary navigation path exit `DialogRun`.

For non-navigation keys the order is shortcut selection/redraw first, then exit-key matching. Normal return preserves the 16-bit key/event value.

### Internal dialog codes

`DialogRun` contains explicit internal handling for raw bytes `0x64`, `0x65`, `0x66`, and `0x67`. Their semantic names are not yet established. They remain internal/unresolved and must not be added to the public `Key_e` enum based on guesswork.

## A25C special-key dispatcher

```c
void SYS_A25C(uint8_t mask, KeyMod_e key);
```

**Confidence: C.** `mask == 0x8` processes at least one class of special/system keys, including `KEY_APPLETS`. The meanings of mask bits `1`, `2`, and `4` remain unknown. Keep the raw syscall name until the dispatcher is fully characterized.

## Research rule

Unknown traps remain named `SYS_Axxx` until their contract is supported by evidence. A useful internal name is not, by itself, enough to promote a syscall to the documented SDK API.
