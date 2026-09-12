# SDK audit — public umbrella-header gap A1AC–A1FC

Status: **AUDIT_FINDING / NEXT_BUILD_BATCH**

## Finding

The continued SDK audit shows that the earlier conservative exclusion of the `A1AC–A1FC` auxiliary interfaces from umbrella-header reconciliation is no longer justified by the current canonical evidence.

The ABI reconstruction index classifies the relevant contracts below as mechanically closed, and the branch already contains focused public headers with exact C declarations and safety notes. `os3k/os3k.h` exposes the older core File API calls and A1E0/A1E4 dialogs, but does not surface these closed accounting/state/dynamic-file interfaces.

This is therefore a public-SDK coverage gap, not unfinished reverse engineering.

## Closed accounting/resize contracts

From `os3k/file_space_accounting.h`:

```c
uint32_t SYS_A1AC(void);
uint32_t SYS_A1B0(uint16_t file_id);
uint32_t SYS_A1B4(uint16_t file_id_or_selector);
uint32_t SYS_A1B8(int32_t size_or_command);
uint32_t SYS_A1BC(uint16_t file_id);
```

Their mechanical roles are already documented: global allocator/reclaimable accounting, unused capacity, size/accounting selector family, active-file resize/state commands, and maximum capacity. Original vendor names remain unknown, so the neutral names must be retained.

## Closed file-password/state contracts

From `os3k/file_password_state.h`:

```c
int32_t SYS_A1D0(uint16_t file_id, uint32_t state_mask, uint8_t enable);
int32_t SYS_A1D4(uint16_t file_id, uint32_t state_mask);
uint32_t SYS_A1D8(void);
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);
```

A1D8 is destructive and must remain clearly documented as such. A1DC is bidirectional and, per the later source-first correction, accepts write lengths `0..6` and rejects only lengths `> 6` with raw `-8`. Public exposure does not remove those safety constraints.

## Closed dynamic-file identity contracts

From `os3k/file_dynamic.h`:

```c
int32_t SYS_A1E8(uint8_t token_group, const char *name,
                 uint32_t min_size, uint32_t requested_max_size);
int32_t SYS_A1EC(uint16_t file_id);
char *SYS_A1F0(uint16_t file_id, char *name_buffer, uint8_t set_name);
int32_t SYS_A1F4(uint16_t file_id, uint16_t index_value, uint16_t *index_out);
int32_t SYS_A1F8(uint16_t local_index);
int32_t SYS_A1FC(uint8_t token_low, uint8_t token_high);
```

These contracts cover descriptor creation/removal, filename get/set, local-index get/set and reverse lookup, and 16-bit token construction/validation. A1EC is destructive; neutral names remain appropriate because reliable modern vendor symbols were not recovered.

## Why the prior exclusion is superseded

The earlier A200–A234 audit note deliberately excluded `A1AC–A1FC` pending a separate evidence check. That check is now complete at the documentation/header level:

- focused public headers exist;
- the canonical ABI index describes the listed interfaces as mechanically closed;
- exact prototypes are already maintained in those focused headers;
- no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` promotion is required to expose them;
- no new semantic/vendor naming guess is necessary.

The correct remaining gate is compilation/integration, not further reverse engineering.

## Recommended integration pattern

Prefer including the focused headers from `os3k/os3k.h` or reproducing only exact synchronized declarations. Focused headers should remain the detailed authority for edge cases and destructive-operation warnings.

A single broad include-based pattern may be cleaner than manually duplicating every declaration, but that choice must be validated against the existing header include graph and old applet source compatibility.

## Build gate

Do not call this integrated or `BUILD_VALIDATED` until the Linux/m68k toolchain executes:

1. expose only the exact already-closed focused interfaces;
2. `make -C os3k clean all` — PASS;
3. clean build/link of one representative applet — PASS;
4. verify no duplicate declaration/type conflicts;
5. record the resulting commit and validation in the ABI consolidation and continuity records.

No ABI inventory count changes are implied by this audit finding.
