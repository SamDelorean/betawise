# SDK audit: clipboard include safety — architectural correction

## Status

**SUPERSEDED AS AN INTEGRATION INSTRUCTION.**

The earlier source-graph observation remains mechanically true: `os3k/file_clipboard_buffer.h` is self-contained and including it from `os3k.h` would not itself create a textual include cycle or expose the adjacent reconstructed clipboard headers.

That observation does **not** establish the correct BetaWise SDK architecture.

## Authoritative architectural rule

Preserve the original BetaWise organization unless concrete evidence requires a correction:

- reconstructed ABI evidence determines the mechanical syscall contract;
- `os3k.h` is the historical public C/SDK declaration surface;
- `syscall.c` contains the A-line symbol/opcode veneers;
- `os3k.c` contains wrappers/adaptations only where the historical SDK architecture requires them;
- reconstructed auxiliary headers such as `file_clipboard_buffer.h`, `file_dynamic.h`, and `file_password_state.h` are evidence/reconstruction artifacts and are **not** automatically public SDK subheaders.

Therefore, the default correction for a closed syscall with an evidenced historical BetaWise public symbol is to reconcile its declaration into the appropriate historical `os3k.h` structure, not to turn `os3k.h` into an umbrella over reconstructed headers.

## A20C/A210/A214 consequence

`ClipboardSet`, `ClipboardGet`, and `ClipboardClear` remain a real public-surface inconsistency because their historical BetaWise names are already linked by `syscall.c` while `os3k.h` does not expose them.

The next safe design step is to determine the exact placement and declaration style inside the existing `os3k.h` organization, using comparable original BetaWise direct veneers (including the Battery pattern) as structural controls. `file_clipboard_buffer.h` remains supporting evidence for the recovered contracts; it is not, by itself, the intended public integration mechanism.

Do not use this finding to import adjacent reconstructed clipboard families or to promote any `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry.

## Build gate

No header/source change is made by this correction. Any eventual `os3k.h` change remains `PENDING_M68K_BUILD` and must pass:

- `make -C os3k clean all`
- one representative applet clean build

Do not mark `BUILD_VALIDATED` until both execute successfully on the configured m68k toolchain.
