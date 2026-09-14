# System 3 / OS3K ABI reconstruction index — continuation A290–A470

This document is the canonical navigation continuation for `abi-reconstruction-index.md` after A28C. It does not reopen ABI discovery and does not promote any limited-evidence contract. The authoritative status inventory remains `docs/os3k-abi-consolidation-2026-09-11.md` and the Drive sheet `ABI consolidada 2026-09-11`.

## Status policy

- `CERRADA`: mechanically closed contract. Public SDK exposure is a separate decision.
- `CERRADA_CON_LÍMITE_DE_EVIDENCIA`: mechanics are reconstructed but an exact source-level C contract, return meaning, vendor identity, or private semantic property is not demonstrable. These entries must not be promoted merely to fill the public header.
- `NO_SYSCALL/NO_CALLABLE`: no callable A-line contract is established.

No entry in A290–A470 is `PENDIENTE_REAL`.

## Navigation by range

| Range | Canonical status/navigation |
| --- | --- |
| A290–A2AC | All `CERRADA`. Neutral `SYS_Axxx` identities are retained. A298–A2AC have source-first mechanically closed contracts; see the corresponding `sys-a29x`/transport closure notes and auxiliary headers under `os3k/`. |
| A2B0 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; return contract not demonstrable. Do not publish a guessed `void` prototype. |
| A2B4–A2E0 | `CERRADA`, including A2B8 `_OS3K_CallSysInt`. These are eligible for SDK reconciliation only where the exact auxiliary-header contract is already present. |
| A2E4 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; mechanical two-pointer copy/init behavior is known but the vendor return contract is not. |
| A2E8 | `CERRADA`. |
| A2EC | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; callable mechanics reconstructed, source-level return contract unresolved. |
| A2F0–A2F4 | `CERRADA`. |
| A2F8 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`. |
| A2FC | `CERRADA`. |
| A300 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; `void` remains only a strong inference. |
| A304 | `CERRADA`; static regression recorded PASS. |
| A308 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; return contract unresolved. |
| A30C–A328 | `NO_SYSCALL/NO_CALLABLE`; cross-ROM null A-line vectors. |
| A32C | `CERRADA`; `uint32_t SYS_A32C(void)`, already SDK reconciled and build-validated. |
| A330 | `CERRADA`; raw `_OS3K_getchar` with public `getchar` wrapper. |
| A334 | `CERRADA`; `int32_t SYS_A334(void)`, already SDK reconciled and build-validated. |
| A338 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; abort-like termination mechanics are closed, but exact vendor C prototype is not demonstrated. Existing `abort(void)` is compatibility surface, not evidence of a recovered vendor prototype. |
| A33C–A3B0 | `CERRADA`; reconstructed C-library block (`atoi` through `ungetc`). |
| A3B4–A3C0 | `CERRADA`, NEO13-only; public neutral contracts already SDK reconciled and build-validated. |
| A3C4–A400 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; NEO13 private/hardware/state helpers. Do not strengthen into public prototypes. |
| A404 | `CERRADA`, NEO13-only; public neutral contract already SDK reconciled and build-validated. |
| A408–A418 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; mechanics known, return/source-level contracts remain limited. |
| A41C | `CERRADA`, NEO13-only; `void *SYS_A41C(uint8_t selector)`, already SDK reconciled and build-validated. |
| A420–A434 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; private wrapper/record/helper mechanics known without a complete public source-level contract. |
| A438 | `CERRADA`, NEO13-only; `char *SYS_A438(char *dst)`, already SDK reconciled and build-validated. |
| A43C–A454 | `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; aggregate/A1-result and private descriptor semantics remain intentionally limited. |
| A458–A464 | `NO_SYSCALL/NO_CALLABLE`; ABI holes/non-pointer slots. |
| A468 | `CERRADA`, NEO13-only bounded circular-queue append; mechanically closed but intentionally not added to the umbrella header absent a real SDK consumer. |
| A46C | `CERRADA`, NEO13-only state dispatcher; mechanically closed but intentionally not added to the umbrella header absent a real SDK consumer. |
| A470 | `CERRADA`, NEO13-only; `uint16_t SYS_A470(void)`, already SDK reconciled and build-validated. |

## Public-SDK boundary

A mechanically closed entry is not automatically a public SDK declaration. The late-NEO batch intentionally exposes only contracts whose exact C argument/return representation is closed and whose publication has been explicitly audited. Conversely, `CERRADA_CON_LÍMITE_DE_EVIDENCIA` is not an invitation to choose the most plausible prototype.

A468 and A46C are specifically classified `CERRADA`; their current non-exposure is a publication-policy choice, not an evidence-limit classification.

## Build-validation boundary

This continuation is documentation-only. It records prior build-validation where already documented by `os3k-abi-consolidation-2026-09-11.md`; it does not claim that any new header batch was compiled during this audit. Any future header integration from A290–A304 must run the m68k build gates before being labeled `BUILD_VALIDATED`.

## Traceability

The detailed per-entry contracts and evidence grades are maintained in the Drive sheet `Índice comparativo BetaWise – ABI OS3K`, tab `ABI consolidada 2026-09-11`, rows A290–A470, and in focused closure/trace documents in this repository. This continuation exists to restore navigation coverage after the original index stopped at A28C without duplicating every private mechanical trace into one oversized table.
