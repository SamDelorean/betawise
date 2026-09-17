# SDK audit: A1E8-A208 BetaWise publication boundary (2026-09-16)

## Scope

Re-evaluate A1E8/A1EC/A1F0/A1F4/A1F8/A1FC/A200/A204/A208 strictly from original BetaWise source/history and layering. Reconstructed `file_*.h` files are not publication authority.

## BetaWise source/history

The historical filesystem research commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` selectively renamed neighboring filesystem veneers but left A1E8-A208 neutral. In particular, it promoted A20C/A210/A214 immediately after this family to `ClipboardSet`/`ClipboardGet`/`ClipboardClear` while A200/A204/A208 remained `SYS_A200`/`SYS_A204`/`SYS_A208`.

Current `syscall.c` preserves the same publication distinction: A1E8-A208 remain neutral veneers, followed by the named Clipboard A20C-A214 veneers. Current `os3k.c` contains no wrapper/adaptation for A1E8-A208. Current `os3k.h` does not publish A1E8-A208.

## Conclusion

There is no demonstrated BetaWise public-header defect for A1E8-A208.

Classification:

`CLOSED_MECHANICAL_ABI / BETAWISE_VENEER_NEUTRAL / NO_BETAWISE_WRAPPER / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN / NOT_PUBLICLY_PROMOTED`

The distinction at A200-A214 is particularly useful: BetaWise history itself separates the neutral A200/A204/A208 veneers from the descriptively identified A20C/A210/A214 Clipboard calls. Mechanical adjacency and related behavior therefore do not justify promoting A200-A208.

## Consequences

- Keep A1E8-A208 outside `os3k.h` unless independent BetaWise-era symbol evidence or a concrete SDK dependency appears.
- Do not include reconstructed focused headers from `os3k.h`.
- Do not invent descriptive names from reconstructed behavior.
- Preserve the separate A20C/A210/A214 direct-declaration repair path; it has historical BetaWise name provenance that A200-A208 lack.

No syscall is reopened and no closure classification changes. Documentation-only audit; no m68k build is required and no `BUILD_VALIDATED` status is created.
