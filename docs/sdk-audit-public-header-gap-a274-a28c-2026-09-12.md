# SDK audit — public umbrella-header gap A274–A28C

Status: **AUDIT_FINDING / NEXT_BUILD_BATCH**

## Finding

The canonical ABI index and focused auxiliary headers already expose mechanically closed System 3 contracts throughout the `A274–A28C` range, while the public umbrella header `os3k/os3k.h` does not currently surface them.

This is a public-surface consistency gap, not unfinished reverse engineering.

## Closed contracts already available in focused headers

```c
void SYS_A274(void);
void *SYS_A278(void);

void SYS_A27C(
    const uint8_t **cursor,
    uint16_t *field_0_out,
    uint16_t *field_2_out,
    uint16_t *payload_size_out,
    const uint8_t **payload_out);

void SYS_A280(
    uint8_t **cursor,
    uint16_t field_0,
    uint16_t field_2,
    uint16_t payload_size,
    const uint8_t *payload);

int32_t SYS_A284(
    const uint8_t **cursor,
    uint16_t field_0,
    uint16_t field_2,
    uint16_t *payload_size_out,
    const uint8_t **payload_out);

uint16_t SYS_A288(void);

uint8_t SYS_A28C(
    const char *primary_name,
    const char *secondary_name,
    SYS_A28C_DataCallback data_callback,
    SYS_A28C_EventCallback event_callback);
```

Sources:

- `os3k/printer_selection.h`
- `os3k/applet_settings.h`
- `os3k/record_cursor.h`
- `os3k/record_writer.h`
- `os3k/record_search.h`
- `os3k/transport_payload.h`
- `os3k/irda_state_init.h`
- `docs/applet-current-settings-closure.md`
- `docs/abi-reconstruction-index.md`

The corresponding veneers are already present in `os3k/syscall.c` at indices 157–163.

## A278 correction from the continued audit

An earlier version of this audit note conservatively left A278 outside the proposed batch pending status verification. That caution is now superseded.

`docs/applet-current-settings-closure.md` explicitly closes A278 mechanically across AS3000 2005, NEO 2005 and NEO 2013 and gives the usable public form:

```c
void *SYS_A278(void);
```

The trap consumes no arguments and returns either the current SmartApplet settings-region pointer or `NULL` in full `D0`. `os3k/applet_settings.h` already carries the same declaration. Therefore A278 belongs in the same umbrella-header reconciliation batch on exactly the same evidence policy as A274/A27C/A280/A284/A288/A28C.

## Scope boundary

This finding does **not** imply promotion of neighboring traps outside the mechanically closed focused-header set.

- A274, A278, A27C, A280, A284, A288 and A28C have focused closure documents/headers and explicit usable C contracts.
- No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry should be strengthened merely to produce a contiguous numerical run.
- A290/A294 and later entries remain governed by their own consolidated status and should not be inferred from adjacency.

## Recommended integration pattern

Prefer keeping the focused headers as the detailed contract authorities and making the umbrella header expose the same safe declarations either by controlled includes or by synchronized declarations. Do not duplicate callback typedefs incompatibly.

Because `SYS_A28C` depends on callback typedefs declared in `irda_state_init.h`, inclusion is safer than manually duplicating those typedefs in `os3k.h`.

## Build gate

Treat this as a build-gated SDK batch:

1. integrate only the already closed focused headers/contracts;
2. clean rebuild `os3k/libos3k.a` with `m68k-elf-gcc`;
3. build/link a representative applet;
4. confirm no declaration conflicts or duplicate typedefs;
5. only then mark the umbrella-header reconciliation `BUILD_VALIDATED`.

No source-level/vendor names are assigned by this audit. No ABI inventory counts change.
