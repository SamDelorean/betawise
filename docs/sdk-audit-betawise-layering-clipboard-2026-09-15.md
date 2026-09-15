# SDK audit — BetaWise layering and clipboard visibility

Date: 2026-09-15
Branch: `sdk/abi-automation`

## Material finding

The current audit must preserve the original BetaWise SDK layering instead of treating every reconstructed subheader as an umbrella-header include candidate.

Direct inspection gives a useful control:

- `syscall.c` maps A138 directly to the historical public symbol `ShowBatteryPercentage`.
- `os3k.h` declares `ShowBatteryPercentage(uint8_t time_seconds)` as part of the public SDK surface.
- `syscall.c` maps A20C/A210/A214 directly to the historical BetaWise names `ClipboardSet`, `ClipboardGet`, and `ClipboardClear`.
- `file_clipboard_buffer.h` explicitly records that these three names already belonged to BetaWise and now carries the reconstructed closed mechanical contracts.
- Unlike Battery, however, `os3k.h` currently has no declarations for `ClipboardSet`, `ClipboardGet`, or `ClipboardClear`.

This is stronger than a generic “missing subheader include” observation: it is a concrete `.h <-> syscall.c <-> historical BetaWise name` inconsistency for already-closed contracts. The safe reconciliation unit is therefore the individual public symbol and its historical placement, not the auxiliary reconstruction header as a whole.

## Rule for subsequent audit

For each `CERRADA` contract, reconcile in this order:

1. closed ABI/mechanical contract;
2. veneer/symbol in `syscall.c`;
3. wrapper/adaptation in `os3k.c` when the original BetaWise architecture used one;
4. public declaration, shared types, and structures in `os3k.h` according to the original BetaWise organization;
5. auxiliary reconstructed headers remain evidence/documentation unless their inclusion is demonstrably consistent with that organization.

Do not promote `CERRADA_CON_LÍMITE_DE_EVIDENCIA`, and do not reopen a closed syscall absent a concrete dependency.

## Build status

No header was changed in this audit batch. The concrete clipboard public-surface correction is a candidate for a later header batch, but any such change requires the real m68k build gate before it may be marked `BUILD_VALIDATED`.

Status: `AUDIT_FINDING / PENDING_M68K_BUILD_FOR_HEADER_CHANGE`.
