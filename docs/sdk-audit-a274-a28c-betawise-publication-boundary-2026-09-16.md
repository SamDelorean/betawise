# SDK audit: A274-A28C BetaWise publication boundary (2026-09-16)

## Scope

Audit A274, A278, A27C, A280, A284, A288 and A28C strictly under the original BetaWise structure rule. Reconstructed per-family headers are not publication authorities.

## BetaWise source/history cross-audit

Historical BetaWise filesystem/applet research commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` keeps all seven veneers neutral in `syscall.c`:

- A274 -> `SYS_A274`
- A278 -> `SYS_A278`
- A27C -> `SYS_A27C`
- A280 -> `SYS_A280`
- A284 -> `SYS_A284`
- A288 -> `SYS_A288`
- A28C -> `SYS_A28C`

This is significant because the same historical source already uses recovered/descriptive names for adjacent A238-A244 (`AppletFindByName`, `AppletFindById`, `AppletGetName`, `AppletSendMessage`). The A274-A28C family therefore has no equivalent positive BetaWise-era symbol provenance.

Current `os3k/syscall.c` preserves A274-A28C as the same neutral `SYS_Axxx` veneers. Current `os3k/os3k.h` stops its directly exposed neutral SmartApplet mask family at A270 and does not declare A274-A28C. `os3k/os3k.c` provides no wrapper/adaptation for A274-A28C.

## Conclusion

The absence of A274-A28C from `os3k.h` is **not a demonstrated public-header inconsistency** under the BetaWise architecture.

Classification:

`CLOSED_ABI_WHERE_ALREADY_ESTABLISHED / BETAWISE_VENEER_NEUTRAL / NO_BETAWISE_WRAPPER / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN / NOT_PUBLICLY_PROMOTED`

This document does not change the established mechanical closure class of any member. In particular, it does not promote any `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry.

## Consequences

- Do not copy A274-A28C declarations into `os3k.h` solely from reconstructed focused headers.
- Do not invent descriptive/vendor names.
- Existing direct exposure of A260-A270 is not sufficient precedent to extend the public surface: publication must be established per contract from BetaWise evidence.
- Revisit an individual entry only if a concrete SDK dependency or independent BetaWise-era public identity appears.

Documentation-only audit; no m68k build is required and no `BUILD_VALIDATED` status is created.
