# SDK audit: A260-A270 neutral names already on the public surface

## Material inconsistency

A260/A264/A268/A26C/A270 are not a missing-publication case. They are already declared directly in `os3k.h`, with the same mechanical signatures carried by `applet_selection_mask.h`, and `syscall.c` exports the corresponding neutral `SYS_Axxx` veneers.

However, `applet_selection_mask.h` explicitly states that original public/vendor symbols have **not** been recovered and that the generic A-line names are intentionally retained. This means the direct declarations currently present in `os3k.h` must not be described as recovered historical BetaWise API names. They are reconstructed neutral ABI contracts that have already been placed on the public compilation surface.

This is a different class from both:

- A20C/A210/A214, where independently pre-existing named veneers support a narrow missing-public-declaration candidate; and
- neutral reconstructed families such as A1E8-A1FC or A22C/A230/A234, which remain outside `os3k.h` because no historical public identity has been established.

## Safe reconciliation

Do **not** remove A260-A270 from `os3k.h` in this documentation lot: that would be a source-compatibility/header change and requires an explicit integration decision plus m68k build validation. Do **not** rename them or invent vendor symbols.

For continuity, classify them as:

`PUBLIC_COMPILATION_SURFACE / RECONSTRUCTED_NEUTRAL_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`

Their presence in `os3k.h` is therefore not evidence that other closed `SYS_Axxx` contracts should be promoted there. Any future cleanup/removal is `PENDING_M68K_BUILD` and should first check repository consumers and source compatibility.

## Safety/build status

Documentation-only classification. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, and no source/header change is made. Nothing is newly `BUILD_VALIDATED`.
