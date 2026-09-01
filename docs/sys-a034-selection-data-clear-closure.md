# A034 / index 13 — selection-data backing-store clear

Status: **mechanical confidence A**. Historical/name confidence is tracked separately.

## Current neutral ABI

```c
void SYS_A034(void);
```

A034 takes no caller arguments and has no portable return-value contract. It operates entirely on OS-owned display/selection state.

## Confirmed behavior

A034 invalidates the backing store used to retain LCD/text-selection data by filling every active entry with `0xFF`.

`0xFF` is an invalid/unwritten sentinel for this structure. It is not a display color and A034 is **not** a second clear-screen primitive. The handler itself does not issue LCD clear commands, update the visible display, or normalize cursor state.

The historical AS3000 source contains the exact predecessor `LCDClearSelectionData(void)`. That routine fills `gpubLCDData[LCD_MAX_ROWS][LCD_MAX_COLUMNS]` with `0xFF`; the related historical selection-refresh path interprets `0xFF` as a location for which no data has been written, while the save-screen-data path writes ordinary characters into the same backing store.

The modern/public name remains `SYS_A034`: continuity of the operation is demonstrated, but an exported modern vendor symbol has not been recovered.

## Generation comparison

- **AS3000 System 3 (2005):** fixed 4-by-40 selection-data matrix; every entry is set to `0xFF`.
- **NEO System 3 (2005):** same nested-fill operation over runtime display dimensions; row addressing uses a 132-byte stride.
- **NEO System 3.15 (2013):** instruction-level algorithm is equivalent to the 2005 NEO implementation after relocation of globals/buffer addresses; it also uses the runtime geometry and 132-byte row stride.

The NEO change is therefore storage/layout evolution rather than a change from selection-data invalidation to visible screen clearing.

## Callers and negative evidence

The official AlphaWord Plus 2005 and AlphaWord Plus NEO 2012 applets both contain the A034 import veneer in their dense A-line syscall tables. A structural 68k target-resolution sweep found **zero executable xrefs** to that veneer in each applet. This is recorded as a negative caller result, not as absence of the syscall.

No caller evidence supports arguments or a contractual return value.

## Adversarial checks

The audit explicitly attempted to refute the historical-selection interpretation by testing alternatives such as a second `ClearScreen`, framebuffer/color fill, cursor reset, or direct hardware operation. Those alternatives are contradicted by the handler mechanics and by the historical source/consumer relationship of the `0xFF` sentinel.

## Validation status

A private static regression was **executed** against the three canonical ROMs, the historical source correlation, and both official AlphaWord Plus applets: **36/36 checks passed**. It verifies canonical ROM identity, handler boundaries, fixed AS3000 geometry, NEO runtime geometry and stride, normalized NEO equivalence, the historical `LCDClearSelectionData`/sentinel/save relationships, imported A034 veneers, and the negative xref result.

Dynamic hardware/emulator probing is additional validation and is not claimed as executed here.

ROM bytes, extensive disassembly, proprietary firmware and private workpapers are intentionally not published in this repository.
