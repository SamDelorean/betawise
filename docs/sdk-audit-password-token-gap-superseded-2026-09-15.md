# SDK audit correction: password/token umbrella gap is superseded

Date: 2026-09-15
Branch: `sdk/abi-automation`

The earlier note `docs/sdk-audit-password-token-subheaders-umbrella-gap-2026-09-15.md` must **not** be used as an instruction to add `password_runtime.h` or `file_token_group.h` to `os3k/os3k.h`.

Direct inspection of the current umbrella header confirms that A248, A24C, A250, A254 and A258 are already declared publicly in `os3k/os3k.h`. Therefore absence of those two dedicated `#include`s is not a visibility gap; adding them merely for exposure would duplicate declarations already present in the umbrella.

This correction makes the safe rule explicit: every candidate dedicated header must first be reconciled against direct declarations already present in `os3k/os3k.h`. Only an actually missing public declaration belonging to a `CERRADA` contract is eligible for a later umbrella integration. `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entries are not to be promoted.

No header, prototype, veneer or syscall classification is changed by this correction. Any later umbrella visibility change remains `PENDING_M68K_BUILD` until compiled with the real m68k SDK toolchain and must not be marked `BUILD_VALIDATED` before that succeeds.

Related corrective audit: `docs/sdk-audit-umbrella-direct-declarations-reconcile-2026-09-15.md`.
