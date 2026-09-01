# A048 / index18 — DrawBitmap closure

Status: **MECÁNICA_CERRADA A / REVALIDADO_SOURCE_FIRST**.

## Contract

```c
void DrawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                const uint8_t *bitmap);
```

The historical `os3k.h`/`os3k.pdf` name and prototype are confirmed by the NEO 2005 and NEO 2013 handlers: they consume four 16-bit coordinates/dimensions followed by one pointer and do not provide a contractual return value.

## Generational behavior

### AlphaSmart 3000 (2005)

A048 exists in the A-line table, but its target is a single `RTS`. Therefore the call is a **no-op on the canonical AS3000 2005 ROM**. It consumes no arguments in the handler and has no handler-side drawing effect.

### NEO 2005 / NEO 2013

Both generations implement the real bitmap renderer. The 0x116-byte handlers are instruction-equivalent except for relocation of one internal vertical-display-state global.

Confirmed behavior:

- `x`, `y`, `w`, and `h` are consumed as zero-extended 16-bit values; `bitmap` is a pointer to source bitmap data.
- If `x + w > 264` or `y + h > 66`, the handler returns without drawing; it does not clip an oversized rectangle at those outer bounds.
- The display is partitioned horizontally at x=132 into two LCD controllers. Rectangles wholly on one side use the corresponding controller; rectangles crossing x=132 are split into two writes.
- Before physical rendering, an internal vertical start-line/roll value is added to `y`. Independent firmware paths initialize that value to zero, update it modulo 64, and emit it as LCD start-line state. This is an internal implementation detail, not an additional API argument.
- The NEO raster helpers perform masked 1-bpp writes to the physical LCD controller RAM while preserving neighboring destination bits.
- The bitmap source is read by the renderer; no source-buffer write was observed. There is no NULL guard, so any path that reads bitmap data requires a readable source pointer.
- `DrawBitmap` does **not** update the logical cursor or the selection/character backing store. For example, `PutChar` calls the bitmap renderer and then performs its own shadow/cursor bookkeeping separately.

## Callers and negative search

The audited NEO `PutChar` implementation is a positive firmware-internal caller: it obtains glyph bitmap data from the current system font and routes it through A048.

Official AlphaWordPlus binaries from 2005 and 2012 both contain an imported A048 veneer in their A-line stub blocks. A structural 68k target-resolution scan (rather than a raw `A048` byte search) found **zero executable calls to that veneer in both binaries**. This is recorded as a negative search for that corpus, not as a claim that no other SmartApplet uses A048.

## Refutations / corrections

- The inherited assumption that AS3000 and NEO provide equivalent A048 implementations is false: AS3000 is a naked-`RTS` stub while NEO contains the renderer.
- An early partial analysis incorrectly described the relocated NEO global as being added to `x`. Direct disassembly shows that it is added to **`y`**, and independent firmware code identifies it mechanically with vertical start-line/roll handling.
- `DrawBitmap` is not a character-shadow update primitive; physical rendering and logical character backing-store maintenance are separate layers.

## Verification

Static regression: **EXECUTED — 33/33 PASS**. It revalidates the three canonical ROM hashes, A048 vectors, AS3000 stub, NEO ABI and bounds, controller split, vertical start-line adjustment, raster-helper equivalence across NEO generations, controller I/O anchors, and the two AlphaWordPlus negative-xref checks.

A dynamic hardware probe remains **SPECIFIED / NOT EXECUTED** and is treated as additional validation rather than a blocker for the mechanical static closure.

Raw ROM bytes, extensive disassembly, helper listings, and firmware-address dossiers remain private and are not included in this repository.
