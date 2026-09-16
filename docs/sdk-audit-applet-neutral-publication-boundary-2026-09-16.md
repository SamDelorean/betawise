# SDK audit: neutral SmartApplet publication boundary

## Result

The pending reconstructed SmartApplet contracts A22C/A230/A234 (`applet_selection.h`) and A278 (`applet_settings.h`) are mechanically closed but do not have recovered reliable original public/vendor symbols.

Both focused headers state that limitation explicitly and intentionally retain neutral `SYS_Axxx` names. `syscall.c` likewise exports these slots as `SYS_A22C`, `SYS_A230`, `SYS_A234`, and `SYS_A278`.

This is materially different from the immediately adjacent historically named direct veneers `AppletFindByName`, `AppletFindById`, `AppletGetName`, and `AppletSendMessage`, which are represented directly in `os3k.h`.

## Architectural consequence

Under the preserved BetaWise SDK architecture, absence of A22C/A230/A234/A278 from `os3k.h` is **not currently a demonstrated public-header inconsistency**. Mechanical closure alone does not justify publication of neutral reverse-engineering names.

Do not include `applet_selection.h` or `applet_settings.h` from `os3k.h`, do not copy their neutral prototypes into the public header merely because the calls are closed, and do not invent descriptive BetaWise-style names.

Revisit public exposure only if independent evidence establishes public identity or a concrete SDK dependency: historical declaration, symbol/import table, original applet consumer, or equivalent evidence.

This resolves these two earlier umbrella-header candidates while preserving their focused headers as ABI/emulator evidence.

## Safety/build status

Documentation-only reconciliation. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, and no source/header change is made. No m68k build is required for this documentation lot and nothing is newly `BUILD_VALIDATED`.
