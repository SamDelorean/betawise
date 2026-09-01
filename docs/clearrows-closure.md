# A024 / ClearRows closure

Status: **mechanically closed / source-first revalidated**.

## Public contract

```c
void ClearRows(uint8_t row_first, uint8_t row_last);
```

The System 3 documentation names A024 `ClearRows`. Direct firmware analysis confirms two byte-sized row arguments and no contractual return value.

## Historical source correlation

Early AlphaSmart 3000 source contains the direct predecessor `LCDClearPartOfDisplay(UInt8 iubStartRow, UInt8 iubEndRow)`. Its source comment says the interval is inclusive. The routine computes `end - start + 1` and, in debug builds, rejects spans smaller than two rows and spans of four or more rows. This establishes an important intended precondition: the historical routine was for clearing a **partial display of exactly two or three rows**; one row belongs to the row-clearing primitive, while the whole display belongs to the full-screen clear primitive.

## Generation behavior

### AS3000 System 3

The AS3000 handler retains the historical optimization. It hides the cursor and uses the LCD controllers' hardware clear command when an entire two-row controller can be cleared. Mixed spans use the A020 row-clearing primitive for the remaining row. This closely matches the historical `LCDClearPartOfDisplay` control flow.

Because the release firmware does not retain the historical debug assertions, callers must not treat every `uint8_t` pair as valid merely because the ABI accepts bytes. Invalid spans can over-clear or otherwise produce generation-specific results.

### NEO System 3

NEO 2005 and NEO 2013 implement A024 as a straightforward inclusive loop from `row_first` through `row_last`, calling A020 for each row with columns 1 through 40. The two NEO handlers are byte-identical in the audited releases apart from their surrounding address space.

If `row_first > row_last`, the NEO loop clears no rows. If `row_last == 255` and the loop reaches it, the byte row counter wraps to zero, so the routine may fail to terminate. This is another reason to keep calls within physical screen rows.

## Portable use

For portable AS3000/NEO behavior, use A024 for its historically intended **two- or three-row partial-display spans**. Use A020 (`ClearRowCols`) for a single row/range and A000 (`ClearScreen`) for the complete display.

The observed valid AS3000 partial-display spans are 1..2, 1..3, 2..3, 2..4, and 3..4. NEO firmware is mechanically more permissive, but relying on that broader behavior is not portable.

## Validation

The closure was checked against the canonical AS3000 2005, NEO 2005, and NEO 2013 firmware images. A project-created static regression checks ROM identity, handler identity/boundaries, the historical-source correspondence, exact delegation to A020, generation-specific control flow, inclusive behavior, invalid-span behavior, and the NEO byte-wrap hazard. Result: **53/53 PASS**.

Long firmware listings, ROM bytes, and private reverse-engineering workpapers remain outside the public repository.