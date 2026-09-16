# SDK audit: FileDeleteChars name provenance (A1A4)

## Result

A1A4 is mechanically closed and currently exported/deployed as `FileDeleteChars` in both `syscall.c` and `os3k.h`, but the current name must not be treated as independently recovered historical BetaWise/vendor spelling.

The branch history gives a precise provenance boundary:

- commit `35be225c7058d0f33ba57aa3c6a33ca961852b6b` (2026-08-26), message `sdk: identify A1A4 as FileDeleteChars`, changed only the `syscall.c` veneer from `SYS_A1A4` to `FileDeleteChars`;
- no historical declaration or wrapper was introduced by that commit;
- the current `os3k.h` later exposes `FileDeleteChars(uint32_t delete_count)` as the public compilation spelling;
- `os3k.c` contains no higher-level `FileDeleteChars` wrapper: the name maps directly to the raw A-line veneer.

Therefore A1A4 belongs with the reconstruction-era descriptive-name cases rather than the independently pre-existing historical-symbol cases such as ClipboardSet/Get/Clear.

Classification:

`PUBLIC_COMPILATION_SURFACE / RECONSTRUCTION_ERA_DESCRIPTIVE_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`

This does not change the closed ABI contract or assert that the name is wrong; it only records that repository evidence currently does not establish it as the original vendor/BetaWise public symbol.

## Architectural consequence

Do not use `FileDeleteChars` as precedent for inventing descriptive names for neutral `SYS_Axxx` contracts. Do not rename or revert A1A4 in this documentation lot: that would alter the existing public compilation surface and direct veneer symbol. Any future naming cleanup requires consumer/source-compatibility review and m68k build validation, and is therefore `PENDING_M68K_BUILD` until performed.

## Safety/build status

Documentation-only provenance correction. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, no source/header change is made, and nothing is newly `BUILD_VALIDATED`.
