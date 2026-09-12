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

## SDK reconciliation: keyboard modifier API

The historical header/stub mismatch around `A0A0` and `A0AC` is now resolved without removing the older source-level spellings.

Canonical raw trap contracts:

```c
uint16_t GetModifierKeys(void);       /* raw A0A0: complete unshifted modifier word */
void SetModifierKeys(uint16_t value); /* raw A0AC: replaces the complete 16-bit word */
```

`GetKeyModifiers()` and `SetKeyModifiers()` remain available as source-compatibility inline wrappers. The historical getter retains its documented shifted-byte behavior by returning `(uint8_t)(GetModifierKeys() >> 8)`; it is therefore no longer confused with the raw `A0A0` trap. The historical setter forwards directly to the raw 16-bit replacement operation.

The public header also now exposes already-closed contracts that had remained absent from `os3k.h`: `FileWriteBuffer`, `FileReadBuffer`, `SYS_A248`, and the `A260/A264/A268/A26C/A270` SmartApplet mask-table family. No unresolved late-NEO contract was promoted to a stronger public prototype.

SDK reconciliation commit: `e53a44d4adb9f24728d53da9481f6c76078513c2`.

## Operational conclusion

The sequential syscall-discovery phase is complete for the corroborated `A000..A470` range. Further work should prioritize emulator-facing contracts and validation of the public SDK surface. Small ROM analysis should be reopened only when it resolves a concrete emulator or ABI dependency.

## SDK header reconciliation — batch 2A validation

The first part of the second SDK-header reconciliation batch is now build-validated on the Linux 64-bit development host.

The historical SYS_A32C and SYS_A334 declarations were corrected to the already-closed ABI contracts: SYS_A32C returns uint32_t and SYS_A334 returns int32_t, both with void parameter lists.

Validation after the header change:

- clean rebuild of os3k/libos3k.a: PASS;
- clean build and link of applets/HelloWorld: PASS;
- toolchain: Linux 64-bit host with m68k-elf-gcc;
- no additional ABI contracts were strengthened or exposed in this batch.

Status: BUILD_VALIDATED.

## SDK header reconciliation — batch 2B validation

The closed raw contracts SYS_A0B8, SYS_A0BC, SYS_A0C0, SYS_A0C4, SYS_A0C8, SYS_A0CC, and SYS_A0D0 are now exposed in os3k.h using neutral SYS_Axxx names only. No higher-level vendor semantics were assigned.

Validation after the header change:

- clean rebuild of os3k/libos3k.a: PASS;
- clean build and link of applets/HelloWorld: PASS;
- no CERRADA_CON_LÍMITE_DE_EVIDENCIA contract was promoted.

Status: BUILD_VALIDATED.

## SDK header reconciliation — batch 2C validation

The closed NEO13-only raw contracts SYS_A3B4, SYS_A3B8, SYS_A3BC, SYS_A3C0, SYS_A404, SYS_A41C, SYS_A438, and SYS_A470 are now exposed in os3k.h in a clearly marked NEO13-only section. Their neutral SYS_Axxx names are retained because source-level/vendor identities remain unknown.

Validation after the header change:

- clean rebuild of os3k/libos3k.a: PASS;
- clean build and link of applets/HelloWorld: PASS;
- no A3C4-A400, A408-A434, or A43C-A454 limited-evidence entries were promoted.

Status: BUILD_VALIDATED.

## SDK reconciliation — A250 correction

The public SDK declaration and consolidated documentation for SYS_A250 were reconciled with the later source-first closure. Firmware consumes the low byte of the first ABI slot; it is therefore exposed as uint8_t prompt_variant rather than an unused uint32_t reserved slot. All direct native callers found in each canonical ROM pass selector value 2, while the original semantic name and selector meaning remain unresolved.

Validation after the header correction:

- clean rebuild of os3k/libos3k.a: PASS;
- clean build and link of applets/HelloWorld: PASS;
- password_runtime.h, os3k.h, the canonical ABI index, and the consolidated A248-A258 reference now agree.

Status: BUILD_VALIDATED.
