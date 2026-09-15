# SDK audit — umbrella direct declarations vs dedicated subheaders (2026-09-15)

## Material finding

The pending umbrella-header exposure plan must be reconciled before adding `#include` directives.

Direct inspection of `os3k/os3k.h` on `sdk/abi-automation` shows that at least the A248/A24C/A250/A254/A258 closed contracts are **already publicly declared directly in the umbrella header**. The dedicated headers `password_runtime.h` and `file_token_group.h` repeat those declarations.

Therefore their absence as `#include`s from `os3k.h` is not, by itself, a visibility gap. Adding those includes would currently create duplicate declarations rather than expose previously invisible ABI.

## Safe consequence

Do not add `password_runtime.h` or `file_token_group.h` to `os3k.h` merely to expose A248–A258. First reconcile the broader candidate list against declarations already present directly in `os3k.h` and distinguish:

1. genuinely absent closed contracts;
2. contracts already declared directly in the umbrella header;
3. `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entries, which remain non-promotable.

No syscall is reopened and no evidence status is promoted by this finding.

## Build status

Documentation-only audit correction. No m68k build was executed and nothing in this lot is `BUILD_VALIDATED`.
