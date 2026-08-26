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

The dialog family is documented in detail in [`DIALOG_API.md`](DIALOG_API.md). The literal marker field has a focused evidence note in [`DIALOG_MARKER.md`](DIALOG_MARKER.md).

| Trap | Name | Confidence summary |
| --- | --- | --- |
| `A0F0` | `DialogInit` | A — argument/state initialization confirmed in AS3000 and NEO firmware |
| `A0F4` | `DialogAddItem` | A for six-argument ABI, `marker`, `id`, `shortcut_key`, `file_size`, 64-item capacity and 0/-1 return in analyzed 2005 firmware |
| `A0F8` | `DialogAddExitKey` | A — 15-key capacity, 0/-1 return confirmed |
| `A0FC` | `DialogSetChoice` | A — writes current-choice byte directly |
| `A100` | `DialogDraw` | B/A- overall — marker/shortcut/file-size rendering paths known; geometry and visual edge cases still under execution validation |
| `A104` | `DialogRun` | B/A- overall — 16-bit return and shortcut/exit-key interaction confirmed; remaining navigation edge cases still under validation |
| `A108` | `DialogGetChoice` | A — returns current-choice byte |
| `A10C` | `DialogGetChoiceId` | A — returns current item's 32-bit caller ID |
| `A110` | `DialogGetItemId` | A — indexed caller-ID getter with bounds check |

Direct handler analysis establishes the key metadata identity:

```c
DialogGetChoiceId() == DialogGetItemId(DialogGetChoice())
```

for a valid current choice. `A10C` directly indexes the current-choice ID and does not bounds-check; `A110` checks `1 <= index <= item_count` and returns `0` otherwise.

The `marker` field is now directly characterized across AS3000 2005, NEO 2005 and NEO 2013 firmware. `A0F4` stores the raw third-argument byte without semantic validation; the drawing path reads it and passes it directly to `A010 / PutChar`. Official callers use blank and visible glyphs including `' '`, `'*'`, `'+'`, and `'x'`. The dialog subsystem does not interpret those glyphs as state flags.

`shortcut_key` is a validated one-byte per-item shortcut. `A0F4` accepts keys for which `TranslateKeyToChar` yields a non-zero character, plus `KEY_FILE_1` through `KEY_FILE_8`; other values are normalized to `KEY_NONE`. `DialogDraw` generates the shortcut label automatically using the `"[ ]"` or `"[F ]"` templates. `DialogRun` selects and redraws the matching item but does not exit unless that same key is also present in the exit-key list. High-byte modifier flags do not participate in the raw shortcut-byte comparison, and duplicate shortcuts resolve to the last matching item.

`file_size` is a per-item **character count** used by `DialogDraw` to append a file-size annotation. `(size_t)-1` suppresses the annotation, `0` renders `" (empty)"`, `1` renders `" (1 char)"`, and larger values are rendered as grouped decimal `" (N chars)"`.

## A25C special-key dispatcher

```c
void SYS_A25C(uint8_t mask, KeyMod_e key);
```

**Confidence: C.** `mask == 0x8` is used to process at least one class of special/system keys, including `KEY_APPLETS`. The meanings of mask bits `1`, `2`, and `4` are still unknown. Do not rename this syscall to a narrower menu-specific function until the dispatcher is fully characterized.

## Research rule

Unknown traps remain named `SYS_Axxx` until their contract is supported by evidence. A useful internal name is not, by itself, enough to promote a syscall to the documented SDK API.
