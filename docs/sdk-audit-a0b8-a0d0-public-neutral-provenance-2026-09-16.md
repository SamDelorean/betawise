# SDK audit: A0B8-A0D0 public neutral provenance

## Material reconciliation

Commit `558adf452e3ad94d0fc0f46ea082b3a5307b2ace` added seven closed raw contracts directly to `os3k.h`:

- `SYS_A0B8`
- `SYS_A0BC`
- `SYS_A0C0`
- `SYS_A0C4`
- `SYS_A0C8`
- `SYS_A0CC`
- `SYS_A0D0`

The commit itself explicitly described them as closed raw OS3K contracts whose higher-level vendor semantics remained unassigned. It also recorded a clean `libos3k.a` rebuild and HelloWorld build/link, so that historical `BUILD_VALIDATED` status remains valid for the header/build change.

What that validation does **not** establish is historical BetaWise/public-symbol provenance. The neutral `SYS_Axxx` spelling and the commit's own "vendor semantics remain unassigned" statement make these reconstructed publication names rather than recovered vendor identities.

Accordingly classify A0B8-A0D0 as:

`PUBLIC_COMPILATION_SURFACE / RECONSTRUCTED_NEUTRAL_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`

while independently retaining their existing mechanical ABI closure and historical build-validation record.

## Architectural consequence

A0B8-A0D0 must not be used as precedent for the rule "closed ABI implies public os3k.h declaration". They are evidence of an earlier SDK-reconciliation policy that exposed neutral raw contracts, not evidence that the original BetaWise SDK did so.

Do not remove or rename them in this documentation lot. That would alter the current public compilation surface. A future cleanup requires a repository-consumer/source-compatibility audit and an actual m68k rebuild; until then such a change is `PENDING_M68K_BUILD`.

This finding does not invalidate the recorded build: build validity and public-symbol provenance are separate dimensions.

## Safety/build status

Documentation-only reconciliation. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, no source/header change is made, and no new `BUILD_VALIDATED` claim is created.
