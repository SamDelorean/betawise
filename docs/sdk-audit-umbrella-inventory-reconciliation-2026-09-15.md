# SDK audit — umbrella inventory reconciliation

Date: 2026-09-15
Branch: `sdk/abi-automation`

## Material finding

The current umbrella-integration inventory must not be treated as authorization to add all listed dedicated headers to `os3k/os3k.h` in one mechanical change.

The later password/token audit already demonstrated that presence of a dedicated reconstruction header does not imply absence from the umbrella: A248/A24C/A250/A254/A258 are already declared directly in `os3k.h`. That finding superseded the proposed password/token includes.

The same reconciliation rule now applies to the earlier File API and applet candidate inventories: each candidate declaration must be checked individually against direct declarations already present in `os3k.h` before any `#include` is added. The safe publication unit is the missing declaration/contract, not the dedicated header as a whole.

## Confirmed still-isolated example

`file_password_state.h` declares the closed A1D0/A1D4/A1D8/A1DC contracts under neutral `SYS_Axxx` names. They are not part of the direct File API declarations visible in the current `os3k.h` section inspected in this audit. This remains a valid candidate set, subject to the build boundary below.

No status is changed by this note. In particular, `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entries remain non-promotable and closed syscalls are not reopened.

## Integration/build boundary

Do not modify `os3k.h` until the remaining candidate inventory has been reconciled declaration-by-declaration. Any subsequent umbrella visibility change must be recorded as `PENDING_M68K_BUILD` until compiled with the actual m68k SDK toolchain. Do not mark it `BUILD_VALIDATED` from static inspection alone.

No m68k build was executed in this audit pass and no `BUILD_VALIDATED` claim is made.
