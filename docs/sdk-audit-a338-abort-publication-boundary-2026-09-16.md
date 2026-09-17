# SDK audit: A338 `abort` publication boundary (2026-09-16)

## Scope

Audit A338 under the original BetaWise source structure. Reconstructed auxiliary headers are not publication authority.

## Evidence

Current `os3k/syscall.c` names trap A338 directly as `abort`, and current `os3k/os3k.h` declares `void abort(void)` in the standard-library surface. There is no BetaWise wrapper for A338 in `os3k.c`.

The canonical continuation index classifies A338 as `CERRADA_CON_LÍMITE_DE_EVIDENCIA`: abort-like termination mechanics are closed, but the exact vendor C prototype is not independently demonstrated. The same index explicitly states that the existing `abort(void)` declaration is compatibility surface, not evidence of a recovered vendor prototype.

## Conclusion

This is **not a safe header-repair candidate**.

The existing BetaWise compatibility declaration is retained unchanged because removing or changing it would be a compatibility/API change, while the evidence grade explicitly forbids strengthening the inferred prototype into a recovered ABI claim.

Classification for SDK audit purposes:

`EXISTING_BETAWISE_COMPATIBILITY_SURFACE / CERRADA_CON_LÍMITE_DE_EVIDENCIA / DO_NOT_PROMOTE / DO_NOT_REWRITE_WITHOUT_DEPENDENCY`

Consequences:

- do not treat `void abort(void)` as newly ABI-validated;
- do not use A338 as precedent for exposing other limited-evidence traps;
- do not remove it merely to make the umbrella header mirror reconstruction status;
- reopen only if a concrete dependency requires an exact contract and independent evidence resolves the source-level prototype.

Documentation-only audit. No m68k build is required, and no `BUILD_VALIDATED` status is created.
