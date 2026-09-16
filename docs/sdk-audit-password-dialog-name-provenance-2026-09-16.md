# SDK audit: A1E0/A1E4 password-dialog name provenance

## Result

A1E0/A1E4 are mechanically closed and currently exported as `PasswordVerifyFileDialog` and `PasswordChangeFileDialog` in `syscall.c`, with matching public declarations in `os3k.h`. Their current descriptive spellings, however, are reconstruction-era names rather than independently demonstrated historical BetaWise/vendor symbols.

Repository history establishes the boundary precisely:

- commit `056e417566596699efa0d53d923a8ea25d5f2f69` (2026-08-27), message `sdk: identify A1E0 and A1E4 password dialogs`, changed only two `syscall.c` veneers from `SYS_A1E0`/`SYS_A1E4` to the current descriptive names;
- that commit did not add an historical SDK declaration, symbol-table citation, or higher-level `os3k.c` wrapper;
- current `syscall.c` therefore maps those descriptive names directly to the raw A-line veneers;
- current `os3k.h` publishes the same spellings on the compilation surface.

Classification:

`PUBLIC_COMPILATION_SURFACE / RECONSTRUCTION_ERA_DESCRIPTIVE_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`

This is a provenance correction only. It does not weaken or reopen the closed mechanical contracts and does not assert that the descriptive names are functionally wrong.

## Architectural consequence

Do not use `PasswordVerifyFileDialog` or `PasswordChangeFileDialog` as precedent for deriving vendor-style names for neutral closed calls, including the adjacent A1D0-A1DC and A1E8-A1FC families. Those adjacent families correctly remain neutral absent independent symbol evidence.

Do not rename/revert A1E0/A1E4 in this documentation lot. Their current names are part of the existing public header and veneer symbol surface; any future naming cleanup requires consumer/source-compatibility review and an actual m68k build, so such a change is `PENDING_M68K_BUILD` until performed.

## Safety/build status

Documentation-only provenance reconciliation. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, no source/header change is made, and nothing is newly `BUILD_VALIDATED`.
