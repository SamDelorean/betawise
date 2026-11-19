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

## Per-trap closure links

Added in the 2026-11-19 consolidation for traceability. The range table above remains the status authority; this table only links each trap to its closure/trace documents.

| Trap | Closure / trace documents |
| --- | --- |
| A290 | [`sys-a290-closure.md`](sys-a290-closure.md) |
| A294 | [`sys-a294-closure.md`](sys-a294-closure.md) |
| A298 | [`sys-a298-closure.md`](sys-a298-closure.md) |
| A29C | [`sys-a29c-closure.md`](sys-a29c-closure.md) |
| A2A0 | [`sys-a2a0-closure.md`](sys-a2a0-closure.md) |
| A2A4 | [`sys-a2a4-closure.md`](sys-a2a4-closure.md) |
| A2A8 | [`sys-a2a8-closure.md`](sys-a2a8-closure.md) |
| A2AC | [`sys-a2ac-closure.md`](sys-a2ac-closure.md) |
| A2B4 | [`sys-a2b4-closure.md`](sys-a2b4-closure.md) |
| A2B8 | [`sys-a2b8-closure.md`](sys-a2b8-closure.md) |
| A2BC | [`sys-a2bc-closure.md`](sys-a2bc-closure.md) |
| A2C0 | [`sys-a2c0-closure.md`](sys-a2c0-closure.md) |
| A2C4 | [`sys-a2c4-closure.md`](sys-a2c4-closure.md) |
| A2C8 | [`sys-a2c8-closure.md`](sys-a2c8-closure.md) |
| A2CC | [`sys-a2cc-closure.md`](sys-a2cc-closure.md) |
| A2D0 | [`sys-a2d0-closure.md`](sys-a2d0-closure.md) |
| A2D4 | [`sys-a2d4-closure.md`](sys-a2d4-closure.md) |
| A2D8 | [`sys-a2d8-closure.md`](sys-a2d8-closure.md) |
| A2DC | [`sys-a2dc-closure.md`](sys-a2dc-closure.md) |
| A2E0 | [`sys-a2e0-closure.md`](sys-a2e0-closure.md) |
| A2E8 | [`sys-a2e8-closure.md`](sys-a2e8-closure.md) |
| A2F0 | [`sys-a2f0-closure.md`](sys-a2f0-closure.md) |
| A2F4 | [`sys-a2f4-closure.md`](sys-a2f4-closure.md) |
| A2FC | [`sys-a2fc-closure.md`](sys-a2fc-closure.md) |
| A300 | [`sys-a300-blocked.md`](sys-a300-blocked.md) |
| A304 | [`sys-a304-closure.md`](sys-a304-closure.md) |
| A308 | [`sys-a308-blocked.md`](sys-a308-blocked.md) |
| A30C | [`sys-a30c-null-vector.md`](sys-a30c-null-vector.md) |
| A310 | [`sys-a310-null-vector.md`](sys-a310-null-vector.md) |
| A314 | [`sys-a314-null-vector.md`](sys-a314-null-vector.md) |
| A318 | [`sys-a318-null-vector.md`](sys-a318-null-vector.md) |
| A31C | [`sys-a31c-null-vector.md`](sys-a31c-null-vector.md) |
| A320 | [`sys-a320-null-vector.md`](sys-a320-null-vector.md) |
| A324 | [`sys-a324-null-vector.md`](sys-a324-null-vector.md) |
| A328 | [`sys-a328-null-vector.md`](sys-a328-null-vector.md) |
| A32C | [`sys-a32c-closure.md`](sys-a32c-closure.md) |
| A334 | [`sys-a334-closure.md`](sys-a334-closure.md) |
| A338 | [`sys-a338-blocked.md`](sys-a338-blocked.md) |
| A39C | [`abi-trace-a39c-strncpy.md`](abi-trace-a39c-strncpy.md) |
| A3A0 | [`abi-trace-a3a0-strrchr.md`](abi-trace-a3a0-strrchr.md) |
| A3A4 | [`abi-trace-a3a4-strstr.md`](abi-trace-a3a4-strstr.md) |
| A3A8 | [`abi-trace-a3a8-tolower.md`](abi-trace-a3a8-tolower.md) |
| A3AC | [`abi-trace-a3ac-toupper.md`](abi-trace-a3ac-toupper.md) |
| A3B0 | [`abi-trace-a3b0-ungetc.md`](abi-trace-a3b0-ungetc.md) |
| A3B4 | [`sys-a3b4-neo2013-closure.md`](sys-a3b4-neo2013-closure.md) |
| A3B8 | [`sys-a3b8-neo2013-closure.md`](sys-a3b8-neo2013-closure.md), [`abi-trace-a3b8.md`](abi-trace-a3b8.md) |
| A3BC | [`sys-a3bc-neo2013-closure.md`](sys-a3bc-neo2013-closure.md), [`abi-trace-a3bc.md`](abi-trace-a3bc.md) |
| A3C0 | [`sys-a3c0-neo2013-closure.md`](sys-a3c0-neo2013-closure.md), [`abi-trace-a3c0.md`](abi-trace-a3c0.md) |
| A3C4 | [`sys-a3c4-blocked.md`](sys-a3c4-blocked.md), [`abi-trace-a3c4.md`](abi-trace-a3c4.md) |
| A3C8 | [`sys-a3c8-blocked.md`](sys-a3c8-blocked.md), [`abi-trace-a3c8.md`](abi-trace-a3c8.md) |
| A3CC | [`sys-a3cc-blocked.md`](sys-a3cc-blocked.md), [`abi-trace-a3cc.md`](abi-trace-a3cc.md) |
| A3D0 | [`sys-a3d0-blocked.md`](sys-a3d0-blocked.md), [`abi-trace-a3d0.md`](abi-trace-a3d0.md) |
| A3D4 | [`sys-a3d4-blocked.md`](sys-a3d4-blocked.md), [`abi-trace-a3d4.md`](abi-trace-a3d4.md) |
| A3D8 | [`sys-a3d8-blocked.md`](sys-a3d8-blocked.md), [`abi-trace-a3d8.md`](abi-trace-a3d8.md) |
| A3DC | [`sys-a3dc-blocked.md`](sys-a3dc-blocked.md), [`abi-trace-a3dc.md`](abi-trace-a3dc.md) |
| A3E0 | [`sys-a3e0-blocked.md`](sys-a3e0-blocked.md), [`abi-trace-a3e0.md`](abi-trace-a3e0.md) |
| A3E4 | [`sys-a3e4-blocked.md`](sys-a3e4-blocked.md), [`abi-trace-a3e4.md`](abi-trace-a3e4.md) |
| A3E8 | [`sys-a3e8-blocked.md`](sys-a3e8-blocked.md), [`abi-trace-a3e8.md`](abi-trace-a3e8.md) |
| A3EC | [`sys-a3ec-blocked.md`](sys-a3ec-blocked.md), [`abi-trace-a3ec.md`](abi-trace-a3ec.md) |
| A3F0 | [`sys-a3f0-blocked.md`](sys-a3f0-blocked.md), [`abi-trace-a3f0.md`](abi-trace-a3f0.md) |
| A3F4 | [`sys-a3f4-blocked.md`](sys-a3f4-blocked.md), [`abi-trace-a3f4.md`](abi-trace-a3f4.md) |
| A3F8 | [`sys-a3f8-blocked.md`](sys-a3f8-blocked.md), [`abi-trace-a3f8.md`](abi-trace-a3f8.md) |
| A3FC | [`sys-a3fc-blocked.md`](sys-a3fc-blocked.md), [`abi-trace-a3fc.md`](abi-trace-a3fc.md) |
| A400 | [`sys-a400-blocked.md`](sys-a400-blocked.md), [`abi-trace-a400.md`](abi-trace-a400.md) |
| A404 | [`sys-a404-closure.md`](sys-a404-closure.md), [`abi-trace-a404.md`](abi-trace-a404.md) |
| A408 | [`sys-a408-blocked.md`](sys-a408-blocked.md), [`abi-trace-a408.md`](abi-trace-a408.md) |
| A40C | [`sys-a40c-blocked.md`](sys-a40c-blocked.md), [`abi-trace-a40c.md`](abi-trace-a40c.md) |
| A410 | [`sys-a410-blocked.md`](sys-a410-blocked.md), [`abi-trace-a410.md`](abi-trace-a410.md) |
| A414 | [`sys-a414-blocked.md`](sys-a414-blocked.md), [`abi-trace-a414.md`](abi-trace-a414.md) |
| A418 | [`sys-a418-blocked.md`](sys-a418-blocked.md), [`abi-trace-a418.md`](abi-trace-a418.md) |
| A41C | [`sys-a41c-closure.md`](sys-a41c-closure.md), [`abi-trace-a41c.md`](abi-trace-a41c.md) |
| A420 | [`sys-a420-blocked.md`](sys-a420-blocked.md), [`abi-trace-a420.md`](abi-trace-a420.md) |
| A424 | [`sys-a424-blocked.md`](sys-a424-blocked.md), [`abi-trace-a424.md`](abi-trace-a424.md) |
| A428 | [`sys-a428-blocked.md`](sys-a428-blocked.md), [`abi-trace-a428.md`](abi-trace-a428.md) |
| A42C | [`sys-a42c-blocked.md`](sys-a42c-blocked.md), [`abi-trace-a42c.md`](abi-trace-a42c.md) |
| A430 | [`sys-a430-blocked.md`](sys-a430-blocked.md), [`abi-trace-a430.md`](abi-trace-a430.md) |
| A434 | [`sys-a434-blocked.md`](sys-a434-blocked.md), [`abi-trace-a434.md`](abi-trace-a434.md) |
| A438 | [`sys-a438-neo2013-closure.md`](sys-a438-neo2013-closure.md), [`abi-trace-a438.md`](abi-trace-a438.md) |
| A43C | [`sys-a43c-blocked.md`](sys-a43c-blocked.md), [`abi-trace-a43c.md`](abi-trace-a43c.md) |
| A440 | [`sys-a440-blocked.md`](sys-a440-blocked.md), [`abi-trace-a440.md`](abi-trace-a440.md) |
| A444 | [`sys-a444-blocked.md`](sys-a444-blocked.md), [`abi-trace-a444.md`](abi-trace-a444.md) |
| A448 | [`sys-a448-blocked.md`](sys-a448-blocked.md), [`abi-trace-a448.md`](abi-trace-a448.md) |
| A44C | [`sys-a44c-blocked.md`](sys-a44c-blocked.md), [`abi-trace-a44c.md`](abi-trace-a44c.md) |
| A450 | [`sys-a450-blocked.md`](sys-a450-blocked.md), [`abi-trace-a450.md`](abi-trace-a450.md) |
| A454 | [`sys-a454-blocked.md`](sys-a454-blocked.md), [`abi-trace-a454.md`](abi-trace-a454.md) |
| A458 | [`abi-trace-a458.md`](abi-trace-a458.md) |
| A45C | [`abi-trace-a45c.md`](abi-trace-a45c.md) |
| A460 | [`abi-trace-a460.md`](abi-trace-a460.md) |
| A464 | [`abi-trace-a464.md`](abi-trace-a464.md) |
| A468 | [`abi-trace-a468.md`](abi-trace-a468.md) |
| A46C | [`abi-trace-a46c.md`](abi-trace-a46c.md) |
| A470 | [`abi-trace-a470.md`](abi-trace-a470.md) |
