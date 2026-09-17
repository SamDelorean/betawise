# SDK header reconciliation — final audited build batch

Status: **AUDIT_COMPLETE / SOURCE_INTEGRATION_PENDING_BUILD_GATE**

This note consolidates the public-header gaps found by the full A000–A470 SDK audit. It is an integration manifest, not a new ABI reconstruction pass. Only contracts already classified `CERRADA` and already represented by exact focused headers are eligible. `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entries remain excluded.

## Integration policy

Prefer including the existing focused headers from `os3k/os3k.h` rather than copying their declarations into a second location. This keeps one declaration authority for complex callback typedefs, packed record structures, destructive-operation warnings and edge-case notes. If the include graph causes compatibility problems, reproduce only exact synchronized declarations as a fallback.

Do not rename neutral `SYS_Axxx` interfaces and do not infer vendor semantics during this batch.

## Batch A — File API support, A1AC–A1FC

Focused authorities:

- `file_space_accounting.h`
- `file_password_state.h`
- `file_dynamic.h`

Eligible contracts:

```c
uint32_t SYS_A1AC(void);
uint32_t SYS_A1B0(uint16_t file_id);
uint32_t SYS_A1B4(uint16_t file_id_or_selector);
uint32_t SYS_A1B8(int32_t size_or_command);
uint32_t SYS_A1BC(uint16_t file_id);
int32_t SYS_A1D0(uint16_t file_id, uint32_t state_mask, uint8_t enable);
int32_t SYS_A1D4(uint16_t file_id, uint32_t state_mask);
uint32_t SYS_A1D8(void);
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);
int32_t SYS_A1E8(uint8_t token_group, const char *name,
                 uint32_t min_size, uint32_t requested_max_size);
int32_t SYS_A1EC(uint16_t file_id);
char *SYS_A1F0(uint16_t file_id, char *name_buffer, uint8_t set_name);
int32_t SYS_A1F4(uint16_t file_id, uint16_t index_value, uint16_t *index_out);
int32_t SYS_A1F8(uint16_t local_index);
int32_t SYS_A1FC(uint8_t token_low, uint8_t token_high);
```

Safety: A1D8 and A1EC are destructive. A1DC modern write mode accepts lengths 0..6 and rejects >6 with raw -8.

## Batch B — Clipboard and applet selection, A200–A234

Focused authorities:

- `file_clipboard_edit.h`
- `file_clipboard_buffer.h`
- `file_clipboard_state.h`
- `applet_selection.h`

Eligible contracts are the exact focused-header declarations for A200, A204, A208, A20C, A210, A214, A218, A21C, A220, A224, A228, A22C, A230 and A234.

## Batch C — Applet settings and packed records / transport, A274–A2AC

Focused authorities include:

- `printer_selection.h`
- `applet_settings.h`
- `record_cursor.h`
- `record_writer.h`
- `record_search.h`
- `transport_payload.h`
- `irda_state_init.h`
- the dedicated A290–A2AC focused headers already audited on this branch.

A278 is explicitly eligible: `void *SYS_A278(void);` is mechanically closed and the earlier caution excluding it was superseded by `applet-current-settings-closure.md`.

A290–A2AC are eligible only via their exact existing focused declarations; do not reconstruct or simplify their structure/callback types in `os3k.h`.

## Batch D — closed subset A2B0–A308

A2B8 is already publicly represented as `CallSysInt`; do not duplicate it under a speculative alias.

Eligible closed slots requiring umbrella exposure are:

`A2B4, A2BC, A2C0, A2C4, A2C8, A2CC, A2D0, A2D4, A2D8, A2DC, A2E0, A2E8, A2F0, A2F4, A2FC, A304`.

Explicitly excluded because they remain `CERRADA_CON_LÍMITE_DE_EVIDENCIA`:

`A2B0, A2E4, A2EC, A2F8, A300, A308`.

## Already reconciled — do not churn

The audit found no new integration work needed for the already reconciled raw modifier API, A0B8/A0BC/A0C0/A0C4/A0D0, A248/A250/A260/A264/A26C/A270, A32C/A334, and the public late NEO13 closed contracts A3B4/A3B8/A3BC/A3C0/A404/A41C/A438/A470.

A468/A46C remain intentionally private unless a concrete SDK consumer requires them. A338 remains a compatibility-surface special case and is not part of this integration batch.

## Required build gate

No source integration produced from this manifest may be called `BUILD_VALIDATED` until all of the following run on the real m68k toolchain:

```sh
cd ~/Projects/alphasmart/betawise
make -C os3k clean all
make -C applets/Betawise clean all
```

Then build/link at least one additional representative applet that exercises ordinary `os3k.h` inclusion. Confirm there are no duplicate declarations, typedef collisions, include cycles, or incompatible declarations between the umbrella and focused headers.

If include-based integration fails only because of header graph/legacy compatibility, switch to exact synchronized declarations; do not alter ABI types to make the build pass.

## Acceptance

The batch is integrated only after:

1. the source diff contains only exact already-closed contracts;
2. no limited-evidence slot is promoted;
3. OS3K library build passes;
4. representative applet clean build/link passes;
5. the exact validation commands and commit are recorded in the continuity/ABI consolidation records.

Until then the state is **REVISADO_ESTÁTICAMENTE / COMPILACIÓN_PENDIENTE**.
