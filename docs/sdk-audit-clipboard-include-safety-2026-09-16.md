# SDK audit: clipboard include safety

## Finding

The pending A20C/A210/A214 public-exposure fix can remain a narrow include-graph change without importing adjacent reconstructed clipboard contracts.

`os3k/file_clipboard_buffer.h` is self-contained: it includes only `<stdint.h>` and declares `ClipboardSet`, `ClipboardGet`, and `ClipboardClear`. It does not include `os3k.h`, depend on `FILE`, `KeyMod_e`, `AppletHeader_t`, or any other BetaWise public type, and it does not pull in `file_clipboard_edit.h` or `file_clipboard_state.h`.

Therefore, from the current source graph, adding `#include "file_clipboard_buffer.h"` to the public umbrella does not create a textual include cycle and does not mechanically expose the adjacent A200/A204/A208 or A218-A228 families.

This is source-graph evidence only. It does not constitute m68k compiler validation.

## Structural placement

If/when the header change is made, the least disruptive BetaWise-compatible placement is with the existing top-level includes in `os3k.h`, after the standard integer headers and alongside `version.h`. The dedicated header remains the single declaration authority; do not duplicate its three prototypes in `os3k.h`.

## Scope boundary

This finding applies only to the already-closed historical BetaWise symbols A20C/A210/A214. It does not promote any `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry and does not authorize exposing adjacent neutral `SYS_Axxx` clipboard contracts.

## Build gate

The actual umbrella-header edit remains `PENDING_M68K_BUILD`. After changing the include graph, require both:

- `make -C os3k clean all`
- one representative applet clean build

Do not mark the integration `BUILD_VALIDATED` until both execute successfully on the configured m68k toolchain.
