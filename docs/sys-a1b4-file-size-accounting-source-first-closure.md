# SYS_A1B4 / A1B4 — file size/accounting query family

Status: mechanically and semantically closed from the three canonical System 3 generations. This file is a reconciliation companion to `docs/sys-a1b4-size-accounting-source-first-closure.md`, which is the primary closure record. The original vendor symbol and selector names were not recovered, so the neutral `SYS_A1B4` name and numeric selectors are retained.

## Contract

```c
uint32_t SYS_A1B4(uint16_t file_id_or_selector);
```

For an ordinary resolvable token, the call returns `descriptor.current_size`. The reserved selectors provide additional read-only accounting views:

- `0xFC`: resolve token `0` (the active descriptor) and return its recoverable/previous-size state.
- `0xFD`: sum `current_size` across descriptors in the current file group/folder.
- `0xFE`: sum `min(current_size, min_size)` across descriptors in the current group/folder.
- `0xFF`: sum `min(current_size, min_size)` across the complete descriptor table.

The descriptor field at `+0x10` is `min_size` / minimum allocation; older provisional “accounting threshold” wording is superseded by the A1E8 reconstruction.

For the ordinary path and the `0xFC` path, if descriptor resolution fails, A1B4 preserves the resolver-provided auxiliary result/status rather than forcing a fabricated zero. The complete catalog of resolver error values remains intentionally unspecified.

The operation is read-only. It does not alter descriptor state, cursor, sizes, or registered live mirrors.

For an ordinary resolvable token:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

because ordinary A1B4 returns `current_size`, A1B0 returns `max_size-current_size`, and A1BC returns `max_size`.

## Evidence summary

The public A1B4 handler is 0x102 bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). Direct firmware revalidation confirms identical selector dispatch and aggregate rules modulo relocated private data/call targets and diagnostic metadata. Firmware contains 53 direct absolute internal calls in AS3000, 53 in NEO 2005, and 57 in NEO 2013, in addition to the A-line vector.

Historical SmartApplet correlation includes ordinary file tokens, the clipboard token `0x00CB`, and direct uses of the reserved accounting selectors. Early `FileModule.c` supports the underlying size/old-size descriptor lineage but does not supply a trustworthy modern public symbol for this combined query.

## Confidence

- **CONFIRMED:** ABI `uint32_t(uint16_t)`; ordinary current-size query; `0xFC` active recoverable/previous-size query; `0xFD` group current-size sum; `0xFE` group `min(current_size,min_size)` sum; `0xFF` table-wide equivalent; `min_size` field interpretation; read-only semantics; equivalent behavior across all three canonical firmware generations.
- **STRONG INFERENCE:** the reserved selectors are internal storage/accounting views.
- **UNKNOWN:** original vendor function/selector names and complete resolver error-value catalog.

Canonical static regression: **96/96 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.

## Reconciliation note

An earlier draft of this companion file incorrectly simplified unresolved ordinary/`0xFC` paths to a forced zero and quoted a narrower 66/66 harness. Direct firmware revalidation in the primary closure record supersedes those statements: resolver auxiliary status/result is preserved, and the canonical regression is 96/96 PASS.
