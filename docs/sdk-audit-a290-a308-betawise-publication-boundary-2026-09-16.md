# SDK audit: A290-A308 BetaWise publication boundary (2026-09-16)

## Scope

Audit A290-A308 under the original BetaWise architecture. Reconstructed per-family headers are not publication authorities.

## BetaWise source/history

The historical BetaWise filesystem-era source at commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` retains A290, A294, A298, A29C, A2A0, A2A4, A2A8, A2AC, A2B0 and A2B4 as neutral `SYS_Axxx` veneers. The current `os3k/syscall.c` preserves the same neutral names.

`os3k/os3k.h` does not publish this family. `os3k/os3k.c` contains no wrapper/adaptation for A290-A2B4; the next structurally significant entry is A2B8, whose raw veneer is `_OS3K_CallSysInt` and whose public BetaWise wrapper is `CallSysInt`.

This neighboring A2B8 case is useful architectural evidence: where BetaWise requires a public adaptation, the project preserves the private raw veneer in `syscall.c` and the wrapper in `os3k.c`/`os3k.h`. No analogous layering exists for A290-A2B4.

## Conclusion

A290-A2B4 are not demonstrated `os3k.h` omissions. Their closed mechanical contracts do not, by themselves, justify publication or invented descriptive names.

Classification:

`CLOSED_MECHANICAL_ABI / BETAWISE_VENEER_NEUTRAL / NO_BETAWISE_WRAPPER / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN / NOT_PUBLICLY_PROMOTED`

A2B8 is explicitly excluded from that classification: it has intentional raw-vs-wrapper layering (`_OS3K_CallSysInt` -> `CallSysInt`) and remains public through the wrapper.

## Consequences

- Do not add A290-A2B4 to `os3k.h` merely because their ABI contracts are closed.
- Do not invent vendor-style names.
- Do not use reconstructed focused headers to override this BetaWise source/history result.
- Preserve the existing A2B8 raw-return/public-wrapper distinction.
- Revisit A290-A2B4 only on independent BetaWise-era public-symbol evidence or a concrete SDK dependency.

Documentation-only audit. No syscall is reopened, no limited-evidence entry is promoted, no m68k build is required, and no `BUILD_VALIDATED` status is created.
