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
- Firmware implementations read the argument as one byte, multiply non-zero values by 100, and pass the result to `SleepCentiseconds` (A0D4).
- The same argument semantics have been observed in AS3000 and NEO System 3 firmware.

The historical AS3000 source contains an earlier `PowerShowBatteryPercentage` function, but its parameter represented a display row rather than the later OS3K duration argument. The historical and OS3K interfaces must not be conflated.

## Dialog API: A0F0–A110

Known functions:

```c
void DialogInit(bool single, uint8_t row_first, uint8_t row_last, uint8_t col);
int DialogAddItem(char* text, uint8_t text_len, char marker, int id,
                  Key_e shortcut_key, size_t file_size);
int DialogAddExitKey(Key_e key);
void DialogSetChoice(uint8_t index);
void DialogDraw(void);
short DialogRun(void);
char DialogGetChoice(void);
int DialogGetChoiceId(void);
int DialogGetItemId(uint8_t index);
```

Current evidence strongly supports ordered item indexing and a caller-provided `id` independent of that index. `shortcut_key` behaves as a per-item keyboard shortcut in existing applets, but its complete visual/behavioral contract is still being characterized. `file_size` remains incompletely understood. `DialogProbe` exists specifically to validate these details without changing several uncertain parameters at once.

## A25C special-key dispatcher

```c
void SYS_A25C(uint8_t mask, KeyMod_e key);
```

**Confidence: C.** `mask == 0x8` is used to process at least one class of special/system keys, including `KEY_APPLETS`. The meanings of mask bits `1`, `2`, and `4` are still unknown. Do not rename this syscall to a narrower menu-specific function until the dispatcher is fully characterized.

## Research rule

Unknown traps remain named `SYS_Axxx` until their contract is supported by evidence. A useful internal name is not, by itself, enough to promote a syscall to the documented SDK API.
