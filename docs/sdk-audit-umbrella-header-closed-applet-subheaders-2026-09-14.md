# SDK audit — closed applet-context subheaders eligible for umbrella integration

Date: 2026-09-14
Branch: `sdk/abi-automation`

## Purpose

Continue the umbrella-header publication audit after the File API inventory in
`sdk-audit-umbrella-header-closed-subheaders-inventory-2026-09-14.md`.

This pass checks only dedicated applet-context headers whose canonical ABI index
already classifies the represented traps as mechanically closed. It does not
change ABI status, add declarations to `os3k.h`, or promote any
`CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry.

## Confirmed closed applet-context candidates

The following dedicated headers are currently isolated from `os3k/os3k.h` but
contain mechanically closed contracts already represented by veneers in
`os3k/syscall.c` and by the canonical ABI index:

- `applet_selection.h` — A22C, A230, A234 selected-SmartApplet context.
- `applet_selection_mask.h` — A260, A264, A268, A26C, A270 32-entry selection-block mask operations.
- `applet_settings.h` — A278 current-SmartApplet settings pointer query.

The headers deliberately retain neutral `SYS_Axxx` names where original modern
vendor symbols are not recovered. That naming boundary is preserved.

## Publication conclusion

These three headers form a second safe *candidate* tranche for umbrella exposure
on the same structural rule used for the closed File API headers: prefer
`#include` from `os3k.h` rather than duplicating prototypes.

This conclusion is limited to publication eligibility from the current closed
mechanical contracts. It does not authorize blanket inclusion of every dedicated
header under `os3k/`; password-runtime, token-group, printer, record, transport,
IrDA and other headers require their own publication-status pass.

## Build boundary

No `os3k.h` change is made in this audit pass. Adding these includes changes SDK
compile visibility and therefore remains pending validation with the actual m68k
toolchain. No m68k build was executed and no `BUILD_VALIDATED` claim is made.
