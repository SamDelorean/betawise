# SDK audit: A248-A258 neutral file-password/token public surface

## Material classification

A248/A24C/A250/A254/A258 are already declared directly in `os3k.h` under neutral `SYS_Axxx` names. A254 is also carried by the focused reconstruction header `file_token_group.h`; the public header documents the same byte-sized token-group contract.

The surrounding reconstruction evidence establishes an important provenance boundary:

- `file_password_state.h` explicitly says the adjacent A1D0-A1DC file-password/state interfaces retain A-line names because reliable original System 3 public symbols have not been recovered.
- `file_space_accounting.h` uses the same explicit neutral-name rule for A1AC-A1BC.
- `file_token_group.h` documents A254 mechanically but supplies no recovered vendor/public symbol.
- `syscall.c` exports A248-A258 as neutral `SYS_Axxx` veneers rather than historically identified names.

Accordingly, the current direct declarations of A248/A24C/A250/A254/A258 in `os3k.h` must not be treated as evidence that historical BetaWise public names have been recovered. They belong to the same compatibility class already identified for A260-A270:

`PUBLIC_COMPILATION_SURFACE / RECONSTRUCTED_NEUTRAL_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`

This classification concerns provenance/publication, not mechanical ABI closure.

## Architectural consequence

Do not use A248-A258 as precedent for promoting other closed neutral `SYS_Axxx` calls into `os3k.h`. Do not invent descriptive vendor-style replacements.

Do not remove or rename these existing declarations in this documentation lot. Such a change would affect the current public compilation surface and requires an explicit compatibility decision, repository-consumer audit, and m68k build. Any future source/header cleanup is therefore `PENDING_M68K_BUILD` until that validation is actually performed.

## Safety/build status

Documentation-only reconciliation. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, no source/header change is made, and nothing is newly `BUILD_VALIDATED`.
