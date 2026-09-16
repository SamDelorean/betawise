# SDK audit: preserve BetaWise direct-declaration architecture

## Rule

For SDK consolidation, reconstructed auxiliary headers are evidence artifacts, not an automatic extension mechanism for the public SDK.

The default architecture remains the original BetaWise split:

1. `os3k.h` — public C declarations, public types, constants and source-level compatibility surface.
2. `syscall.c` — direct A-line veneers mapping public/private symbols to trap slots.
3. `os3k.c` — wrappers/adaptations only where BetaWise historically needs behavior beyond a direct veneer.
4. reconstructed focused headers — contract/evidence isolation for reverse engineering; they do not become public includes merely because their contracts are closed.

Reconstructed ABI evidence controls the mechanical signature. BetaWise structure controls where that signature is represented, except where evidence proves the historical declaration itself incorrect.

## Current structural evidence

The present `os3k.h` already follows this model: direct-veneer public functions such as `ShowBatteryPercentage`, `FileWriteBuffer`, `FileReadBuffer`, `FileOpen`, `FileClose`, `AppletFindByName`, and `AppletSendMessage` are declared directly in `os3k.h`. Closed neutral raw contracts such as A248-A270 are also declared directly there. `syscall.c` maps these symbols directly to their A-line slots.

A20C/A210/A214 follow the same direct-veneer shape in `syscall.c`: `ClipboardSet`, `ClipboardGet`, and `ClipboardClear`. Their absence from `os3k.h` is therefore a narrow declaration-surface inconsistency. The structurally consistent repair candidate is direct declarations in the appropriate `os3k.h` section, using the mechanically closed signatures already recorded in `file_clipboard_buffer.h`; do not make that auxiliary header part of the public include graph merely to expose the symbols.

## Consequence for earlier umbrella inventories

Earlier notes describing reconstructed headers as candidates for wholesale inclusion must be interpreted only as inventories of contracts missing from the public surface. They are not integration instructions. Each missing contract must be reconciled individually as:

`closed ABI contract <-> syscall.c veneer <-> os3k.c wrapper if historically required <-> direct os3k.h representation`

Do not create a parallel subheader-based SDK architecture.

## Safety boundary

This rule does not promote `CERRADA_CON_LÍMITE_DE_EVIDENCIA`, does not reopen closed syscalls, and does not recover vendor names by analogy. Neutral `SYS_Axxx` names remain neutral where identity is not evidenced.

No source/header change is made in this lot. Any eventual `os3k.h` edit remains `PENDING_M68K_BUILD`; require `make -C os3k clean all` and a representative applet clean build before `BUILD_VALIDATED`.
