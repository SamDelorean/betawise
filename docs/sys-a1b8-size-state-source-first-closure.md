# SYS_A1B8 / A1B8 — active-file size/state operation

Status: mechanically and semantically closed from the three canonical System 3 generations. No trustworthy modern vendor symbol was recovered, so the neutral `SYS_A1B8` name is retained.

## Contract

```c
uint32_t SYS_A1B8(int32_t requested_size_or_command);
```

A1B8 operates on the active File API descriptor. For a non-negative ordinary size it clamps the request to `max_size`, stores the effective `current_size`, clamps `cursor` to the new EOF when required, raises `recoverable_size` when the new size exceeds the previous recoverable extent, synchronizes registered live mirrors, and returns the resulting current size.

The firmware also recognizes negative command values in the same 32-bit slot:

- `-1`: no dedicated branch; unsigned upper clamp makes the effective request `max_size`.
- `-2`: use `recoverable_size` as the requested size, then follow the common clamp/store path.
- `-3`: snapshot `current_size` into `recoverable_size`, preserve the current size, then follow the common path.
- `-4`: global recoverable-size snapshot path. When the associated global gate permits it, iterate the descriptor table and copy each descriptor's `current_size` to `recoverable_size`; return zero without applying a normal active-file resize.
- `-5`: set `min_size = max(current_size, 0x200)` while preserving `current_size`, then follow the common synchronization path.
- `-6`: request `max(recoverable_size, current_size + 40)` before the common upper clamp.

The `+0x10` descriptor field is `min_size`; older provisional “accounting threshold” terminology is superseded by the independently reconstructed A1E8 contract.

If no active descriptor resolves on the normal active-file path, the initialized return value remains zero and no descriptor mutation occurs.

## Evidence summary

The A1B8 handler is exactly 0x102 bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). The three bodies have the same control-flow and field operations; differences are relocated globals/private calls and diagnostic metadata.

Direct firmware revalidation confirms the one 32-bit argument, all negative command compares, the `+40` growth rule, the `0x200` minimum-size rule, the upper clamp against `max_size`, writes to `current_size`, cursor clamp, synchronization helper call, and conditional promotion of `recoverable_size`.

Firmware contains 28 direct absolute internal calls to A1B8 in AS3000 2005, 28 in NEO 2005, and 29 in NEO 2013, in addition to the A-line vector. Earlier File API/source correlation establishes the descriptor field lineage (`current_size`, `recoverable_size`, `max_size`, `min_size`, `cursor`) but does not expose a trustworthy modern public symbol for this combined command interface.

## Confidence

- **CONFIRMED:** one signed 32-bit command/size slot; ordinary resize mechanics; commands `-1..-6` as described; `min_size` correction; cursor clamp; recoverable high-water behavior; live-mirror synchronization; equivalent behavior in all three canonical firmware generations.
- **STRONG INFERENCE:** this is the central active-file size/state control primitive of the modern File API.
- **UNKNOWN:** original modern vendor function name and vendor names, if any, for the negative commands and the `-4` global gate.

Static regression against the three canonical images: **60/60 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
