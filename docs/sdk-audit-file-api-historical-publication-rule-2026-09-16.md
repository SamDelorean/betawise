# SDK audit: File API historical publication rule (2026-09-16)

## Scope

Continue the SDK audit using only the original BetaWise architecture as the publication authority. Reconstructed focused headers are not used to decide public exposure.

## Historical evidence

At BetaWise filesystem research commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` (2022-02-14), `syscall.c` deliberately changed only selected entries in the contiguous A198-A228 filesystem region from neutral A-line names to descriptive source symbols:

- A198 -> `FileWriteBuffer`
- A19C -> `FileReadBuffer`
- A1C0 -> `FileSetFolder`
- A1C8 -> `FileOpen`
- A1CC -> `FileClose`
- A20C -> `ClipboardSet`
- A210 -> `ClipboardGet`
- A214 -> `ClipboardClear`

In that same research commit A1AC/A1B0/A1B4/A1B8/A1BC, A1D0-A1DC, A1E8-A208 and A218-A228 remained neutral. The commit's `os3k.h` change was unrelated to filesystem declarations and did not publish the newly named File/Clipboard veneers.

The historical public `os3k.h` at that commit also shows the original BetaWise pattern directly: named public declarations are placed directly in `os3k.h`; there is no reconstructed per-family header architecture to preserve or extend.

## Audit rule derived from BetaWise itself

For this filesystem region, a recovered/descriptive veneer name in historical BetaWise source is positive provenance evidence that can justify checking for a missing direct declaration in `os3k.h` once the ABI signature is mechanically closed. A neutral `SYS_Axxx` veneer is not, by itself, evidence for public exposure.

This rule is based on BetaWise source/history, not on any reconstructed `file_*.h`.

## Consequences for current audit

- A1AC-A1BC remain outside the public-header repair queue: BetaWise history left the veneers neutral and `os3k.c` has no wrapper for them.
- A1D0-A1DC, A1E8-A208 and A218-A228 must be judged the same way: their reconstructed headers cannot decide publication.
- A20C/A210/A214 remain a distinct case because the descriptive Clipboard symbols are present in BetaWise-era `syscall.c`; their missing public declarations may therefore be audited as a direct `os3k.h` consistency issue.
- Later reconstruction-era names (for example A1A0/A1A4/A1A8 naming work) must not be treated as historical BetaWise provenance merely because they are descriptive today.

No syscall is reopened by this rule and no ABI closure status changes.

Documentation-only audit; no m68k build is required and no `BUILD_VALIDATED` status is created.
