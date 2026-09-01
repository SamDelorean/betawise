# A018 / `SetCursorMode` closure

## Contract

```c
void SetCursorMode(CursorMode_e cursor_mode);
```

A018 consumes the low byte of a single 32-bit ABI slot and does not have a contractual return value.

The currently documented constants remain:

```c
CURSOR_MODE_HIDE = 0x0C
CURSOR_MODE_SHOW = 0x0F
```

However, the raw OS3K interface must **not** be modeled as a boolean hide/show call. Official AlphaWord Plus binaries also call A018 with `0x0D`. No vendor name for that additional value has been recovered, so this project deliberately does not invent one.

## Firmware behavior

Across the canonical AS3000 2005, NEO 2005 and NEO 2013 System 3 images, the supplied mode byte is preserved as cursor-mode state.

On NEO 2005/2013, `0x0C` is the sole value tested as the hidden state by A018. When the requested mode is `0x0C`, the graphical cursor is removed only if it is currently visible. Any mode byte other than `0x0C` follows the visible-state branch and causes the cursor to be drawn only if it is currently absent. This makes repeated transitions idempotent and prevents double inversion of the cursor rectangle.

The NEO renderer implements the transition through `RasterOp` using `ROP_DSTINVERT`, a null bitmap, width 2, the active font height, the cursor Y coordinate, and X adjusted to `x - 1` except at zero. The cursor structure fields used by the firmware correlate with BetaWise's `Cursor_t` layout (`pFont`, `x`, `y`, `visible`, and the guard/pause byte).

AS3000 follows its character-LCD controller path instead of the NEO raster path. The mode byte is passed onward to the LCD cursor/display-attribute helper rather than collapsed to a NEO-style graphical boolean. Code using undocumented raw mode values therefore should not assume identical visual semantics across hardware generations.

## Official callers

Structural xref recovery against the AlphaWord Plus A-line veneer found three A018 callers in each inspected official generation. Their immediate mode values are identical in both binaries: `0x0D`, `0x0D`, and `0x0C`.

This caller evidence is why the public `CursorMode_e` definition should be treated as documenting known named constants, not as proof that only two raw mode values exist.

## Confidence

**A — mechanically closed.** The signature, byte-sized argument, hidden-state comparison, state transitions, NEO raster operation, AS3000-vs-NEO implementation difference, and concrete official caller values are supported by source correlation plus direct firmware analysis. A static regression covers all three canonical ROM identities and the official caller patterns. Dynamic hardware probing remains a separate validation layer and is not required for the static ABI closure.
