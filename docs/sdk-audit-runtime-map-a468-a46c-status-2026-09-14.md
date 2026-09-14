# SDK audit: stale A468/A46C status wording in runtime map

Status: DOCUMENTATION_INCONSISTENCY_CONFIRMED / NO_ABI_CHANGE / NO_BUILD_REQUIRED

## Finding

`docs/os3k-runtime-memory-storage.md` currently says:

> Nothing here promotes A468/A46C or another limited-evidence entry.

That wording is stale and conflicts with the completed late-range SDK audit.
`docs/sdk-audit-a3b4-a470-2026-09-12.md` explicitly records both A468 and A46C as
`CERRADA` in the consolidated inventory. They are intentionally absent from the
public umbrella header because there is no current SDK consumer requirement,
not because their callable contracts remain evidence-limited.

## Correct interpretation

- A468: `CERRADA`; intentionally private/not exposed in `os3k.h`.
- A46C: `CERRADA`; intentionally private/not exposed in `os3k.h`.
- The evidence-limited entries in this late NEO13 region remain A3C4-A400 except
  A404, A408-A418, A420-A434, and A43C-A454.
- A458/A45C/A460/A464 remain `NO_SYSCALL/NO_CALLABLE`.

The stale runtime-map sentence must not be used to downgrade A468/A46C or to
change the consolidated inventory.

## Recommended textual correction

Replace the stale sentence with wording equivalent to:

> Neutral `SYS_Axxx` names, unknown flag names and unknown return meanings stay
> unchanged. This map does not promote any `CERRADA_CON_LÍMITE_DE_EVIDENCIA`
> entry. A468/A46C are closed contracts but remain intentionally private from
> the umbrella SDK surface.

## Scope

This is documentation reconciliation only. It does not alter `os3k.h`,
`syscall.c`, trap veneers, ABI classifications, or publication policy. No m68k
build is required and no `BUILD_VALIDATED` claim is made.
