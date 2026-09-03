# SYS_A1F0 file-name get/set — source-first closure

Status: mechanically and semantically closed from canonical firmware evidence. No reliable modern vendor symbol was recovered, so the neutral `SYS_A1F0` name is retained.

```c
char *SYS_A1F0(uint16_t file_id, char *name_buffer, uint8_t set_name);
```

## Source-first correlation

BetaWise maps vector index 124 to `SYS_A1F0` without a vendor name. Historical FileModule material establishes the File API lineage but does not recover an exact modern symbol. The older A1E0–A1FC reconstruction was therefore treated only as a hypothesis.

The neighboring A1E8 source-first closure independently established the modern dynamic-file naming rules. Direct target correlation now shows that A1F0 calls the already mapped C-library handlers `memcpy` (A360), `strcpy` (A38C) and `strlen` (A390), while its validation and duplicate-name comparison branches reuse the same private naming helpers present in the A1E8 block.

## Canonical firmware confirmation

A1F0 is vector index 124. Entry points:

- AS3000 2005: `0x004E26FA`;
- NEO 2005: `0x005E5056`;
- NEO 2013: `0x0043B458`.

The handler is exactly `0x1C4` bytes in all three canonical images. It consumes three 32-bit ABI slots: a 16-bit `file_id`, a pointer `name_buffer`, and the low byte `set_name`. `set_name == 0` selects GET; any nonzero value selects SET.

## Confirmed behavior

The descriptor's internal NUL-terminated name begins at offset `+0x34` and has an effective capacity of 15 characters plus NUL.

### GET (`set_name == 0`)

- resolve `file_id` through the common File API resolver;
- resolution failure returns `NULL`;
- when `name_buffer != NULL`, copy the internal name with `strcpy(name_buffer, descriptor + 0x34)`;
- when `name_buffer == NULL`, omit the external copy;
- successful GET returns the internal `descriptor + 0x34` pointer.

### SET (`set_name != 0`)

`name_buffer` is a required non-NULL precondition on the SET path: the handler passes it directly to `strlen` without a null check.

The candidate is normalized before validation:

- length `<= 15`: copy to a local buffer with `strcpy`;
- length `> 15`: copy exactly 15 bytes with `memcpy` and force local byte 15 to NUL.

Thus overlength input is truncated to 15 characters rather than rejected solely for length.

The normalized candidate then passes through the same private name-validation family used by A1E8. Invalid input returns `NULL` before replacement. A1F0 scans the applicable descriptor topology in 0x48-byte records and uses the A1E8 duplicate-name comparator. The target descriptor is explicitly excluded from comparison with itself. A duplicate name on another applicable descriptor returns `NULL` without replacing the old name.

A valid, non-duplicate SET copies the normalized candidate to `descriptor + 0x34` with `strcpy` and returns that internal pointer.

Before the duplicate scan, the SET path also repairs an overlength existing target name by forcing its byte 15 to NUL if its measured length exceeds 15.

## Callers and generation comparison

Exactly one absolute direct A1F0 JSR exists in each canonical firmware image. The three callers are structurally equivalent and construct `set_name = 1` with a file token and name pointer, independently confirming the SET interpretation. No direct internal GET caller was found; that negative result does not exclude A-line/app callers.

The three handlers have the same span, five absolute JSR positions, three structural BSR positions and control flow. Only 22 same-position bytes differ; those differences are confined to relocations, global addresses and call displacements. Outside those fields the opcode skeleton is identical.

## Refutation

- Refuted a getter-only API: the third slot selects a complete SET path and the direct internal caller uses value 1.
- Refuted rejecting every name longer than 15 characters: firmware explicitly performs `memcpy(..., 15)` followed by NUL termination.
- Refuted treating the C-library calls as unnamed private helpers: their targets match A360/A38C/A390 exactly.
- Refuted allowing a duplicate name on another applicable descriptor: the 0x48-byte scan and shared A1E8 comparator reject it while excluding self.
- No speculative `FileGetName`/`FileSetName` vendor symbol is introduced.

## Classification

**CONFIRMED:** three-argument ABI; zero/nonzero GET/SET selector; common File API resolver; name at descriptor `+0x34`; 15+NUL capacity; exact long-name truncation; `strlen`/`memcpy`/`strcpy`; A1E8 validation/duplicate family; self-exclusion; internal-pointer success return; NULL failure; one SET caller per ROM; cross-ROM equivalence.

**INFERENCIA FUERTE:** conceptual role as the modern File API file-name get/set operation.

**DESCONOCIDO:** original modern vendor symbol and the original names of the private validation/comparison helpers.

## Regression

Static primary-byte regression was **EXECUTED: 99/99 PASS** against the three canonical ROMs. It verifies canonical hashes, handler span/prologue/epilogue, argument slots, selector branch, mapped C-library targets, 15-byte normalization path, A1E8 shared-helper targets, 0x48 descriptor scan, self-exclusion, duplicate NULL path, GET optional copy, common internal-pointer return, direct caller counts and cross-generation opcode skeleton.

Dynamic regression is **SPECIFIED / NOT EXECUTED**: GET with destination/NULL, SET lengths 0/1/15/16/>15, invalid normalized name, duplicate other descriptor, self-name, invalid token, and preservation of the previous name on failed validation/duplicate.

Firmware bytes, complete disassembly and ROM-derived workpapers remain private.
