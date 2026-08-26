# ABI research status

This file separates usable SDK interfaces from active reverse-engineering hypotheses.

## Confirmed / SDK-ready

- `A138` — `ShowBatteryPercentage(uint8_t time_seconds)`
  - `0`: interactive battery-capacity screen, waits for a key.
  - `>0`: timed battery indicator in seconds.
  - Contract observed in AS3000 and NEO firmware.

## Dialog family A0F0–A110

The dialog subsystem is tracked function by function in `docs/DIALOG_API.md`.

### Confirmed members / properties

- `A0F0` — `DialogInit`
  - Four-argument handler state mapping confirmed in AS3000 and NEO 2005 firmware.
- `A0F4` — `DialogAddItem`
  - Six-argument OS3K ABI confirmed.
  - `id` is a caller-provided 32-bit value stored in a per-item array.
  - Later-OS3K analyzed capacity is 64 items.
  - returns `0` on insertion and `-1` when full.
  - historical predecessor confirms `text`, `text_len` and literal prefix `marker` lineage.
  - `shortcut_key` core semantics confirmed: accepted values are validated through the internal shortcut formatter, unsupported values normalize to `KEY_NONE`, labels are rendered automatically, and `DialogRun` matches the raw key byte.
  - shortcut match selects/redraws but does not exit unless the same key is also registered as an exit key; duplicate shortcuts resolve to the last matching item.
  - high-byte Ctrl/Cmd/Alt/Shift/Caps-Lock modifier bits do not participate in shortcut matching.
  - `file_size` core semantics confirmed as optional character-count display metadata with `-1` sentinel.
  - `file_size` is a per-item 32-bit character count consumed by `DialogDraw`; `(size_t)-1` suppresses its annotation.
  - formatter cases are confirmed as `0 -> " (empty)"`, `1 -> " (1 char)"`, and `>1 -> " (N chars)"` with thousands grouping.
- `A0F8` — `DialogAddExitKey`
  - capacity 15 keys; returns `0` on success and `-1` when full.
- `A0FC` — `DialogSetChoice`
  - writes the current-choice byte directly; no syscall-level range validation.
- `A108` — `DialogGetChoice`
  - returns the current-choice byte.
- `A10C` — `DialogGetChoiceId`
  - direct firmware analysis confirms it returns the 32-bit `id` for the current 1-based choice.
  - does not explicitly bounds-check.
- `A110` — `DialogGetItemId`
  - returns the same per-item `id` array by 1-based index.
  - returns `0` if the index is outside `1..item_count`.

For a valid current choice, firmware establishes:

```c
DialogGetChoiceId() == DialogGetItemId(DialogGetChoice())
```

### Strong but still execution-oriented

- `A100` — `DialogDraw`
  - role is coherent with historical `DialogMenuDisplay`; rendering differences remain to test.
- `A104` — `DialogRun`
  - firmware returns a 16-bit value.
  - shortcut selection and shortcut/exit-key interaction are confirmed directly from both 2005 handlers.
  - remaining work is primarily arrow/navigation edge cases and execution-level AS3000/NEO differences.

The baseline `DialogProbe` remains useful as an emulator/hardware regression test, but `A10C` no longer requires a separate discovery probe; it should now be checked against the confirmed relation above.

## Strong but still under validation

- Keyboard primitives `A088`–`A0B0`.
  - Existing BetaWise names and applet use are coherent, but individual contracts should eventually be documented function by function.

- Applet primitives `A238`–`A244`.

- `A2B8` — `CallSysInt` and currently known selectors.

## Probable / experimental

- `A0E8` — probable timer setup operation.
- `A0EC` — probable timer clear/stop operation.
- `A25C` — general special/system-key processing; mask `0x8` is known in existing use, other bits remain unknown.
- `A120`, `A128`, `A130` — battery-related helpers seen from A138 internals; not named yet.

## Named but incompletely surfaced in the header

The syscall table already contains several research-derived names that deserve dedicated reconstruction before being promoted to a documented API, including:

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
