# SYS_A1BC / A1BC — per-file maximum-size query

Status: mechanically and semantically closed from the three canonical System 3 generations. The original vendor symbol was not recovered, so the neutral `SYS_A1BC` name is retained.

## Contract

```c
uint32_t SYS_A1BC(uint16_t file_id);
```

The call resolves `file_id` through the common File API descriptor resolver. If a descriptor resolves, the return value is that descriptor's `max_size`. If resolution does not produce a descriptor, the handler preserves the resolver-provided auxiliary result/status rather than fabricating a maximum size. The complete resolver error catalog remains intentionally unspecified.

The operation is read-only: it does not alter file sizes, cursor, descriptor state, or registered live mirrors.

For an ordinary resolvable token, the neighboring reconstructed queries provide the invariant:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

because ordinary A1B4 returns `current_size` and A1B0 returns `max_size-current_size`.

## Evidence summary

The public A1BC handler is 0x36 bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). The only generational differences are relocated resolver/diagnostic operands; normalizing them yields an identical body. No direct absolute internal JSR to A1BC was found in any of the three complete firmware images (0/0/0), which is recorded as a negative result rather than interpreted as absence from the public ABI.

Early `FileModule.c` supports the underlying maximum-size descriptor concept but does not provide a trustworthy modern public symbol for this specific call.

## Confidence

- **CONFIRMED:** ABI `uint32_t(uint16_t)`; common File API resolution; successful return of descriptor `max_size`; preservation of resolver auxiliary result on unresolved input; read-only behavior; equivalent semantics across all three canonical firmware generations.
- **STRONG INFERENCE:** conceptual role is a per-file/descriptor maximum-capacity query.
- **UNKNOWN:** original modern vendor function name and complete resolver status/error catalog.

Static regression against the three canonical images: **33/33 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
