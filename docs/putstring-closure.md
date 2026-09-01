# A030 — PutString

## Contract

```c
void PutString(const char *str);
```

A030 consumes one pointer to a NUL-terminated byte string. It has no contractual return value and does not guard against `NULL`.

## Confirmed behavior

The November 2005 AS3000 firmware, November 2005 NEO firmware, and July 2013 NEO firmware implement the same algorithm:

- bytes other than LF (`0x0A`) are passed to A010 / `PutChar`;
- LF is not emitted through `PutChar`;
- LF advances to the next OS3K row and column 1 by invoking A004 / `SetCursor` with cursor mode `0x0C` (`CURSOR_MODE_HIDE`);
- the current row is incremented as an 8-bit value before the A004 call, so `255` wraps to `0`;
- CR (`0x0D`) has no special branch in A030 and follows the ordinary `PutChar` path;
- termination is a byte NUL (`0x00`).

Position validation and any platform-specific behavior for an out-of-range row remain A004 responsibilities.

## Relationship to PutStringRaw

A014 / `PutStringRaw` is a distinct lower-level operation: it walks a NUL-terminated string and delegates every byte to `PutChar`. A030 adds explicit LF handling, so the two APIs must not be treated as aliases.

## Cross-generation result

The three canonical handlers are structurally equivalent. Absolute addresses of the current-row global and A004/A010 implementations differ between firmware generations, but the ABI and control flow do not.

## Static verification

A reproducible static regression revalidated the canonical ROM hashes and A030 vectors, handler boundaries, LF branch, cursor-hide/column-one arguments, byte row increment, NUL loop, helper calls, and synthetic normal/LF/empty/multiple-LF/wrap/CR cases.

Result: **36/36 PASS**.

Dynamic hardware/emulator probing remains pending and is not represented as executed evidence.
