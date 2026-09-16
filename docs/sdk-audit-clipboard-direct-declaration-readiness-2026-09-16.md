# SDK audit: Clipboard direct-declaration readiness

## Result

The A20C/A210/A214 Clipboard trio is now fully reconciled against the preserved BetaWise source layering and is a safe public-header repair candidate.

`syscall.c` exports the historical BetaWise names `ClipboardSet`, `ClipboardGet`, and `ClipboardClear` directly. The focused reconstruction header `file_clipboard_buffer.h` closes their mechanical signatures as:

```c
int32_t ClipboardSet(const uint8_t* buffer, uint32_t count,
    uint8_t clipboard_tag);
int32_t ClipboardGet(uint8_t* buffer, uint32_t count,
    uint8_t read_variant);
void ClipboardClear(void);
```

A complete inspection of `os3k.c` confirms that it contains no Clipboard wrapper or adaptation. This distinguishes the trio from APIs such as `ClearScreen`, `SetCursor`, `TranslateKeyToChar`, `CallSysInt`, and stdio functions, where BetaWise intentionally interposes a higher-level wrapper.

The current `os3k.h` file API section does not declare the Clipboard trio.

## Architectural consequence

Under the original BetaWise architecture, the correct integration is therefore direct declarations in `os3k.h`, matching the direct veneers in `syscall.c`. `file_clipboard_buffer.h` remains an ABI/evidence artifact and must not be included as a new public subheader.

No adjacent neutral Clipboard contract is promoted by this result. A200/A204/A208 and A218-A228 remain outside the public header because their historical public/vendor identities have not been recovered.

## Build gate

This lot intentionally records the completed source-graph reconciliation only; it does not modify `os3k.h`. The subsequent header edit is mechanically narrow but changes the public SDK compilation surface and must remain `PENDING_M68K_BUILD` until `make -C os3k clean all` and a representative applet clean build are actually run in the m68k environment.

Nothing in this lot is newly `BUILD_VALIDATED`. No closed syscall is reopened and no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted.
