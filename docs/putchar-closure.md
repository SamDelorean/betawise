# A010 / `PutChar` closure

Status: **mechanical confidence A** for the System 3 ABI.

## Contract

```c
void PutChar(char c);
```

The handler consumes the low byte of the first ABI slot. It writes the corresponding glyph at the current OS3K cursor position and advances cursor state. No portable return value is contractual.

The public contract is stable, but cursor validation and wrapping are generation-specific and must not be generalized across AS3000 and NEO.

## Source genealogy

Historical SDK material declares `void PutChar(char c)`. The early AS3000 `LCDModule.obj` independently retains `LCDSetChar`: it validates the classic 4x40 cursor geometry, outputs one character and increments the cursor column. That is strong genealogy for the name and purpose, while the later System 3 firmware remains the primary evidence for the modern implementation.

BetaWise's `fputc`/`putchar` path is a separate higher-level layer. In particular, when an extended font is active BetaWise draws its own bitmap and maintains private cursor/scroll state. It must not be collapsed into raw A010.

## Firmware evidence

A010/index 4 resolves to:

- AS3000 2005: runtime `0x004CD406`, length `0x64`.
- NEO 2005: runtime `0x005D0ABE`, length `0x168`.
- NEO/System 3.15 2013: runtime `0x00421792`, length `0x174`.

All three handlers were re-extracted from canonical ROMs and disassembled reproducibly with zero unknown opcodes.

### AS3000

AS3000 checks logical row 1..4 and column 1..40. An invalid cursor state causes a return without drawing or incrementing. For a valid state it translates the input byte through the firmware character table, selects the appropriate LCD half, writes the translated data through the low-level LCD helper and increments the column by one. The handler does not automatically wrap to another row.

### NEO

NEO forces the System font for this raw path, derives the glyph bitmap as `font.bitmap_data + font.max_bytes * byte`, and draws it through A048/`DrawBitmap` using current graphical cursor coordinates and the font geometry. It also stores the original byte in OS3K's logical screen shadow, indexed from the logical row and column, then advances cursor state.

When horizontal advancement reaches the 264-pixel display edge, NEO resets the column to 1, advances the row and moves to the next font-height line. When the next line reaches the 64-pixel vertical edge, the row returns to 1 and the graphical y coordinate returns to its origin. This wrapping is real NEO behavior, not an AS3000 guarantee.

The NEO handler assumes a valid logical cursor state before indexing the shadow buffer. Therefore the AS3000 4x40 validation must not be advertised as a cross-platform safety property.

## NEO 2013-only suppression guard

NEO 2013 adds one internal guard absent from NEO 2005: when byte global `0x5C8E` equals 1, the handler skips the rendering core and returns through its epilogue. A small internal setter writes the low byte of its argument to that global, and six direct internal call sites toggle it around callback/operation paths.

The mechanical effect is therefore established as suppression of this rendering path, but no reliable vendor name or higher-level semantic name has been recovered for the flag. It remains intentionally unnamed rather than receiving an invented API label.

## Official callers

Structural xref resolution against official AlphaWord Plus binaries finds:

- 49 executable references to the A010 veneer in AlphaWord Plus 2005;
- 49 executable references in AlphaWord Plus NEO 2012.

Each generation contains 27 immediate-character calls and 22 calls whose byte value is produced dynamically in `D0`. Representative literal callers emit space (`0x20`), double quote (`0x22`), period (`0x2E`) and colon (`0x3A`), confirming ordinary one-byte text/UI usage independently of the firmware analysis.

## Regression

A dedicated static regression was **executed 50/50 PASS**, exit 0, empty stderr. It checks canonical ROM identity, table/handler resolution, argument width, AS3000 geometry checks and translation path, NEO bitmap/shadow/wrap mechanics, A048 targets, the NEO 2013-only suppression guard, core-generation similarity and the 49+49 official caller census.

A dynamic/emulator probe remains **specified / not executed** and is additional validation rather than a blocker for the static mechanical closure.

Full handler hashes, long disassembly listings, the regression source/output and internal-address detail remain in the private Drive workpaper. Firmware bytes are intentionally not published here.
