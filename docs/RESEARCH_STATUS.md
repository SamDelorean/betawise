# ABI research status

This file separates usable SDK interfaces from active reverse-engineering hypotheses.

## Confirmed / SDK-ready

- `A138` — `ShowBatteryPercentage(uint8_t time_seconds)`
  - `0`: interactive battery-capacity screen, waits for a key.
  - `>0`: timed battery indicator in seconds.
  - Contract observed directly in AS3000 and NEO firmware.

## Dialog family A0F0–A110

The dialog subsystem is tracked in `docs/DIALOG_API.md`, with focused evidence in `docs/DIALOG_MARKER.md` and `docs/DIALOG_NAVIGATION.md`.

### Confirmed members / properties

- `A0F0` — `DialogInit`
  - state initialization confirmed;
  - `single != 0` selects fixed one-column mode;
  - `single == 0` leaves column count at zero for `DialogDraw` auto-layout;
  - `row_first..row_last` is an inclusive row range.
- `A0F4` — `DialogAddItem`
  - six-argument OS3K ABI confirmed;
  - `marker` is a literal raw glyph byte passed to `PutChar` during drawing;
  - `id` is a caller-provided 32-bit value;
  - `shortcut_key` validation/rendering/selection behavior confirmed;
  - `file_size` is optional character-count presentation metadata with `-1` sentinel;
  - analyzed 2005 capacity is 64 items;
  - returns `0` on insertion and `-1` when full.
- `A0F8` — `DialogAddExitKey`
  - capacity 15 normal one-byte exit keys;
  - returns `0` on success and `-1` when full.
- `A0FC` — `DialogSetChoice`
  - direct current-choice byte write; no syscall-level range validation.
- `A100` — `DialogDraw`
  - core row-major geometry promoted to confirmed;
  - `item_width = max_rendered_item_length + 3`;
  - automatic `columns = col / item_width` when not fixed to one column;
  - inclusive `visible_rows = row_last - row_first + 1`;
  - viewport and selected-cell cursor positioning reconstructed across AS3000 2005, NEO 2005 and NEO 2013.
- `A104` — `DialogRun`
  - normal Home/End/Left/Right/Up/Down navigation promoted to confirmed;
  - no horizontal or vertical wrap;
  - Up/Down move by `columns`;
  - incomplete final-row Down is rejected when no item exists directly below;
  - viewport scrolls by one item row when ordinary movement crosses the visible window;
  - navigation keys are dispatched before the normal shortcut/exit-key scan;
  - ordinary shortcut selection occurs before ordinary exit-key matching;
  - normal return preserves a 16-bit key/event value.
- `A108` — `DialogGetChoice`
  - returns the current-choice byte.
- `A10C` — `DialogGetChoiceId`
  - returns the 32-bit `id` for the current 1-based choice;
  - no explicit bounds check.
- `A110` — `DialogGetItemId`
  - returns the same ID array by 1-based index;
  - returns `0` outside `1..item_count`.

For valid state:

```c
DialogGetChoiceId() == DialogGetItemId(DialogGetChoice())
```

### Dialog work still requiring execution or further RE

The normal public dialog contract is now mostly closed. Remaining work is narrow:

- exact visual selection/border rendering across AS3000 and NEO;
- semantic names and exact event behavior of raw internal dialog codes `0x64`–`0x67`;
- pathological invalid geometry/current-choice states;
- keyboard-layout edge cases for unusual shortcut translations;
- confirmation of capacities/presentation details in additional System 3 generations.

The baseline `DialogProbe` is now primarily a regression applet. Marker, shortcut, file-size, choice-ID and normal navigation stages should assert known behavior rather than discover it.

## Filesystem reconstruction — active

Detailed notes are in `docs/FILE_API.md`.

The current BetaWise names in the `A198`–`A1CC` range remain research-derived and are not yet public SDK contracts.

### Directly established so far

- `A1C8`:
  - two ABI argument slots are consumed;
  - the first argument is resolved through a common descriptor resolver and only its low 16 bits are used by that path;
  - the second argument is inspected as a byte-sized mode/flag;
  - the resolved descriptor is installed as the current/active object after `A1CC` clears the previous current descriptor;
  - `mode == 1` clears descriptor field `+0x14`;
  - a scalar/status-like value is returned in `D0`, but its exact contract remains unresolved.
- `A1CC`:
  - consumes no arguments;
  - clears the current/active descriptor global and returns.

These mechanics are structurally equivalent in the November 2005 AS3000 and NEO handlers.

### Naming status

- Legacy BetaWise `FileOpen` / `FileClose`: **B working names**. Direct firmware behavior materially supports an enter/open/select-current-context and release-current-context interpretation, but exact ownership and return semantics still need callers.
- Independent `neo-re` currently calls `A1C8` `query_object_metric` and `A1CC` `commit_editable_buffer`. That interpretation conflicts with the full direct handler behavior and is retained as comparative evidence rather than adopted.
- No public prototypes have been added to `os3k.h`.

Historical April 2000 `FileModule.c` confirms a per-file descriptor model (`max size`, `size`, `old size`, `cursor`, row-one position, read-only state, storage pointer) and current-file selection, but it contains no historical functions literally named `FileOpen` or `FileClose`. Later OS3K semantics must therefore be reconstructed independently.

## Strong but still under validation

- Keyboard primitives `A088`–`A0B0`.
  - Existing BetaWise names and applet use are coherent, but individual contracts should be documented function by function.
- Applet primitives `A238`–`A244`.
- `A2B8` — `CallSysInt` and currently known selectors.

## Probable / experimental

- `A0E8` — probable timer setup operation.
- `A0EC` — probable timer clear/stop operation.
- `A25C` — general special/system-key processing; mask `0x8` is known in existing use, other bits remain unknown.
- `A120`, `A128`, `A130` — battery-related helpers seen from A138 internals; not named yet.

## Named but incompletely surfaced in the header

The syscall table already contains research-derived names that deserve dedicated reconstruction before promotion to a documented API, including:

- `FileWriteBuffer`
- `FileReadBuffer`
- `FileSetFolder`
- `FileOpen`
- `FileClose`
- `ClipboardSet`
- `ClipboardGet`
- `ClipboardClear`

Historical source, firmware callers, emulator traces, and probes should be cross-correlated before prototypes are added.

## Research workflow

For each syscall:

1. inspect original AlphaSmart source when available;
2. inspect firmware/Ghidra symbols and callers;
3. inspect BetaWise and existing applet use;
4. inspect emulator traces and related hardware behavior;
5. create a minimal probe when execution can resolve remaining uncertainty;
6. validate separately on AS3000 and NEO when practical;
7. only then promote the interface and documentation.
