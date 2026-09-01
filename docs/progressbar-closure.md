# A080 — `ProgressBar`

Status: mechanically closed from primary firmware evidence and revalidated source-first.

The public SDK spelling remains:

```c
void ProgressBar(uint8_t row, uint16_t value, uint16_t total);
```

`os3k/syscall.c` maps index 32 to `ProgressBar`. The firmware handlers consume the first stack slot by its low byte (`row`) and consume the complete 32-bit ABI slots containing `value` and `total`. This does **not** by itself justify widening the public historical `uint16_t` source prototype: normal calls zero-extend those values into the compiler's longword stack slots.

## Confirmed behavior

The routine draws a 40-column text progress indicator on `row` and preserves the incoming logical cursor row/column, but **not** its visibility mode.

It first obtains the current cursor position with A008/GetCursorPos.

Percentage calculation is:

```text
if total == 0:
    percentage = 0
else:
    percentage = low32(100 * value) / total   // unsigned
    if percentage > 100:
        percentage = 100
```

Thus `total == 0` is explicitly handled and does not reach the divider.

For a nonzero percentage, the fill-cell count is:

```text
floor(33 * percentage / 100)
```

so the 33-cell interior reaches full width only at 100%.

## Screen layout

The handler composes the row through existing OS3K display primitives:

- column 1: `[`;
- columns 2..34: up to 33 fill-character cells;
- column 35: `]`;
- column 36: not touched by the percentage-text block;
- columns 37..40: decimal percentage plus `%`.

The fill byte written by the firmware is character code `0x00`. This document deliberately does not assign a visual glyph name to that byte without independent source evidence.

When `percentage == 0`, the routine clears columns 1..40, writes the brackets and establishes an empty bar. For `percentage > 0`, it writes the filled cells but does **not** clear the remaining interior cells. Therefore the visual behavior is incremental/stateful: a caller that previously displayed a larger percentage and then supplies a smaller nonzero value can leave stale filled cells. Initializing with a zero call, and otherwise using nondecreasing progress, matches the mechanics of the handler.

The percentage text is always refreshed: columns 37..40 are cleared, the numeric percentage is converted in radix 10 through A07C, then emitted with A014/PutStringRaw followed by `%`.

## Cursor side effect

The saved cursor row and column are restored before return, but A004/SetCursor is called with mode `0x0F` unconditionally. The function never reads the prior cursor mode.

Therefore `ProgressBar` restores cursor **position** but forces the cursor to the public SHOW mode on return. A caller entering with the cursor hidden must hide it again if that state matters.

## Internal helpers

A080 uses:

- A008 / GetCursorPos;
- A020 / ClearRowCols;
- A004 / SetCursor;
- A010 / PutChar;
- A014 / PutStringRaw;
- A07C / 32-bit radix conversion;
- one internal unsigned low-32 multiplication helper;
- the unsigned quotient helper already characterized with A07C.

The multiplication helper is byte-identical in all three canonical ROMs. It forms the low 32 bits of a 32-bit product using `MULU.W` partial products and returns that value in `D0`. No vendor/compiler name is assigned to this helper.

## Cross-generation verification

| Platform | Handler address | File offset | Size |
| --- | ---: | ---: | ---: |
| AlphaSmart 3000, 2005 | `0x004CC04C` | `0x0000C04C` | `0x130` |
| NEO, 2005 | `0x005CBBA4` | `0x0000BBA4` | `0x130` |
| NEO, 2013 | `0x0041C5C4` | `0x0000C5C4` | `0x130` |

All three handlers contain 102 decoded instructions with the terminal `RTS` at `+0x12E`. After neutralizing relocated absolute call targets, the bodies are identical; normalized SHA-256:

`4eccc46899c780172035cf08caa7e85478010512c06e2653d10b09b7eb3aa72f`

Firmware bytes and full disassembly remain private.

## Official caller evidence

Structural resolution of the AlphaWordPlus import-call form found **15 executable A080 callers** in AlphaWordPlus 2005 and **15** in AlphaWordPlus NEO 2012.

Representative callers pass a UI row such as 3 and a total of 100. This independently supports the documented three-argument use and the ordinary percentage/progress interpretation. Callers do not use `D0` as a function result, consistent with the historical `void` prototype and the absence of an explicit return-value construction in the handler.

## Adversarial cases

- `total == 0`: safe; rendered percentage is 0 rather than dividing by zero.
- `value > total`: calculated percentage is clamped to 100, subject to the public-width arithmetic below.
- The multiply is a low-32-bit product before division. Within the historical `uint16_t value` public domain, `100 * value` cannot overflow 32 bits. Raw callers that deliberately place wider values in the stack slot can encounter modulo-32-bit multiplication before the clamp.
- Decreasing from a previously larger nonzero percentage does not clear stale interior fill cells.
- Cursor visibility is not preserved; return mode is forced to SHOW (`0x0F`).

## Source-first reconciliation

The SDK sources independently provide the name and historical prototype: `DEFINE_SYSCALL(32, ProgressBar)` and `void ProgressBar(uint8_t row, uint16_t value, uint16_t total);`. The available OS3K documentation describes it as drawing a progress bar on the specified row. A directed search of the accessible early AlphaSmart source corpus did not recover an independent pre-OS3K implementation with stronger naming or semantic authority, so firmware remains the primary evidence for the behavior above.

## Regression status

A dedicated static regression was **executed and passed 93/93 checks**. It verifies canonical ROM hashes, A080 vectors and exact A084 boundaries, handler hashes and normalized identity, argument loads, helper targets/effects, percentage clamp, 33-cell scaling, A07C decimal conversion, forced SHOW mode, terminal `RTS`, 15+15 official executable callers, representative call arguments, and adversarial semantic cases.

Regression source SHA-256:

`91a5b66113d3e88a429fa888dd09e0774608a37cb824a24ee5bf87005d870930`

Regression output SHA-256:

`b4008b0c95fa13b6b03c6b4477041ed45edadba46ab4ba22416d184f5153215a`

The first harness run reported one failure solely because the expected multiplication-helper SHA was transcribed incorrectly. Correcting that test constant produced the final 93/93 result; no firmware interpretation changed.

A dynamic hardware/emulator probe has **not** been executed and remains additional validation rather than a prerequisite for this static/mechanical closure.

## Classification

- **CONFIRMED:** three-slot calling form; row low-byte consumption; full-slot arithmetic for value/total; `total==0` path; percentage clamp; 33-cell fill rule; exact row composition; incremental nonzero update behavior; A07C radix-10 formatting; cursor-position restore plus forced SHOW; cross-ROM normalized identity; 15+15 official callers.
- **INFERENCIA FUERTE:** retaining the historical public `uint16_t value/total` C spelling as the intended SDK surface while documenting the handler's 32-bit ABI-slot consumption.
- **DESCONOCIDO:** any older vendor-internal implementation/name for the progress-bar routine and the intended visual name of fill byte `0x00`.
