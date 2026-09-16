# SDK audit: A1AC-A1BC publication boundary (2026-09-16)

## Scope

Cross-audit the closed file space/accounting family A1AC-A1BC against the current BetaWise SDK publication surface, without reopening the closed ABI contracts.

## Evidence

`os3k/file_space_accounting.h` explicitly records that the original public/vendor symbols have not been recovered and therefore deliberately retains neutral A-line names for A1AC, A1B0, A1B4, A1B8 and A1BC.

`os3k/syscall.c` likewise exports this family only as `SYS_A1AC`, `SYS_A1B0`, `SYS_A1B4`, `SYS_A1B8` and `SYS_A1BC`; comments on A1B4/A1B8 explicitly leave selector/command names pending.

The current `os3k/os3k.h` does not publish this family. This differs from nearby File API calls whose recovered/descriptive names are already represented directly in the public header.

## Classification

The absence of A1AC-A1BC from `os3k.h` is **not currently a demonstrated public-header defect**.

Classification:

`CLOSED_MECHANICAL_ABI / RECONSTRUCTED_NEUTRAL_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN / NOT_PUBLICLY_PROMOTED`

Mechanical closure is not sufficient evidence to promote neutral `SYS_Axxx` contracts into the historical BetaWise public API. The focused reconstructed header remains an evidence/contract artifact, not an instruction to include it from `os3k.h`.

## Consequences

- Do not include `file_space_accounting.h` from `os3k.h` merely because these contracts are closed.
- Do not copy the neutral prototypes into `os3k.h` without independent evidence of intended public SDK exposure.
- Do not invent vendor-style names for A1AC-A1BC.
- A1B4's documented failure behavior remains unchanged: callers must not assume every failure returns zero.
- This audit does not reopen any syscall and does not alter ABI status.

Documentation-only audit; no m68k build was required and no `BUILD_VALIDATED` status is created.