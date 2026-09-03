# SYS_A1B4 / A1B4 — file size/accounting query family

Status: mechanically and semantically closed from the three canonical System 3 generations. The original vendor symbol and original selector names were not recovered, so the neutral `SYS_A1B4` name and numeric selectors are retained.

## Contract

```c
uint32_t SYS_A1B4(uint16_t file_id_or_selector);
```

The low 16 bits of the ABI slot are interpreted either as an ordinary File API token or as one of four reserved selectors.

For an ordinary token, the call resolves the descriptor and returns `descriptor.current_size`. Resolution failure returns `0`.

Reserved selectors implement aggregate/accounting queries:

- `0xFC`: resolve token `0` (the active descriptor) and return its recoverable/previous size (`+0x08`), or `0` if no active descriptor resolves.
- `0xFD`: sum `current_size` across descriptors in the current file group/folder.
- `0xFE`: sum `min(current_size, min_size)` across descriptors in the current file group/folder.
- `0xFF`: sum `min(current_size, min_size)` across the complete descriptor table.

The descriptor field at `+0x10` is `min_size` / minimum allocation. Earlier provisional descriptions of it as a generic accounting/reservation threshold are superseded by the later A1E8 reconstruction.

The operation is read-only. It does not alter descriptor state, cursor, file sizes, or registered live mirrors.

For an ordinary resolvable token, the neighboring reconstructed calls provide the invariant:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

because the ordinary A1B4 path returns `current_size`, A1B0 returns `max_size-current_size`, and A1BC returns `max_size`.

## Evidence summary

The public A1B4 handler is 0x102 bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). After normalizing only relocated globals/helpers and diagnostic operands, all three bodies are identical. The firmware contains 53 direct absolute internal calls in AS3000, 53 in NEO 2005, and 57 in NEO 2013, consistent with a central File API query primitive.

Historical SmartApplet correlation includes direct use of `0xFC` and `0xFF` by AlphaWord Plus and use of the reserved clipboard token `0x00CB` by multiple applets. The early FileModule source supports the underlying size/old-size/max-size descriptor model but does not expose an equivalent public modern symbol or names for these reserved selectors.

## Confidence

- **CONFIRMED:** ABI `uint32_t(uint16_t)`; ordinary current-size query; `0xFC` active previous/recoverable-size query; `0xFD` group current-size sum; `0xFE` group `min(current_size,min_size)` sum; `0xFF` table-wide equivalent; descriptor field mapping; read-only behavior; equivalent semantics across all three compared firmware generations.
- **STRONG INFERENCE:** the syscall belongs to the File API size/accounting family.
- **UNKNOWN:** original modern vendor function name and vendor names for selectors `0xFC..0xFF`.

Static regression against the three canonical images: **66/66 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
