# A004 / `_OS3K_SetCursor` closure

Status: **mechanical confidence A** for the raw System 3 ABI. The public BetaWise `SetCursor()` wrapper remains a separate higher-level layer.

## Contract

```c
void _OS3K_SetCursor(uint8_t row, uint8_t col, CursorMode_e cursor_mode);
```

The compared System 3 handlers consume three byte-valued ABI slots in this order: `row`, `col`, `cursor_mode`. Historical SDK/debug material uses the same prototype. Official callers use `CURSOR_MODE_HIDE` (`0x0C`) and `CURSOR_MODE_SHOW` (`0x0F`). No portable return value is contractual.

## Firmware evidence

Canonical ROM comparison resolves A004/index 1 to:

- AS3000 System 3 (2005): runtime `0x004CD826`, handler length `0x8E`.
- NEO System 3 (2005): runtime `0x005D11EC`, handler length `0x88`.
- NEO/System 3.15 (2013): runtime `0x0042216C`, handler length `0x88`.

All three handlers were independently re-extracted from canonical ROMs and disassembled with the project's reproducible 68000 decoder with zero unknown opcodes.

NEO 2005 and NEO 2013 have the same 42-instruction control skeleton apart from relocated absolute addresses. They store the logical row/column, convert working copies to zero-based coordinates, calculate graphical cursor coordinates from font height/width and origin state, and pass the third argument to the cursor-mode helper.

AS3000 uses the character-LCD implementation. It selects the LCD half and DDRAM base from the row, computes the column address using `col - 1`, updates logical row/column state and applies the requested cursor mode. AS3000 incidentally maps row 0 to row 1 and rows above 4 to row 4; NEO does not reproduce that behavior, so row clamping is **not** a cross-platform ABI guarantee. Callers should provide coordinates valid for the active platform/API.

## Source genealogy

The early AS3000 `LCDModule.obj` retains independent symbols including `LCDMoveCursor`, `LCDSetCursorAttributes`, `LCDGetCursorPosition` and `LCDGetCursorAttributes`. That implementation validates the classic 4x40 character geometry and establishes the historical lineage of the later cursor API. It is supporting genealogy rather than a substitute for the 2005/2013 firmware evidence.

## Official caller evidence

Two official AlphaWord Plus generations were resolved structurally to their A004 veneers rather than by raw word matching:

- AlphaWord Plus 2005: 113 executable xrefs to the A004 veneer.
- AlphaWord Plus NEO 2012: 118 executable xrefs.

Representative immediate call sites reconstruct `SetCursor(4, 1, CURSOR_MODE_HIDE)` and `SetCursor(1, 1, CURSOR_MODE_SHOW)`, confirming argument order and the two documented cursor modes. No observed caller relies on a return value.

## Raw syscall versus BetaWise wrapper

`_OS3K_SetCursor()` is the raw A004 syscall. `SetCursor()` in BetaWise is not always an alias:

- with `g_pCurFont == NULL`, the wrapper delegates directly to A004;
- with an extended font active, the wrapper performs BetaWise-owned scrolling/start-line handling, updates its private cursor state and graphical coordinates, optionally clears a newly exposed row, and then applies the cursor mode without calling raw A004.

These layers must remain distinct, just as for A000/ClearScreen.

## Regression status

A dedicated static regression was **executed** against the three canonical ROMs and the two official AlphaWord Plus binaries: **37/37 PASS**, exit 0, empty stderr. It checks ROM/handler identity, A-line table resolution, handler boundaries, argument slots, structural effects, generation comparison, official veneers/xrefs and concrete HIDE/SHOW call sites.

A dynamic/emulator probe remains **specified / not executed** and is additional validation rather than a blocker for this static mechanical closure.

Private Drive workpapers retain handler hashes, full reproducible disassembly listings, regression hashes and longer firmware evidence. ROM bytes and extended disassembly are intentionally not published here.
