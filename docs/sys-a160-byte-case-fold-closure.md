# A160 / index88 — SYS_A160 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered raw contract

```c
uint8_t SYS_A160(uint8_t c);
```

Only the low return byte is contractual. The handler consumes the low byte of the first 32-bit ABI argument slot and several return paths write only `D0.B`.

No original vendor function name has been recovered, so the neutral `SYS_A160` name is retained.

## Relationship to the historical note

BetaWise describes this syscall as “tolower, handles international chars”. Firmware confirms the functional idea, with an important distinction: `A160` is **not** the standard C-library `tolower(int)` service. OS3K has a separate standard-library `tolower` syscall later in the ABI.

`A160` is a byte-oriented case-fold/lowercase primitive for the AlphaSmart character set.

## Confirmed algorithm

For input byte `c`:

1. ASCII `A` through `Z` return `c + 0x20`.
2. ASCII `a` through `z` return unchanged.
3. Other bytes are searched in a fixed 22-entry source table.
4. A match returns the byte at the same index in a 22-entry destination table.
5. A byte absent from the table returns unchanged.

There is no locale argument and no dynamic locale/global lookup in the public handler.

## Extended-character tables

AS3000 2005 and NEO 2005 use the same extracted mapping table.

NEO 2013 contains a real table difference: source-table position 13 contains `0xA7` instead of the 2005 value `0xA6`, while the destination at that position remains `0xA7`.

Consequently:

- AS3000 2005: `0xA6 -> 0xA7`
- NEO 2005: `0xA6 -> 0xA7`
- NEO 2013: `0xA6` is not mapped by that entry and therefore remains unchanged

The proprietary character set is deliberately described byte-wise here; Unicode names are not assigned without source evidence.

## Caller correlation

No direct absolute `JSR` caller to the `A160` handler was found in any of the three canonical ROMs.

The handler address has exactly three raw references per ROM:

- the A-line vector-table entry;
- one internal pointer consumer in the already reconstructed `A078` case-folded comparison/search path;
- one internal pointer consumer in the already reconstructed `A08C` string-comparison path.

Those callers invoke the helper indirectly, which explains the absence of a direct `JSR` xref while independently confirming its case-fold role.

An AlphaWord Plus 2005 scan finds `A160` only in the normal contiguous syscall-stub table and establishes no additional inline caller.

## Generation comparison

The public handler is `0x4E` bytes in AS3000 2005, NEO 2005 and NEO 2013. After neutralizing only the relocated source/destination table pointers, the handler code is identical across all three generations.

Semantic equivalence is nevertheless not complete over the entire byte domain because the NEO 2013 table content differs as described above.

## Regression status

Static regression: **EXECUTED — 74/74 PASS**.

The dedicated suite verifies canonical ROM identities, vector resolution, handler bounds, byte ABI, ASCII behavior, all 22 extracted mapping pairs, table sentinels, normalized code equivalence, internal pointer consumers, the 2005/2013 `0xA6` delta and the AlphaWord Plus stub-table negative cross-check.

A first test-authoring pass had one false failure because the expected AlphaWord stub window was one neighboring-stub pair too short; correcting that expected slice and rerunning the assertion set produced the final 74/74 result without changing any firmware conclusion.

Dynamic exhaustive `0x00..0xFF` regression: **SPECIFIED / NOT EXECUTED**.

## Confidence

- **CONFIRMED:** one-byte ABI, low-byte result, ASCII uppercase folding, lowercase identity, 22-entry extended lookup, fallback identity, caller relationships and generation-specific table delta.
- **STRONG INFERENCE:** descriptive role as AlphaSmart byte case-fold/lowercase service.
- **UNKNOWN:** original vendor function name and formal source-character names/encoding for every extended byte.

Raw firmware, complete private tables/disassembly and audit corpus remain outside the public repository.
