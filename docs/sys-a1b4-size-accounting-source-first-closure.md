# SYS_A1B4 / A1B4 — file-size and accounting query

Status: mechanically and semantically closed from the three canonical System 3 generations. No reliable modern vendor symbol was recovered, so the neutral `SYS_A1B4` name is retained.

## Contract

```c
uint32_t SYS_A1B4(uint16_t file_id_or_selector);
```

For an ordinary resolvable file token, including the reserved clipboard token handled by the common File API resolver, the call returns the descriptor's current file size. The call is read-only.

Four reserved selector values bypass the ordinary query path:

- `0x00FC`: resolve the active descriptor and return its recoverable/previous-size state.
- `0x00FD`: sum current sizes for the descriptors in the current group.
- `0x00FE`: sum `min(current_size, min_size)` for the descriptors in the current group.
- `0x00FF`: sum `min(current_size, min_size)` over the complete descriptor table.

The `min_size` interpretation is the corrected descriptor model established independently by later File API reconstruction; older notes that called this field an accounting threshold are superseded.

For the ordinary path, failure preserves the resolver-provided auxiliary status/result instead of fabricating a file size. The same applies to the `0x00FC` active-descriptor resolution path. The exact complete catalog of resolver error values remains intentionally unspecified here.

For an ordinary resolvable token the neighboring calls provide the invariant:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

where `SYS_A1B0` returns remaining per-descriptor capacity and `SYS_A1BC` returns maximum size.

## Evidence summary

The public handler is 0x102 bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). Direct firmware revalidation confirms the same selector dispatch, group/all-descriptor accumulation rules, ordinary descriptor resolver path, and read-only behavior in all three generations. Platform differences are limited to relocated private data/call targets and diagnostic metadata; no semantic divergence was found.

The full firmware contains 53 direct absolute internal calls to A1B4 in AS3000 2005, 53 in NEO 2005, and 57 in NEO 2013, in addition to the A-line vector. Existing SmartApplet correlation includes ordinary file tokens, clipboard queries, and direct uses of the reserved accounting selectors.

Early `FileModule.c` material supports the size/old-size descriptor lineage but does not provide a trustworthy modern public symbol for this combined query. Therefore a descriptive vendor-like name is not invented.

## Confidence

- **CONFIRMED:** one 16-bit token/selector argument in a 32-bit ABI slot; ordinary current-size query; `0xFC` recoverable/previous-size query for the active descriptor; `0xFD`, `0xFE`, and `0xFF` aggregate behavior; use of `min_size`; read-only semantics; equivalent behavior across all three canonical firmware generations.
- **STRONG INFERENCE:** the reserved selectors form internal storage/accounting views used by System 3 applications.
- **UNKNOWN:** original modern vendor symbol and vendor names, if any, for the four reserved selectors; complete resolver error-value catalog.

Static regression against the three canonical images: **96/96 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
