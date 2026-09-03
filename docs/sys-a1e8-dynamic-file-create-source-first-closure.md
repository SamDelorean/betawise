# SYS_A1E8 dynamic-file creation — source-first closure

Status: mechanically and semantically closed from canonical firmware evidence. No reliable modern vendor symbol was recovered, so the neutral `SYS_A1E8` name is retained.

```c
int32_t SYS_A1E8(
    uint8_t token_group,
    const char *name,
    uint32_t min_size,
    uint32_t requested_max_size);
```

## Source-first correlation

Historical FileModule material establishes the older AlphaSmart file-allocation lineage, while the already reconstructed System 3 File API establishes 16-bit canonical tokens, 0x48-byte descriptors, token groups and descriptor `+0x10` as `min_size`. The prior A1E0–A1FC block closure supplied a working hypothesis for A1E8; this audit rechecked it against the three canonical ROMs rather than treating the earlier reconstruction as primary proof.

## Canonical firmware confirmation

A1E8 is vector index 122. Entry points:

- AS3000 2005: `0x004E1FD0`;
- NEO 2005: `0x005E492C`;
- NEO 2013: `0x0043AD2E`.

The public handler is exactly `0x40E` bytes in all three images, with one terminal `RTS` and six absolute internal `JSR` instructions.

The handler directly consumes four 32-bit ABI slots in the same positions 3/3:

- `token_group`: low byte of the first slot;
- `name`: pointer from the second slot;
- `min_size`: full longword from the third slot;
- `requested_max_size`: full longword from the fourth slot.

A single direct internal JSR caller exists per canonical ROM and is structurally equivalent across generations.

## Confirmed behavior

A1E8 creates and allocates a new System 3 file descriptor in the selected token group.

Confirmed primary invariants include:

- descriptor-table count is compared against 127; full table returns raw `-9`;
- invalid/empty normalized name returns raw `-13`;
- invalid or unavailable requested token group returns raw `-10`;
- duplicate name returns raw `-15`;
- allocation failure after maintenance/retry returns raw `-12`;
- descriptor records use the already established 0x48-byte layout;
- the new local-index field at descriptor `+0x46` is initialized to `0x00FF`;
- `min_size` remains distinct from requested maximum allocation;
- effective maximum allocation is bounded below by both `min_size` and `0x200`:

```text
effective_max = max(requested_max_size, min_size, 0x200)
```

The `0x200` comparison and assignment occur at identical handler offsets in all three ROMs.

On successful construction/allocation the new descriptor is integrated into the runtime table/group topology and the function returns its canonical modern token. If provisional construction cannot be completed, the failure paths undo the provisional state before returning the relevant raw negative status.

## Token-group/name semantics

The modern token-group model is the same one independently closed by A1FC and A254: token identity is not the early single-byte sequential file-number scheme. Name handling uses the System 3 dynamic-file naming rules previously reconstructed for this block, including normalization/truncation to the descriptor name capacity and duplicate detection in the relevant group/class.

The exact modern vendor function name is not inferred from the historical source.

## Refutation

- Refuted a three-argument ABI: four distinct slots are loaded directly by the handler 3/3.
- Refuted treating `min_size` as an anonymous accounting threshold: A1E8 is the constructor that stores/uses it as minimum allocation, corroborating the later accounting routines.
- Refuted an unconstrained requested maximum: firmware explicitly enforces a minimum effective allocation of `max(min_size, 0x200)`.
- Refuted a generic success/failure boolean return: successful creation returns the canonical 16-bit token zero-extended; failures use distinct raw negative statuses.
- No vendor name is invented; the public reconstruction remains `SYS_A1E8`.

## Classification

**CONFIRMED:** four-argument ABI; dynamic descriptor creation; 127-record capacity check; 0x48 descriptor model; name/group validation; duplicate detection; `min_size`; effective maximum floor 0x200; local index 0x00FF; allocation retry/failure family; distinct raw errors -9/-10/-12/-13/-15; canonical-token success return; cross-ROM equivalence.

**INFERENCIA FUERTE:** continuity with the historical AlphaSmart dynamic/file-allocation subsystem.

**DESCONOCIDO:** original modern vendor symbol and symbolic names of the raw negative statuses/private allocator helpers.

## Regression

Static primary-byte regression was **EXECUTED: 70/70 PASS** against all three canonical ROMs. It verifies ROM identity, A1E8 handler span/prologue/epilogue, argument loads, single RTS, six JSRs, capacity-127 branch, all five raw error constructions, `+0x46 = 0x00FF`, `0x200` effective-allocation floor, one direct internal caller per ROM and cross-generation structural invariants.

Dynamic regression is **SPECIFIED / NOT EXECUTED** and must be emulator-first because creation changes descriptor-table and allocator topology. Cases: valid creation, empty/invalid name, duplicate name, unavailable group, table-full condition, `requested_max_size < min_size`, both below 0x200, allocation failure/retry, token/index initialization and cleanup after failed provisional creation.

Firmware bytes, complete disassembly and ROM-derived workpapers remain private.