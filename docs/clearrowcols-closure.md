# A020 / ClearRowCols closure

Status: **mechanically confirmed (A)** against the canonical AS3000 2005, NEO 2005 and NEO 2013 System 3 ROMs.

```c
void ClearRowCols(uint8_t row, uint8_t col_first, uint8_t col_last);
```

## Contract

`ClearRowCols` positions the raw System 3 cursor at `(row, col_first)` with raw cursor mode `0x0C` (hide), then writes ASCII space (`0x20`) through the normal `PutChar` syscall for every column in the inclusive interval `[col_first, col_last]`.

If `col_first > col_last`, no spaces are emitted. The function has no contractual return value and does not restore the previous cursor mode.

The implementation is composition rather than a separate graphical erase primitive: in every canonical ROM, A020 calls the **exact A004 / SetCursor entry** and then the **exact A010 / PutChar entry** from that ROM's A-line table.

## ABI details

The three public arguments are consumed as bytes from their 32-bit ABI argument slots. The loop counter is also maintained as a byte and compared unsigned with `col_last` using the equivalent of `<=`, which confirms that the upper bound is inclusive.

The routine performs no independent row/column validation; validation or display effects are inherited from `SetCursor` and `PutChar`.

### Important precondition

The byte loop means `col_last == 255` is not a safe general-purpose endpoint when `col_first <= col_last`: after emitting column 255 the byte counter wraps to zero and remains `<= 255`. Therefore the `uint8_t` type is the ABI representation, **not a promise that every 0..255 interval is valid**. Normal callers use physical display-column ranges (for example 1..40 on AS3000 and 1..41 on NEO in the SDK debug code).

## Cross-generation comparison

The AS3000 2005, NEO 2005 and NEO 2013 handlers are algorithmically equivalent. Their only material differences here are code placement and branch encoding/offsets; the SetCursor→space-writing loop contract is unchanged.

## Evidence and validation

The canonical ROM hashes were revalidated before analysis. A static regression covering ROM identity, A020 table targets, handler identity, exact A004/A010 call targets, cursor-hide constant, space constant, unsigned inclusive loop behavior and adversarial boundary cases completed **38/38 PASS**.

Raw ROM bytes and full disassembly listings are intentionally retained only in the project's private evidence store.

Dynamic hardware/emulator probing remains pending; it is not represented as executed validation.
