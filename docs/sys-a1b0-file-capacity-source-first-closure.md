# SYS_A1B0 / A1B0 — per-file remaining-capacity query

Status: mechanically and semantically closed from the three canonical System 3 generations. The original vendor symbol was not recovered, so the neutral `SYS_A1B0` name is retained.

## Contract

```c
uint32_t SYS_A1B0(uint16_t file_id);
```

`file_id` is resolved through the same File API descriptor resolver used by the surrounding file calls. On resolution failure the call returns `0`. On success it returns:

```text
descriptor.max_size - descriptor.current_size
```

This is the unused capacity inside that descriptor's current maximum allocation. It is **not** the global free-space/accounting metric of the device.

The call is read-only: it does not change the descriptor, cursor, file sizes, or registered live mirrors.

For an ordinary resolvable token, the reconstructed neighboring calls provide the useful invariant:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

where ordinary `SYS_A1B4(file_id)` returns the current size and `SYS_A1BC(file_id)` returns the maximum size. Special selector values of `SYS_A1B4` are outside this identity.

## Evidence summary

The A1B0 handler has the same 0x40-byte functional structure in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). After normalizing relocation/diagnostic operands, the three handlers are identical. Each firmware contains four direct absolute internal calls to A1B0 in addition to the A-line vector. Earlier SmartApplet correlation also found consumers in AlphaWord Plus and AlphaQuiz, including use with the reserved clipboard token.

The early `FileModule.c` family confirms the descriptor/storage model but does not provide an equivalent public free/available-space symbol. Therefore names such as `FileGetFreeSpace` or `FileGetAvailableSpace` are deliberately not invented.

## Confidence

- **CONFIRMED:** one 16-bit file token argument in a 32-bit ABI slot; descriptor resolution; failure returns 0; successful return is `max_size-current_size`; read-only behavior; equivalent semantics in all three compared firmware generations.
- **STRONG INFERENCE:** conceptual meaning is remaining/available capacity of one file descriptor.
- **UNKNOWN:** the original modern vendor symbol name.

Static regression against the three canonical images: **74/74 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
