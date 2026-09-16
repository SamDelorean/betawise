# SDK audit: reconstructed-neutral publication policy conflict

## Material finding

The preserved BetaWise architecture rule conflicts with an earlier 2026 SDK-header reconciliation policy that exposed mechanically closed neutral `SYS_Axxx` contracts directly in `os3k.h` even when source-level/vendor identities were explicitly unknown.

Concrete examples now present on the public compilation surface include:

- A260/A264/A268/A26C/A270, while `applet_selection_mask.h` says original public/vendor symbols have not been recovered;
- NEO13-only A3B4/A3B8/A3BC/A3C0/A404/A41C/A438/A470, whose `os3k.h` section itself says source-level/vendor identities remain unknown.

The provenance is explicit for the NEO13 batch: commit `e6c20d9d5839ff5d2fd25373f32645cdbbb0ac54` (`sdk: expose closed NEO13 raw contracts`) added these neutral declarations to `os3k.h` and marked that header batch `BUILD_VALIDATED` after a clean `libos3k.a` rebuild and HelloWorld build/link.

That historical build validation proves the header compiled; it does **not** prove that publication as public BetaWise API was architecturally correct or historically sourced.

## Reconciliation rule

From this audit point forward, distinguish two independent axes:

1. **ABI/build validity** — the declaration mechanically matches the closed trap contract and compiles.
2. **public API provenance** — there is independent evidence that the symbol/contract belongs on the historical BetaWise public surface.

`BUILD_VALIDATED` must never be interpreted as satisfying axis 2.

Existing neutral declarations already in `os3k.h` are therefore grandfathered only as `PUBLIC_COMPILATION_SURFACE / RECONSTRUCTED_NEUTRAL_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`. They must not be used as precedent to expose additional closed `SYS_Axxx` contracts.

Do not remove existing declarations in a documentation-only audit batch. Removal is a source-compatibility change and must first inventory repository consumers, decide whether preserving reconstructed extensions is desirable, and run the m68k SDK build afterward. Such cleanup is `PENDING_M68K_BUILD` until executed and validated.

## Effect on prior documentation

Any earlier SDK audit text whose integration rule was effectively “closed raw contract => expose in os3k.h” is superseded as a publication rule. Its mechanical ABI findings and any genuine build results remain valid.

This does not alter closure states and does not demote previous build evidence; it only separates compilation success from historical/public API provenance.

## Safety/build status

Documentation-only policy reconciliation. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, no source/header change is made, and nothing is newly `BUILD_VALIDATED`.
