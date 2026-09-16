# SDK audit: A1A0/A1A8 name provenance

## Material finding

The current public names `FileSmashFile` (A1A0) and `FileGetCurrentFile` (A1A8) must not be treated as recovered historical BetaWise symbol names on the evidence presently in the repository.

Git history shows commit `9f474207db34685d96e57753bee88dbc7d7dc6e2` (2026-08-26, `sdk: name FileSmashFile and FileGetCurrentFile`) changed the `syscall.c` veneers from neutral names to these descriptive names. Immediately before that change, A1A0 carried the explicit comment `historical-name correlation pending`, while A1A8 was still `SYS_A1A8`. The commit changes only `syscall.c`; it supplies no independent historical-header/source provenance for either spelling.

The present branch also declares both names directly in `os3k.h`. Their mechanical ABI/behavior may remain closed and useful, but **name provenance and ABI closure are separate claims**.

## Safe classification

- Do not reopen the closed A1A0/A1A8 mechanical contracts on this basis.
- Do not relabel either syscall `CERRADA_CON_LÍMITE_DE_EVIDENCIA` merely because the public-name provenance is weaker; closure status and historical symbol identity are separate dimensions.
- Until independent BetaWise source/header evidence is found, document `FileSmashFile` and `FileGetCurrentFile` as reconstruction-assigned/descriptive SDK names, not recovered vendor names.
- Do not use these two names as structural controls for deciding whether another neutral `SYS_Axxx` belongs in the historical public SDK.

This differs from A20C/A210/A214, whose `ClipboardSet`/`ClipboardGet`/`ClipboardClear` names are traceable to the pre-2026 BetaWise history.

## Follow-up

A later compatibility decision may choose to retain reconstruction-assigned names in the extended SDK, but that decision must be explicit and must not be represented as preservation of an original BetaWise declaration.

No source/header change is made in this lot. No m68k build is required for this documentation correction, and no new state is `BUILD_VALIDATED`.
