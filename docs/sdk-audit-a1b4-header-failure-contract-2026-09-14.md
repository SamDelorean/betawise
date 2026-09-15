# SDK audit — A1B4 header failure contract reconciliation — 2026-09-14

## Finding

`os3k/file_space_accounting.h` correctly declared the closed A1B4 ABI but its short public comment omitted a material boundary established by the source-first closure: ordinary-token resolution failure, and the `0x00FC` active-descriptor resolution path, preserve the resolver-provided auxiliary result rather than guaranteeing zero.

This omission could encourage callers to treat zero as the universal failure result, contradicting `docs/sys-a1b4-size-accounting-source-first-closure.md` and the canonical runtime/storage map.

## Correction

The header comment now states that ordinary-token and `0xFC` resolution failures preserve the resolver-provided auxiliary result and that callers must not assume every failure returns zero.

No prototype, ABI, syscall status, or public symbolic name changed. No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry was promoted.

## Validation boundary

This is a comment-only SDK reconciliation. It does not require m68k code generation and is not marked `BUILD_VALIDATED`. The broader umbrella-header integration remains a separate build-pending task.
