# A01C / `GetCursorMode` closure

## Contract

```c
void GetCursorMode(CursorMode_e* cursor_mode);
```

A01C consumes one pointer argument and copies exactly one byte of stored cursor-mode state into `*cursor_mode`. The pointer must be valid; the firmware does not provide a null guard. There is no contractual return value.

## Relationship to A018

A01C is the direct reader for the mode byte written by A018 / `SetCursorMode`.

The three canonical System 3 generations use different absolute state locations, but the operation is otherwise identical: load the caller's output pointer, copy one byte from the cursor-mode global, return.

The value is **not normalized** to the two currently named public constants. This matters because official AlphaWord Plus callers of A018 use `0x0D` as well as `0x0C`; A01C returns that raw stored value unchanged. `CURSOR_MODE_HIDE = 0x0C` and `CURSOR_MODE_SHOW = 0x0F` therefore remain known named constants rather than an exhaustive statement of the raw mode domain.

A01C also does not derive its result from the graphical `visible` field used by NEO cursor rendering. It reports the requested/stored cursor mode, not a reconstructed boolean visibility state.

## Official callers

One structural A01C xref was recovered in each inspected AlphaWord Plus generation. Both callers pass the address of a local byte, invoke A01C, and immediately read that byte afterward. This directly confirms the output-pointer ABI and that the useful result is delivered through caller-provided storage rather than through `D0`.

## Confidence

**A — mechanically closed.** The 12-byte handlers are structurally identical across AS3000 2005, NEO 2005 and NEO 2013 apart from the absolute mode-global address. Static regression validates the three canonical ROM identities, handler identities, byte-copy behavior, and both official caller patterns. Dynamic hardware probing remains a separate validation layer.
