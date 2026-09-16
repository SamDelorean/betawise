# SDK audit: FileGetFileInfo name provenance (A1C4)

## Result

A1C4 is mechanically closed and currently exported/deployed as `FileGetFileInfo` in `syscall.c` and `os3k.h`, but repository history does not establish that spelling as an independently recovered historical BetaWise/vendor public symbol.

The provenance boundary is explicit:

- commit `38b622ce70af52b46282d19a129431f30e3c39a0` (2026-08-26), message `sdk: name A1C4 FileGetFileInfo`, changed the `syscall.c` veneer from neutral `SYS_A1C4` to `FileGetFileInfo`;
- immediately before that rename the source comment still described A1C4 tentatively (`set file properties... also name?`), while the same commit replaced it with the descriptive name;
- the commit modified `syscall.c`, not an historical SDK declaration or `os3k.c` wrapper;
- current `os3k.h` publishes the reconstructed five-argument pointer/mirror contract under `FileGetFileInfo`.

Classification:

`PUBLIC_COMPILATION_SURFACE / RECONSTRUCTION_ERA_DESCRIPTIVE_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`

This classification concerns name provenance only. It neither weakens nor reopens the closed mechanical ABI contract and does not assert that `FileGetFileInfo` is functionally incorrect.

## Architectural consequence

Do not use `FileGetFileInfo` as evidence that descriptive names inferred during ABI reconstruction are recovered vendor API spellings, and do not use it as precedent for renaming neutral closed calls.

Do not rename/revert A1C4 in this documentation lot. The current name is part of both the direct veneer and public header surface. Any future naming cleanup requires consumer/source-compatibility review plus an actual m68k build and is therefore `PENDING_M68K_BUILD` until performed.

## Safety/build status

Documentation-only provenance reconciliation. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, no source/header change is made, and nothing is newly `BUILD_VALIDATED`.
