# SDK audit — Clipboard public-header visibility gap

Date: 2026-09-16
Branch: `sdk/abi-automation`

## Finding

A20C/A210/A214 are bound in `os3k/syscall.c` directly to the historical BetaWise symbols `ClipboardSet`, `ClipboardGet`, and `ClipboardClear`. Their mechanically closed C contracts are already declared in `os3k/file_clipboard_buffer.h` and documented by the clipboard closure corpus.

The public umbrella `os3k/os3k.h` does not currently declare those three historical symbols and does not include `file_clipboard_buffer.h`. This leaves a real SDK header-visibility inconsistency: the linker-visible historical names exist, and an auxiliary header has exact declarations, but a consumer including only the canonical BetaWise public header cannot call them with prototypes.

## Safe scope

This finding does **not** reopen A20C/A210/A214 and does not alter their closure state. It does not infer new vendor names or semantics: all three names already exist in `syscall.c`, and the declarations already exist in `file_clipboard_buffer.h`.

Do not solve this by duplicating declarations blindly. The preferred source change should preserve a single declaration authority, most likely by reconciling the auxiliary header with the umbrella include structure after checking for include-cycle/compatibility effects.

## Build gate

No source/header change is made in this audit lot. Any later change to `os3k.h` or the public include graph is `PENDING_M68K_BUILD` until both required gates actually pass on the configured m68k toolchain:

- `make -C os3k clean all`
- one representative applet clean build

Do not mark the future integration `BUILD_VALIDATED` before both pass.
