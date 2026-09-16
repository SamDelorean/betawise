# SDK audit: neutral clipboard publication boundary (A200-A208, A218-A228)

## Result

The clipboard family now has a clean public-identity boundary.

A20C/A210/A214 are the exceptional subfamily with independently pre-existing names in `syscall.c` (`ClipboardSet`, `ClipboardGet`, `ClipboardClear`) and are tracked separately as a narrow missing-public-declaration candidate.

By contrast, A200/A204/A208 and A218/A21C/A220/A224/A228 remain neutral reconstructed contracts:

- `file_clipboard_edit.h` explicitly states that the functional roles are mechanically closed but original public System 3 names and enum labels have not been recovered, so `SYS_Axxx` names are retained.
- `file_clipboard_state.h` likewise states that mechanical contracts are closed across the compared firmware families but reliable original public names have not been recovered.
- `syscall.c` preserves `SYS_A200`, `SYS_A204`, `SYS_A208` and `SYS_A218` through `SYS_A228`, while the immediately adjacent A20C/A210/A214 slots carry the independently identified Clipboard names.
- the 2022 filesystem-research commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` made exactly that distinction: it renamed only A20C/A210/A214 to `ClipboardSet/Get/Clear` and left the surrounding clipboard slots neutral.

This adjacency is useful negative evidence for publication provenance: it does not prove the neutral calls were never vendor-public, but it prevents treating family membership or mechanical closure as sufficient evidence of historical public identity.

## Architectural consequence

Under the preserved BetaWise SDK architecture, the absence of A200/A204/A208 and A218-A228 from `os3k.h` is **not currently a demonstrated public-header defect**.

Do not include `file_clipboard_edit.h` or `file_clipboard_state.h` from `os3k.h`, do not copy their neutral prototypes into the public header merely because they are closed, and do not infer vendor-style names from `ClipboardSet/Get/Clear`.

The public-declaration candidate remains narrowly A20C/A210/A214. Revisit the neutral neighbors only with independent evidence such as a historical declaration, symbol table, applet import/consumer, or equivalent source.

This resolves the earlier umbrella/publication candidates for the neutral clipboard subfamilies without changing ABI closure.

## Safety/build status

Documentation-only reconciliation. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, and no source/header change is made. No m68k build is required for this documentation lot and nothing is newly `BUILD_VALIDATED`.
