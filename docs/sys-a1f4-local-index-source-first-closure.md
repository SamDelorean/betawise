# SYS_A1F4 local-index get/set — source-first closure

Status: mechanically and semantically closed from canonical firmware evidence. The original modern vendor symbol was not recovered, so `SYS_A1F4` is retained.

```c
int32_t SYS_A1F4(uint16_t file_id, uint16_t index_value,
                 uint16_t *index_out);
```

## Source-first correlation

BetaWise maps vector index 125 to `SYS_A1F4` without a reliable vendor name. Historical FileModule material documents the earlier single-byte file-number model, but System 3 uses 16-bit canonical tokens and a separate descriptor-local index, so no early symbol is projected onto this modern ABI.

The prior A1E0–A1FC reconstruction was treated only as a hypothesis. Direct firmware revalidation confirms that descriptor offset `+0x46` is the field accessed by A1F4. The adjacent A1F8 reverse-lookup handler independently compares its argument with descriptor `+0x46` and returns the canonical token from `+0x44` on match, establishing that these are distinct identity fields.

## Canonical firmware confirmation

A1F4 is vector index 125. Entry points:

- AS3000 2005: `0x004E28BE`;
- NEO 2005: `0x005E521A`;
- NEO 2013: `0x0043B61C`.

A1F8 begins exactly `0x44` bytes later in all three images. NEO 2005 and NEO 2013 A1F4 handlers are byte-for-byte identical. AS3000 differs in only one byte, the low byte of the BSR displacement to the common File API resolver; after that relocation is normalized the body is identical.

## Confirmed behavior

A1F4 consumes three 32-bit ABI slots: `file_id` as the low word of slot 1, `index_value` as the low word of slot 2, and `index_out` as the complete pointer in slot 3.

1. Resolve `file_id` through the common File API resolver.
2. Resolver failure returns the resolver status and performs no index read/write.
3. `index_out == NULL` selects **SET**: write `index_value` literally to descriptor `+0x46`.
4. `index_out != NULL` selects **GET**: copy descriptor `+0x46` to `*index_out`; `index_value` is ignored.
5. Successful GET or SET returns `0` in `D0.L`.

The SET path contains no bounds check, normalization, or auxiliary helper for `index_value`.

## Callers

Exactly two absolute direct A1F4 calls exist in each canonical ROM.

- AS3000: `0x004C235A`, `0x004C8D64`;
- NEO 2005: `0x005C2200`, `0x005C85A6`;
- NEO 2013: `0x00412388`, `0x00418D68`.

One caller constructs a SET call with `index_out == NULL`. The other constructs a GET call using a local-word output; an `index_value` of `0x00FF` remains in the reused argument slot but is ignored by the GET branch. Both call sites clean exactly three 32-bit slots after A1F4.

## Refutation

- Refuted a two-argument ABI: the handler loads the third-slot pointer and both callers construct three slots.
- Refuted the intuitive interpretation `NULL == GET`: firmware implements exactly the reverse, `NULL == SET` and non-NULL == GET.
- Refuted an internal range policy for the local index: SET stores the supplied word literally.
- Refuted equating local index with canonical token: A1F8 compares descriptor `+0x46` but returns descriptor `+0x44`.
- No speculative modern vendor symbol is introduced.

## Classification

**CONFIRMED:** three-argument ABI; common File API resolver; descriptor field `+0x46`; NULL=SET/non-NULL=GET; literal-word SET; word GET; success `0`; resolver status on failure; two callers per ROM covering both directions; A1F8 reverse-lookup correlation; cross-generation equivalence.

**INFERENCIA FUERTE:** descriptor `+0x46` is a local file index / reverse-lookup key within the current System 3 file topology.

**DESCONOCIDO:** original modern vendor symbol and exact vendor name for the field.

## Regression

Static primary-byte regression was **EXECUTED: 75/75 PASS** against the three canonical ROMs. It verifies hashes, exact span, ABI stack accesses, common resolver target, status propagation, GET/SET branches, `+0x46` field access, success return, both direct callers, A1F8 `+0x46`/`+0x44` correlation, and cross-generation byte equivalence.

Dynamic regression is **SPECIFIED / NOT EXECUTED** because SET mutates metadata: GET a known descriptor, SET/GET round-trip, values `0x0000`, `0x00FF`, `0xFFFF`, invalid token, failure non-mutation, and restoration of the original value.

Firmware bytes and complete disassembly remain private.
