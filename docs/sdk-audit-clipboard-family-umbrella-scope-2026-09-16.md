# SDK audit — Clipboard family umbrella scope

Date: 2026-09-16
Branch: `sdk/abi-automation`

## Finding

The previously recorded public-header visibility gap is specifically limited to the historical BetaWise names A20C/A210/A214: `ClipboardSet`, `ClipboardGet`, and `ClipboardClear` are linker-visible in `os3k/syscall.c` and declared in `os3k/file_clipboard_buffer.h`, but are absent from the canonical public `os3k/os3k.h`.

Adjacent clipboard auxiliary headers must **not** be swept into the same umbrella-header fix. `file_clipboard_edit.h` (A200/A204/A208) and `file_clipboard_state.h` (A218/A21C/A220/A224/A228) explicitly retain neutral `SYS_Axxx` names because reliable original public names have not been recovered. Their mechanical contracts being closed is not evidence that they belonged to the historical BetaWise public header.

## Safe integration boundary

Any future source/header change for the A20C/A210/A214 visibility gap must remain narrow: expose only declarations whose historical public names are already evidenced. Do not use the clipboard family grouping as justification to add the neighboring neutral A-line headers to `os3k.h`, and do not promote any limited-evidence identity.

This preserves the audit rule that mechanical closure and public SDK identity are separate claims.

## Build gate

No source/header change is made in this lot. A future `os3k.h`/include-graph integration remains `PENDING_M68K_BUILD` until both required gates actually pass:

- `make -C os3k clean all`
- one representative applet clean build

Do not mark that future integration `BUILD_VALIDATED` before both pass.
