# SDK audit: A218-A234 BetaWise publication boundary (2026-09-16)

## Scope

Audit A218/A21C/A220/A224/A228/A22C/A230/A234 using original BetaWise source/history and wrapper structure only for publication decisions.

## Evidence

Historical BetaWise filesystem research `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` named A20C/A210/A214 as `ClipboardSet`/`ClipboardGet`/`ClipboardClear`, but left the immediately following A218-A234 range neutral. Current `syscall.c` still exports A218-A234 as `SYS_A218` through `SYS_A234`, after which A238-A244 are descriptively named `AppletFindByName`, `AppletFindById`, `AppletGetName` and `AppletSendMessage`.

Current `os3k.c` has no wrapper/adaptation layer for A218-A234. Current `os3k.h` does not publish A218-A234, while it directly declares the named A238-A244 Applet APIs.

## Conclusion

The absence of A218-A234 from `os3k.h` is not a demonstrated public-header inconsistency.

Classification:

`CLOSED_MECHANICAL_ABI / BETAWISE_VENEER_NEUTRAL / NO_BETAWISE_WRAPPER / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN / NOT_PUBLICLY_PROMOTED`

This conclusion supersedes any publication inference based solely on reconstructed Clipboard-state or SmartApplet focused headers. Those files remain secondary ABI evidence, not public-header architecture.

## Consequences

- Do not promote A218-A234 into `os3k.h` merely because their mechanical contracts are closed.
- Do not include reconstructed focused headers from `os3k.h`.
- Do not infer names from adjacency to the named Clipboard or Applet APIs.
- A238-A244 remain the positive BetaWise-style comparison: named direct veneers with direct public declarations.
- Revisit A218-A234 only with independent BetaWise-era public identity evidence or a concrete SDK dependency.

No syscall is reopened and no closure classification changes. Documentation-only audit; no m68k build is required and no `BUILD_VALIDATED` status is created.
