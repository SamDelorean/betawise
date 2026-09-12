# SDK audit — public umbrella-header boundary A2B0–A308

Status: **AUDIT_FINDING / NEXT_BUILD_BATCH**

## Scope

This pass compares the consolidated ABI classification for A2B0–A308 against the public umbrella header `os3k/os3k.h` and the focused reconstructed headers already present in `os3k/`.

The purpose is to separate safe public-surface reconciliation from entries whose mechanics are reconstructed but whose source-level return contract remains evidence-limited.

## Safe `CERRADA` contracts

The consolidated ABI marks the following entries `CERRADA` in this interval:

- A2B4
- A2B8 (`_OS3K_CallSysInt` / `CallSysInt`)
- A2BC
- A2C0
- A2C4
- A2C8
- A2CC
- A2D0
- A2D4
- A2D8
- A2DC
- A2E0
- A2E8
- A2F0
- A2F4
- A2FC
- A304

A2B8 is already surfaced by the umbrella header as `CallSysInt`, so it is not a new public-header gap.

The remaining `CERRADA` entries above have focused neutral headers and/or published reconstructed contracts and are candidates for umbrella exposure under the same build gate used for the earlier reconciliation batches.

Representative examples already present in the branch include:

```c
void SYS_A2B4(uint32_t *out_0, uint32_t *out_1, uint32_t *out_2);

int32_t SYS_A2BC(void *state,
                 uint16_t selector,
                 uint16_t flags,
                 uint32_t value,
                 uint16_t coord_x,
                 uint16_t coord_y,
                 uint16_t extent_x,
                 uint16_t extent_y);

uint8_t SYS_A2E8(void *state,
                 uint32_t selector,
                 void *arg3,
                 uint32_t *out_value,
                 void *unused_arg5);

uint8_t SYS_A2FC(void *state,
                 uint32_t selector,
                 void *arg3,
                 uint32_t *out_value,
                 void *unused_arg5);

uint8_t SYS_A304(void *state, uint16_t value, const char *string);
```

## Explicitly excluded: evidence-limited contracts

The following entries are **not** candidates for umbrella promotion in this batch:

- A2B0 — `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; `void SYS_A2B0(uint8_t value)` remains an inference and the vendor return contract is not demonstrated.
- A2E4 — `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; mechanics are closed but the residual D0 value is not a demonstrated public return contract.
- A2EC — `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; mechanical argument/output behavior is reconstructed but the public return contract remains unknown.
- A2F8 — `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; two-pointer mechanics reconstructed, return contract unknown.
- A300 — `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; `void SYS_A300(void *state)` is only a very strong inference.
- A308 — `CERRADA_CON_LÍMITE_DE_EVIDENCIA`; mechanics reconstructed, `void` return remains inferential.

Absence of focused public headers for several of these evidence-limited entries is therefore correct behavior, not an SDK omission.

## Public-header finding

`os3k/os3k.h` currently exposes `CallSysInt` but does not surface the other safe `CERRADA` contracts in this interval. `os3k/syscall.c` already emits veneers through A308, so this is a declaration/public-surface reconciliation problem rather than missing linkage.

Recommended build-gated umbrella candidates from this pass are therefore:

`A2B4, A2BC, A2C0, A2C4, A2C8, A2CC, A2D0, A2D4, A2D8, A2DC, A2E0, A2E8, A2F0, A2F4, A2FC, A304`.

Where a focused header carries structural typedefs or comments that should remain authoritative, prefer controlled inclusion or exact synchronized declarations rather than inventing new names or incompatible types.

## Build gate

Do not mark this reconciliation `BUILD_VALIDATED` until the Linux/m68k environment runs:

1. `make -C os3k clean all`
2. clean build/link of a representative applet
3. declaration-compatibility check between `os3k.h` and focused headers
4. no promotion of the six evidence-limited entries above

This audit does not change ABI inventory counts and does not reopen reverse engineering.
