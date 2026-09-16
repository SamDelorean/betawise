# SDK audit: clipboard declaration authority

## Finding

A20C/A210/A214 (`ClipboardSet`, `ClipboardGet`, `ClipboardClear`) have a mechanically closed ABI and historical BetaWise names. Their current declarations live only in `os3k/file_clipboard_buffer.h`, while the veneers are exported directly by `os3k/syscall.c` and the public umbrella `os3k.h` does not declare or include them.

The safe integration shape is therefore narrower than copying the prototypes into `os3k.h`: `file_clipboard_buffer.h` should remain the single declaration authority, and any future public exposure should be by including that header from `os3k.h` (or by another equivalent single-authority arrangement), not by maintaining duplicate prototypes.

This matters because duplicate declarations would create two independently editable ABI surfaces and could silently drift in signedness, widths, qualifiers, or pointer types.

## Scope boundary

This conclusion applies only to A20C/A210/A214. It does not authorize exposing the adjacent clipboard families whose vendor identities remain unrecovered, and it does not change any syscall closure state.

## Build gate

No header/source change is made in this audit lot. A future include-graph change remains `PENDING_M68K_BUILD` until both of the following actually pass on the configured m68k toolchain:

- `make -C os3k clean all`
- one representative applet clean build

Do not mark that future change `BUILD_VALIDATED` before both gates pass.
