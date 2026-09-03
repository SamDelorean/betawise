# SYS_A1B8 / A1B8 — active-file resize/state operation

Status: mechanically and semantically closed from the three canonical System 3 generations. No reliable modern vendor symbol or original command names were recovered, so the neutral `SYS_A1B8` name and numeric command values are retained.

## Contract

```c
uint32_t SYS_A1B8(int32_t size_or_command);
```

The operation works on the currently active File API descriptor (token `0`). For an ordinary non-negative size it clamps the requested size to `max_size`, stores the effective value as `current_size`, clamps the cursor to the resulting EOF when necessary, synchronizes registered live mirrors, and raises `previous/recoverable_size` when the new current size exceeds it. The returned value is the effective size.

Descriptor fields used by this operation are:

- `+0x04`: current size
- `+0x08`: previous/recoverable size
- `+0x0C`: maximum size
- `+0x10`: minimum allocation / `min_size`
- `+0x14`: cursor

## Special commands

The firmware contains dedicated branches for the following signed values:

- `-2`: use `previous/recoverable_size` as the requested size, then follow the common resize path.
- `-3`: copy `current_size` to `previous/recoverable_size`, retain the current size as the requested size, then follow the common path.
- `-4`: snapshot path. When no active descriptor exists, iterate the descriptor table and copy each descriptor's `current_size` to `previous/recoverable_size`, then return `0`. If an active descriptor already exists, the global snapshot is skipped and the resolver-derived auxiliary result is preserved; no higher-level meaning is assigned to that case.
- `-5`: set `min_size = max(current_size, 0x200)`, retain current size as the requested size, then follow the common path.
- `-6`: use `max(previous/recoverable_size, current_size + 40)` as the requested size before the common clamp.

There is no dedicated `-1` branch. Because the common maximum-size comparison is unsigned, `-1` reaches the common path as a large unsigned value and is therefore clamped to `max_size`. Other unrecognized negative values have the same mechanical property; this is not promoted to a named command family.

The `+0x10` field is identified as `min_size` by the later A1E8 reconstruction. Older provisional descriptions of it as a generic accounting/reservation threshold are superseded.

## Evidence summary

The public handler is exactly 0x102 bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). Only seven raw bytes differ among the three bodies, all attributable to relocated resolver/global/helper references and diagnostic metadata; after normalizing those operands the handlers are identical.

The complete firmware contains 28 direct absolute internal calls in AS3000 2005, 28 in NEO 2005, and 29 in NEO 2013, in addition to the A-line vector. Earlier SmartApplet correlation found AlphaWord Plus using command `-2`, ordinary constants such as `0`, `0x74`, and `0x290`, and dynamic sizes; AlphaQuiz and KeyWords also use dynamic sizes. No SmartApplet caller with literal `-3`, `-4`, `-5`, or `-6` has been established, although those branches are unambiguous in firmware.

## Confidence

- **CONFIRMED:** ABI `uint32_t(int32_t)`; active-descriptor resolution; ordinary resize/max clamp; cursor clamp; live-mirror synchronization; previous-size update; command mechanics for `-2` through `-6`; absence of a dedicated `-1` branch; equivalent behavior in all three compared firmware generations.
- **STRONG INFERENCE:** this is an active-file resize/state-maintenance primitive.
- **UNKNOWN:** original vendor symbol, original names for the negative commands, and the complete resolver-derived result catalog when no active descriptor resolves.

Static regression against the three canonical images: **74/74 PASS**. Dynamic/emulator verification is specified but not executed; because this call mutates file state, such testing should use disposable controlled state.
