# SDK audit: A1E8-A1FC dynamic-file publication boundary

## Result

A1E8/A1EC/A1F0/A1F4/A1F8/A1FC are mechanically closed dynamic-file identity contracts, but the current repository does not provide independently recovered historical BetaWise public symbols for them.

`os3k/file_dynamic.h` states this explicitly and retains neutral `SYS_Axxx` names. `os3k/syscall.c` likewise maps slots 122-127 to `SYS_A1E8` through `SYS_A1FC`. They are absent from the direct public declaration surface in `os3k.h`.

The historical 2022 filesystem research commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` is a useful discriminator: in the same contiguous filesystem region it promoted independently identified names such as `FileWriteBuffer`, `FileReadBuffer`, `FileSetFolder`, `FileOpen`, `FileClose`, and `ClipboardSet/Get/Clear`, while leaving A1E8-A1FC neutral. This does not prove those six calls were never public in the vendor SDK, but it does mean the repository currently lacks sufficient symbol-provenance evidence to represent them as recovered historical public API.

## Architectural consequence

Under the preserved BetaWise integration rule, the absence of A1E8-A1FC from `os3k.h` is **not currently a demonstrated public-header defect**. Do not include `file_dynamic.h` from `os3k.h`, do not copy its neutral prototypes into the public header merely because the ABI is closed, and do not invent descriptive vendor-style names.

The focused header remains a reconstruction/evidence surface. Revisit public exposure only if a concrete repository consumer, historical SDK declaration, symbol table, applet import, or equivalent independent evidence establishes the public identity/placement.

This resolves the earlier umbrella-header candidate for `file_dynamic.h` without changing ABI closure.

## Safety/build status

Documentation-only reconciliation. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, and no source/header change is made. No m68k build is required for this documentation lot and nothing is newly `BUILD_VALIDATED`.
