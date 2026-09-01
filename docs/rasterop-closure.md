# RasterOp / A04C closure

Status: **mechanically closed / source-first revalidated**.

Historical SDK/BetaWise declarations identify index 19 / trap `A04C` as:

```c
void RasterOp(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
              uint8_t *bitmap, RopCode_e rop);
```

with `ROP_CAPTURE=1`, `ROP_DSTINVERT=2`, `ROP_SRCINVERT=3`, `ROP_NOTSRCCOPY=4`, `ROP_WHITENESS=5`, `ROP_BLACKNESS=6`, and `ROP_SRCPAINT=7`.

## Generational availability

* AlphaSmart 3000 System 3 (2005): index 19 is a null vector.
* NEO System 3 (2005): index 19 is a null vector.
* NEO System 3 (2013): index 19 resolves to a real raster handler. Therefore RasterOp is not a portable callable syscall across all three canonical generations.

## NEO 2013 contract

The real handler consumes six arguments in the documented order: four 16-bit coordinate/extent values, a bitmap pointer, and the low byte of the ROP code. It rejects rectangles whose `x+w` exceeds 264 or whose `y+h` exceeds 66.

Before physical raster access it applies the current vertical LCD start-line/roll offset to `y`. The rectangle is then routed to one or both 132-pixel display controllers; crossing `x=132` is split between the two controller helpers while preserving the corresponding bitmap offset.

The two private controller helpers implement the same seven-way ROP switch. Firmware mechanics confirm the historical meanings:

* `ROP_CAPTURE`: copy display bits into the supplied bitmap. The outer handler first clears a buffer region computed from `w` and `ceil(h/8)` before capture.
* `ROP_DSTINVERT`: invert destination bits inside the raster mask.
* `ROP_SRCINVERT`: destination XOR source.
* `ROP_NOTSRCCOPY`: write inverted source.
* `ROP_WHITENESS`: clear destination bits inside the mask.
* `ROP_BLACKNESS`: set destination bits inside the mask.
* `ROP_SRCPAINT`: destination OR source.

An out-of-range ROP value falls through the switch without a defined public operation; callers should use the documented enum only.

The bitmap is handled as 1-bpp data in 8-pixel vertical bands, with horizontal byte stride matching the requested width; controller splitting advances the bitmap pointer by the horizontal portion already consumed.

No cursor or text-selection backing-store update is performed by RasterOp itself.

## Callers and validation

The NEO 2013 ROM contains nine direct internal calls to the RasterOp handler. Concrete immediate callers were recovered for ROP values 1, 2, 5 and 6, independently confirming real use of capture, destination invert, whiteness and blackness. AlphaWordPlus NEO 2012 contains an A04C import veneer but no executable xref to it under the same structural MOVEA.L + indexed-JSR resolver used elsewhere in this ABI audit.

Static regression: **35/35 PASS** over the three canonical ROMs plus the official AlphaWordPlus NEO 2012 corpus. Dynamic probing remains specified but was not required for the mechanical static closure.

The private workpaper retains ROM hashes, handler/helper hashes, reproducible disassembly and exact internal caller offsets; no firmware bytes or extensive disassembly are published here.
