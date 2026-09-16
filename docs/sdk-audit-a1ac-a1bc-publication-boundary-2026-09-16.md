# SDK audit: A1AC-A1BC publication boundary (2026-09-16)

## Status

**REDONE under the original BetaWise structure rule.**

The earlier wording gave the reconstructed `file_space_accounting.h` too much architectural weight. That header is not part of the historical BetaWise public-header architecture and cannot decide publication. It may be consulted only as secondary mechanical ABI evidence.

## Authoritative cross-audit

Publication is evaluated from the BetaWise source structure and history:

`historical BetaWise source/history ↔ syscall.c veneer ↔ os3k.c wrapper (if any) ↔ os3k.h public surface`

The current `os3k/syscall.c` exports A1AC, A1B0, A1B4, A1B8 and A1BC only under neutral `SYS_Axxx` names. A1B4 and A1B8 still carry comments indicating unresolved selector/command naming.

`os3k/os3k.c` contains no wrapper or adaptation for this A1AC-A1BC family.

The historical BetaWise filesystem research commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` is the decisive provenance evidence for this lot. In the same contiguous filesystem block it deliberately recovered/promoted A198/A19C as `FileWriteBuffer`/`FileReadBuffer`, A1C0 as `FileSetFolder`, A1C8/A1CC as `FileOpen`/`FileClose`, and A20C/A210/A214 as `ClipboardSet`/`ClipboardGet`/`ClipboardClear`, while A1AC, A1B0, A1B4, A1B8 and A1BC remained neutral. The historical commit did not add public declarations for this family to `os3k.h`.

The current `os3k/os3k.h` likewise has no A1AC-A1BC declarations.

## Corrected conclusion

The absence of A1AC-A1BC from `os3k.h` is **not currently a demonstrated public-header defect**, but this conclusion now rests on BetaWise source/history, not on the reconstructed focused header.

Classification:

`CLOSED_MECHANICAL_ABI / BETAWISE_VENEER_NEUTRAL / NO_BETAWISE_WRAPPER / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN / NOT_PUBLICLY_PROMOTED`

The reconstructed `file_space_accounting.h` is explicitly non-authoritative for publication. Its signatures/comments remain useful only as ABI reconstruction evidence.

## Consequences

- Do not include `file_space_accounting.h` from `os3k.h`.
- Do not copy neutral A1AC-A1BC prototypes into `os3k.h` merely because the mechanical ABI is closed.
- Do not invent vendor-style names.
- A1B4's established mechanical failure behavior remains unchanged; callers must not assume every failure returns zero.
- Reconsider publication only if independent BetaWise-era evidence or a concrete SDK dependency appears.
- This audit does not reopen any syscall and does not alter ABI closure status.

Documentation-only correction; no m68k build is required and no `BUILD_VALIDATED` status is created.
