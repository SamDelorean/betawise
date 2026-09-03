# SYS_A1B8 / A1B8 — active-file size/state operation

Status: mechanically and semantically closed from the three canonical System 3 generations. This file is a reconciliation companion to `docs/sys-a1b8-active-file-resize-state-source-first-closure.md`, which carries the fuller canonical caller/regression record. No trustworthy modern vendor symbol was recovered, so the neutral `SYS_A1B8` name is retained.

## Contract

```c
uint32_t SYS_A1B8(int32_t requested_size_or_command);
```

A1B8 operates on the active File API descriptor. For a non-negative ordinary size it clamps the request to `max_size`, stores the effective `current_size`, clamps `cursor` to the new EOF when required, raises `recoverable_size` when the new size exceeds the previous recoverable extent, synchronizes registered live mirrors, and returns the resulting current size.

The firmware also recognizes negative command values in the same 32-bit slot:

- `-1`: no dedicated branch. Its unsigned value is above ordinary file capacities, so the common upper clamp yields `max_size`. Other unrecognized negative values have the same mechanical property and are not promoted to named commands.
- `-2`: use `recoverable_size` as the requested size, then follow the common clamp/store path.
- `-3`: snapshot `current_size` into `recoverable_size`, preserve the current size, then follow the common path.
- `-4`: snapshot path. When no active descriptor exists, iterate the descriptor table and copy each descriptor's `current_size` to `recoverable_size`, then return `0`. If an active descriptor already exists, the global snapshot is skipped and the resolver-derived auxiliary result is preserved; no higher-level meaning is assigned to that case.
- `-5`: set `min_size = max(current_size, 0x200)` while preserving `current_size`, then follow the common synchronization path.
- `-6`: request `max(recoverable_size, current_size + 40)` before the common upper clamp.

The `+0x10` descriptor field is `min_size`; older provisional “accounting threshold” terminology is superseded by the independently reconstructed A1E8 contract.

## Evidence summary

The A1B8 handler is exactly 0x102 bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). The three bodies have the same control-flow and field operations; differences are relocated globals/private calls and diagnostic metadata.

Direct firmware revalidation confirms the one 32-bit argument, all dedicated negative command branches, the `+40` growth rule, the `0x200` minimum-size rule, the upper clamp against `max_size`, writes to `current_size`, cursor clamp, synchronization helper call, and conditional promotion of `recoverable_size`.

Firmware contains 28 direct absolute internal calls to A1B8 in AS3000 2005, 28 in NEO 2005, and 29 in NEO 2013, in addition to the A-line vector. Earlier SmartApplet correlation includes command `-2`, ordinary constants `0`, `0x74`, `0x290`, and dynamic sizes; literal `-3` through `-6` callers have not been established even though their firmware branches are unambiguous.

## Confidence

- **CONFIRMED:** ABI `uint32_t(int32_t)`; active-descriptor resolution; ordinary resize/max clamp; cursor clamp; live-mirror synchronization; recoverable-size update; dedicated command mechanics for `-2..-6`; absence of a dedicated `-1` branch; equivalent behavior in all three canonical firmware generations.
- **STRONG INFERENCE:** this is the central active-file resize/state-maintenance primitive of the modern File API.
- **UNKNOWN:** original modern vendor function name, command names, and complete resolver-derived result catalog when no active descriptor resolves.

Canonical static regression: **74/74 PASS**. The narrower 60/60 harness used while this companion was first created remains a valid subset, but the 74/74 regression in the primary closure supersedes it as the closure gate. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
