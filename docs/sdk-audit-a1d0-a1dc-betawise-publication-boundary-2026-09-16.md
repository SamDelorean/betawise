# SDK audit: A1D0-A1DC BetaWise publication boundary (2026-09-16)

## Scope

Re-evaluate A1D0/A1D4/A1D8/A1DC strictly from the original BetaWise source architecture and history. Reconstructed `file_*.h` material is not used as publication authority.

## BetaWise evidence

In the historical filesystem research commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` (2022-02-14), `syscall.c` selectively promoted several neighboring filesystem veneers to descriptive symbols (`FileWriteBuffer`, `FileReadBuffer`, `FileSetFolder`, `FileOpen`, `FileClose`, `ClipboardSet`, `ClipboardGet`, `ClipboardClear`). In the same contiguous block A1D0, A1D4, A1D8 and A1DC remained `SYS_A1D0`, `SYS_A1D4`, `SYS_A1D8` and `SYS_A1DC`.

That historical commit did not publish A1D0-A1DC in `os3k.h`. The current `syscall.c` still retains neutral A-line identities for the raw A1D0-A1DC veneers, and `os3k.c` has no corresponding wrapper/adaptation layer. The current `os3k.h` instead exposes the separately named higher-level password-dialog calls where descriptive identity has been established.

## Conclusion

The absence of raw A1D0-A1DC declarations from `os3k.h` is **not a demonstrated public-header defect**.

Classification:

`CLOSED_MECHANICAL_ABI / BETAWISE_VENEER_NEUTRAL / NO_BETAWISE_WRAPPER / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN / NOT_PUBLICLY_PROMOTED`

This conclusion is derived from BetaWise source/history. Any reconstructed password-state header remains secondary ABI evidence only and is not an integration instruction.

## Consequences

- Do not include a reconstructed A1D0-A1DC header from `os3k.h`.
- Do not copy neutral `SYS_A1D0`-`SYS_A1DC` prototypes into the public header merely because their mechanical contracts are closed.
- Do not infer historical vendor names from reconstructed behavior.
- Do not conflate these raw neutral veneers with the separately named public password-dialog APIs.
- Revisit public exposure only if independent BetaWise-era symbol evidence or a concrete SDK dependency is found.

No syscall is reopened and no closure classification changes. Documentation-only audit; no m68k build is required and no `BUILD_VALIDATED` status is created.
