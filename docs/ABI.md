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

The dialog family is documented in detail in [`DIALOG_API.md`](DIALOG_API.md).

| Trap | Name | Confidence summary |
| --- | --- | --- |
| `A0F0` | `DialogInit` | A — argument/state initialization confirmed in AS3000 and NEO firmware |
| `A0F4` | `DialogAddItem` | A for six-argument ABI, `id`, 64-item capacity and 0/-1 return; partial for shortcut/file-size semantics |
| `A0F8` | `DialogAddExitKey` | A — 15-key capacity, 0/-1 return confirmed |
| `A0FC` | `DialogSetChoice` | A — writes current-choice byte directly |
| `A100` | `DialogDraw` | B/A- — function role strong, rendering details still under execution validation |
| `A104` | `DialogRun` | B/A- — 16-bit return confirmed, complete input contract still under validation |
| `A108` | `DialogGetChoice` | A — returns current-choice byte |
| `A10C` | `DialogGetChoiceId` | A — returns current item's 32-bit caller ID |
| `A110` | `DialogGetItemId` | A — indexed caller-ID getter with bounds check |

Direct handler analysis establishes the key metadata identity:

```c
DialogGetChoiceId() == DialogGetItemId(DialogGetChoice())
```

for a valid current choice. `A10C` directly indexes the current-choice ID and does not bounds-check; `A110` checks `1 <= index <= item_count` and returns `0` otherwise.

The later `shortcut_key` and `file_size` fields are real OS3K ABI arguments, but their complete semantics remain under investigation.

## A25C special-key dispatcher

```c
void SYS_A25C(uint8_t mask, KeyMod_e key);
```

**Confidence: C.** `mask == 0x8` is used to process at least one class of special/system keys, including `KEY_APPLETS`. The meanings of mask bits `1`, `2`, and `4` are still unknown. Do not rename this syscall to a narrower menu-specific function until the dispatcher is fully characterized.

## Research rule

Unknown traps remain named `SYS_Axxx` until their contract is supported by evidence. A useful internal name is not, by itself, enough to promote a syscall to the documented SDK API.
