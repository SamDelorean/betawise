# SYS_A1BC / A1BC — per-file maximum-size query

Status: mechanically and semantically closed from the three canonical System 3 generations. No trustworthy modern vendor symbol was recovered, so the neutral `SYS_A1BC` name is retained.

## Contract

```c
uint32_t SYS_A1BC(uint16_t file_id);
```

A1BC resolves the supplied File API token through the same descriptor resolver used by neighboring A190/A194/A1B0/A1B4 operations. When a descriptor resolves, the return value is exactly `descriptor->max_size` (`+0x0C`). The operation is read-only.

When resolution fails, A1BC preserves the resolver-provided auxiliary result/status rather than dereferencing a null descriptor. The complete catalog of resolver error values remains intentionally unspecified.

For an ordinary resolvable token, the neighboring calls establish:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

because A1B4 returns `current_size` and A1B0 returns `max_size-current_size`.

## Evidence summary

The public handler is exactly 0x36 bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). Direct firmware revalidation confirms one 16-bit file token in a 32-bit ABI slot, the shared resolver call, null-descriptor guard, load from descriptor offset `+0x0C`, and return of the local result in D0.L. No mutation of descriptor state is present.

No direct absolute internal JSR to A1BC was found in any of the three complete canonical firmware images (0/0/0); this negative result is retained rather than treated as evidence against the A-line API. Earlier File API reconstruction independently fixes `+0x0C` as `max_size` and correlates A1BC with A1B0/A1B4 accounting invariants.

## Confidence

- **CONFIRMED:** ABI `uint32_t(uint16_t)`; shared File API resolver; successful return=`max_size`; read-only behavior; resolver-result preservation on failure; equivalent semantics across all three canonical generations.
- **STRONG INFERENCE:** this is the per-file maximum-capacity query primitive.
- **UNKNOWN:** original modern vendor symbol and complete resolver error-value catalog.

Static regression against the three canonical images: **27/27 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
