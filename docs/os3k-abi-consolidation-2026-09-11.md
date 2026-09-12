# OS3K ABI consolidation — 2026-09-11

This note records the current source-first consolidation of the OS3K A-line ABI through trap `A470` (index 284). It is a functional description only; no proprietary ROM bytes or disassembly are included.

## Current inventory

The reconciled inventory contains one entry for every index `0..284` (`A000..A470`):

- `CERRADA`: 208
- `CERRADA_CON_LÍMITE_DE_EVIDENCIA`: 56
- `NO_SYSCALL/NO_CALLABLE`: 21
- `PENDIENTE_REAL`: 0
- total: 285

`CERRADA_CON_LÍMITE_DE_EVIDENCIA` means that the callable's mechanical behavior is sufficiently reconstructed, but some source-level property such as the vendor name, exact C return contract, aggregate-result interpretation, or higher-level private semantics cannot be demonstrated from the available corpus. These entries are not treated as unfinished ABI reverse engineering unless new discriminating evidence becomes available.

`NO_SYSCALL/NO_CALLABLE` is used for null vectors, structural markers, non-pointer table entries, and other positions that do not establish a callable A-line contract.

## Reconciliation corrections

The consolidation expands historical compact rows and fills representation gaps, including the `A050–A068` null-vector span, `A0B8–A0C4`, the File API / clipboard region around `A198–A238`, and later compacted NEO-only entries.

`A258` is kept separate from the `A260` SmartApplet mask-table family. The recovered `A258` contract is the byte-sized setter paired with the `A248` protection-state getter.

Late NEO-only entries whose mechanics are known but whose return or source-level contract is explicitly unknown remain closed with an evidence limit rather than being promoted to a complete public C prototype.

## SDK reconciliation item: keyboard modifier API

The branch currently has a header/stub naming mismatch around `A0A0` and `A0AC`:

- `os3k/syscall.c` exports raw traps as `GetModifierKeys` and `SetModifierKeys`.
- `os3k/os3k.h` still declares the older `GetKeyModifiers` / `SetKeyModifiers` interface and describes a shifted 8-bit getter.

The recovered raw contracts are:

```c
KeyMod_e GetModifierKeys(void);       /* raw A0A0: unshifted 16-bit modifier word */
void SetModifierKeys(uint16_t value); /* raw A0AC: replaces the complete 16-bit word */
```

Backward compatibility, if required, should be implemented as explicit wrappers/aliases rather than by assigning the historical shifted-8-bit behavior to the raw traps.

No compatibility wrapper is introduced by this documentation-only commit; this prevents an accidental source-level API break while making the discrepancy explicit for the next SDK patch.

## Operational conclusion

The sequential syscall-discovery phase is complete for the corroborated `A000..A470` range. Further work should prioritize SDK/header cleanup and emulator-facing contracts. Small ROM analysis should be reopened only when it resolves a concrete emulator or ABI dependency.
