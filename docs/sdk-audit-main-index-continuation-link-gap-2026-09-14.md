# SDK audit — canonical ABI index continuation link gap (2026-09-14)

## Finding

`docs/abi-reconstruction-index.md` identifies itself as the canonical navigation and traceability index, but its `Current reconstructed blocks` table still ends at A28C and contains no reference to the newly added `docs/abi-reconstruction-index-a290-a470.md` continuation.

The continuation itself correctly states that it is the canonical navigation continuation after A28C and preserves the status boundary among `CERRADA`, `CERRADA_CON_LÍMITE_DE_EVIDENCIA`, and `NO_SYSCALL/NO_CALLABLE` through A470. Repository search also finds no other discoverability link to the continuation by filename.

## Impact

This is a documentation-navigation inconsistency only. It does not change any ABI classification, public prototype, veneer, or implementation. A developer following only the canonical index can still incorrectly conclude that indexed coverage stops at A28C even though the authoritative consolidated inventory covers A000–A470 with zero `PENDIENTE_REAL`.

## Safe correction

Add an explicit navigation row or short continuation paragraph immediately after A28C in `docs/abi-reconstruction-index.md`, pointing to [`abi-reconstruction-index-a290-a470.md`](abi-reconstruction-index-a290-a470.md). The link should state that the continuation preserves evidence-limit classifications and is not itself a public-SDK promotion list.

No per-entry contracts need to be duplicated into the original index.

## Evidence boundary

No syscall is reopened. No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted. No source/header/veneer change is proposed by this finding.

## Build-validation boundary

Documentation-only finding. No m68k build was executed or required, and nothing in this audit is newly labeled `BUILD_VALIDATED`.
