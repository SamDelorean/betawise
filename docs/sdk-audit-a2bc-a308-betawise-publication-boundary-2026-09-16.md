# SDK audit: A2BC-A308 BetaWise publication boundary (2026-09-16)

## Scope

Audit A2BC-A308 under the original BetaWise architecture, immediately after the intentional A2B8 `CallSysInt` raw/wrapper boundary.

## BetaWise source/history

At historical BetaWise commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df`, every veneer from A2BC through A308 remains a neutral `SYS_Axxx` symbol. Current `os3k/syscall.c` preserves the same naming across A2BC, A2C0, A2C4, A2C8, A2CC, A2D0, A2D4, A2D8, A2DC, A2E0, A2E4, A2E8, A2EC, A2F0, A2F4, A2F8, A2FC, A300, A304 and A308.

`os3k/os3k.h` does not publish this range. `os3k/os3k.c` has no wrapper/adaptation for it. The immediately preceding A2B8 demonstrates the opposite case: `_OS3K_CallSysInt` is a private raw veneer and `CallSysInt` is a public wrapper because BetaWise actually adds adaptation there.

## Conclusion

A2BC-A308 are not demonstrated public-header omissions. Closed mechanical ABI contracts in this range are emulator/reconstruction contracts unless separate BetaWise public provenance is recovered; they do not justify copying neutral prototypes into `os3k.h`.

Classification:

`CLOSED_MECHANICAL_ABI / BETAWISE_VENEER_NEUTRAL / NO_BETAWISE_WRAPPER / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN / NOT_PUBLICLY_PROMOTED`

## Consequences

- Keep A2BC-A308 outside `os3k.h` absent independent public provenance or a concrete SDK dependency.
- Do not invent descriptive vendor names.
- Do not treat reconstructed focused headers as publication authorities.
- Preserve A2B8 as the intentional neighboring raw-vs-wrapper positive control.

Documentation-only audit. No syscall is reopened, no limited-evidence entry is promoted, no m68k build is required, and no `BUILD_VALIDATED` status is created.
