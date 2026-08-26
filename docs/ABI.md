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
| `A0F0` | `DialogInit` | A/B — call shape and historical model strong |
| `A0F4` | `DialogAddItem` | A for six-argument OS3K call shape; partial parameter semantics |
| `A0F8` | `DialogAddExitKey` | B |
| `A0FC` | `DialogSetChoice` | B/A- |
| `A100` | `DialogDraw` | B/A- |
| `A104` | `DialogRun` | B/A- |
| `A108` | `DialogGetChoice` | B/A- |
| `A10C` | `DialogGetChoiceId` | C — exact contract unresolved |
| `A110` | `DialogGetItemId` | B — official/existing usage stronger than A10C |

The historical AS3000 dialog implementation establishes insertion-ordered, 1-based choices and literal marker rendering. Official OS3K SmartApplet machine code establishes the six-argument `DialogAddItem` ABI. The later `shortcut_key` and `file_size` fields are real ABI arguments, but complete semantics remain under investigation.

`DialogProbe` remains the executable baseline for emulator/hardware validation. In particular, `A10C` is not considered required for the minimal confirmed lifecycle until its exact behavior is established.

## A25C special-key dispatcher

```c
void SYS_A25C(uint8_t mask, KeyMod_e key);
```

**Confidence: C.** `mask == 0x8` is used to process at least one class of special/system keys, including `KEY_APPLETS`. The meanings of mask bits `1`, `2`, and `4` are still unknown. Do not rename this syscall to a narrower menu-specific function until the dispatcher is fully characterized.

## Research rule

Unknown traps remain named `SYS_Axxx` until their contract is supported by evidence. A useful internal name is not, by itself, enough to promote a syscall to the documented SDK API.
