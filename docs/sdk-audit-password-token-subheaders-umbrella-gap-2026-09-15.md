# SDK audit: password/token-group closed subheaders and umbrella exposure

Date: 2026-09-15
Branch: `sdk/abi-automation`

## Material finding

The umbrella-header exposure inventory must include two additional dedicated headers whose declarations are already mechanically closed:

- `os3k/password_runtime.h`: A248, A24C, A250, A258.
- `os3k/file_token_group.h`: A254.

These headers preserve neutral `SYS_Axxx` names and do not require inventing vendor semantics. Their contracts are already represented as closed declarations; this audit does not reopen the syscalls.

## Safe integration boundary

A future umbrella reconciliation may add these headers to `os3k/os3k.h` together with the previously audited closed-only subheaders. Do not include any header solely on the basis of a `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry, and do not promote such entries.

Because changing `os3k.h` changes compilation visibility, the actual include integration remains **PENDING_M68K_BUILD** until tested with the real m68k SDK toolchain. It must not be marked `BUILD_VALIDATED` before that build succeeds.

No ABI prototype, veneer, syscall classification, or public name is changed by this documentation-only audit record.
