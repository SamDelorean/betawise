# SDK audit: A254 public-neutral provenance

## Material finding

A254 is already part of the public compilation surface in `os3k.h` as:

```c
uint8_t SYS_A254(uint8_t token_group, char *group_name_out);
```

The focused reconstruction header `os3k/file_token_group.h` carries the same mechanical signature and describes the recovered token-group behavior, but it does not provide or claim a recovered historical BetaWise/vendor symbol. `syscall.c` likewise retains the neutral `SYS_A254` veneer.

Therefore A254 must be classified as:

`PUBLIC_COMPILATION_SURFACE / RECONSTRUCTED_NEUTRAL_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`

This is the same provenance distinction already established for A260-A270: presence in `os3k.h` demonstrates current SDK exposure, not historical public naming.

## Consequence

A254 is **not** evidence that another mechanically closed neutral `SYS_Axxx` should be promoted into `os3k.h`, and it must not be renamed to a descriptive BetaWise-style symbol without independent historical/public evidence.

Conversely, this documentation lot does not remove A254 from `os3k.h`: removal would be a source-compatibility/header change requiring consumer analysis and an m68k build gate.

The focused `file_token_group.h` remains ABI/emulator evidence and is not an integration header for `os3k.h`.

## Safety/build status

Documentation-only reconciliation. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` contract is promoted, and no source/header change is made. Nothing in this lot is newly `BUILD_VALIDATED`.