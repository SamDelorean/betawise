# SYS_A1EC dynamic-file removal — source-first closure

Status: mechanically and semantically closed from canonical firmware evidence. The modern vendor symbol remains unknown, so the neutral `SYS_A1EC` name is retained.

```c
int32_t SYS_A1EC(uint16_t file_id);
```

## Source-first correlation

Historical FileModule material and the already reconstructed modern File API establish the conceptual distinction among content deletion, file erasure and descriptor removal. The earlier A1E0–A1FC block analysis identified A1EC as removal of a dynamic descriptor/allocation. That conclusion was rechecked directly against the three canonical ROMs.

## Canonical firmware confirmation

A1EC is vector index 123. Entry points:

- AS3000 2005: `0x004E250C`;
- NEO 2005: `0x005E4E68`;
- NEO 2013: `0x0043B26A`.

The public handler is exactly `0x1EE` bytes in every image. It consumes one `file_id` as a word from the first 32-bit ABI slot, contains one terminal `RTS`, uses private PC-relative helpers rather than absolute JSRs, and has no absolute direct JSR caller in the three firmware images. The negative absolute-JSR search does not exclude A-line/app callers.

Raw same-position byte identity exceeds 95% pairwise across the three generations. The repeated `0x48` descriptor-stride mechanics, terminal success construction and table-maintenance structure are preserved.

## Confirmed behavior

After resolving the supplied file token, A1EC removes the descriptor itself and reclaims/repairs the associated runtime allocation/table topology. This is not a byte-range delete and is not a content-only zero-fill.

On the successful path the reconstructed flow:

1. handles the active context if the target descriptor is currently active;
2. removes/reclaims the target allocation;
3. decrements the applicable runtime file/group count;
4. compacts the descriptor table in 0x48-byte records;
5. moves the descriptor-table end back by one record;
6. repairs affected group/folder descriptor pointers;
7. renumbers affected canonical-token metadata as required by the compacted table;
8. performs allocator maintenance;
9. returns `0`.

Resolver failure is returned as the resolver status and the removal path is not entered.

## Distinction from neighboring destructive APIs

- `FileDeleteChars` removes a byte range inside the active descriptor and leaves the descriptor itself present.
- `FileSmashFile` zero-fills the active file contents and resets its size/cursor while preserving the descriptor.
- `SYS_A1EC` removes the descriptor/allocation itself and repairs the descriptor table.

This distinction is important for SDK callers because A1EC changes file identity/topology and can invalidate assumptions about descriptor ordering/tokens.

## Refutation

- Refuted interpreting A1EC as a content-delete alias: the handler performs descriptor-table compaction and allocation reclamation.
- Refuted a no-argument/current-file-only ABI: one explicit 16-bit file token is consumed.
- Refuted a token return on success: the normal successful terminal value is `0`; resolver failures preserve status semantics.
- No speculative modern vendor name is introduced.

## Classification

**CONFIRMED:** one-argument ABI; File API resolution; destructive descriptor/allocation removal; 0x48-byte table compaction family; topology/pointer/token repair; success `0`; resolver-status failure; cross-generation structural equivalence.

**INFERENCIA FUERTE:** continuity with the historical file-removal/allocation-management subsystem.

**DESCONOCIDO:** original modern vendor symbol and private allocator/table helper names.

## Regression

Static primary-byte regression was **EXECUTED: 42/42 PASS** against all three canonical ROMs. It verifies canonical hashes, entry/span, one-word argument extraction, prologue/epilogue, single terminal RTS, absence of absolute internal JSRs, success `MOVEQ #0`, repeated 0x48 stride constants, negative absolute-direct-caller search and >95% pairwise raw structural identity.

Dynamic regression is **SPECIFIED / NOT EXECUTED** and must be emulator-first on disposable descriptors: remove non-active/active dynamic file, verify allocation reclaim, table compaction, token/index repair, group counts, resolver failure, and contrast with `FileSmashFile` / `FileDeleteChars`.

Firmware bytes, complete disassembly and ROM-derived workpapers remain private.