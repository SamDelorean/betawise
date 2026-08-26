# ABI research status

This file separates usable SDK interfaces from active reverse-engineering hypotheses.

## Confirmed / SDK-ready

- `A138` — `ShowBatteryPercentage(uint8_t time_seconds)`
  - `0`: interactive battery-capacity screen, waits for a key.
  - `>0`: timed battery indicator in seconds.
  - Contract observed in AS3000 and NEO firmware.

## Dialog family A0F0–A110

The dialog subsystem is now tracked function by function in `docs/DIALOG_API.md`.

### Strongest members

- `A0F0` — `DialogInit`
  - Four-argument call shape established by existing use and historical lineage.
- `A0F4` — `DialogAddItem`
  - Official OS3K machine code confirms six 32-bit stack arguments.
  - Historical predecessor confirms `text`, `text_len`, literal prefix `marker`, insertion order, 1-based selection model, historical `0/-1` return behavior and historical 25-item capacity.
  - Later `id`, `shortcut_key`, and `file_size` are OS3K extensions.
- `A0FC` — `DialogSetChoice`
- `A100` — `DialogDraw`
- `A104` — `DialogRun`
- `A108` — `DialogGetChoice`
- `A110` — `DialogGetItemId`

These functions have coherent historical/official/BetaWise usage, but later-OS3K edge cases and some exact return contracts still need execution validation.

### Still incomplete

- `A0F8` — `DialogAddExitKey`
  - Purpose is strong; exact return/error contract remains incomplete.
- `A10C` — `DialogGetChoiceId`
  - Exact contract remains unresolved and is deliberately kept below the confidence of `A110`.
- `DialogAddItem.shortcut_key`
  - Real fifth ABI argument; shortcut behavior is strongly suggested, but rendering/matching rules remain experimental.
- `DialogAddItem.file_size`
  - Real sixth ABI argument; official callers commonly use `(size_t)-1`, but semantic meaning is not yet established.

The baseline `DialogProbe` should remain unchanged until the emulator can execute it; follow-up probes should vary one unresolved property at a time.

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
5. create a minimal probe;
6. validate separately on AS3000 and NEO when practical;
7. only then promote the interface and documentation.
