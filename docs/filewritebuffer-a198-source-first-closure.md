# A198 / index102 — FileWriteBuffer

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered ABI

```c
int32_t FileWriteBuffer(
    uint16_t file_id,
    const uint8_t *buffer,
    uint32_t count,
    uint8_t write_mode);
```

`FileWriteBuffer` is the historical BetaWise syscall name at index 102 (`A198`), and direct firmware analysis confirms that it is in fact the System 3 buffered file-write operation.

## Write modes

The raw firmware recognizes exactly three normal mode values:

- `1`: append at the current end of file;
- `2`: insert at the current cursor, shifting the existing suffix forward;
- `3`: overwrite beginning at the current cursor without shifting the suffix.

Any other mode returns `-5` without performing a content write. The descriptive names append/insert/overwrite reflect confirmed behavior; the original vendor enum names have not been recovered.

## Common behavior

The syscall first resolves `file_id` through the System 3 File API descriptor resolver. On resolution failure, descriptor mutation is skipped and the resolver-provided status is returned.

For a valid descriptor, writes are capacity-aware. If a requested write exceeds the available capacity and the descriptor is the active file, System 3 first attempts capacity growth through a private helper. The effective write count is then clamped to the remaining capacity when necessary. This is a partial-write model rather than all-or-nothing failure.

Successful normal calls return the number of bytes actually written.

## Mode-specific state changes

### Mode 1 — append

Data is copied to `storage + current_size`, and `current_size` increases by the effective byte count. The current cursor is not used as the append destination.

### Mode 2 — insert

The suffix from the current cursor through EOF is moved forward to make room, the new bytes are copied into the vacated range, and both `current_size` and the cursor increase by the effective byte count.

### Mode 3 — overwrite

Data is copied at the current cursor. The suffix is not shifted. If the write extends past the previous EOF, `current_size` grows to the new end position. The cursor then advances by the effective byte count.

After a valid write, the descriptor synchronization path updates registered live mirrors. System 3 also raises its recoverable/previous-size state when the new current size exceeds that prior extent.

A zero effective count performs no data copy and returns zero.

## Handler boundary

The A-line vector distance from A198 to A19C is larger than the public syscall body because private helper code is placed between them. The first `RTS` terminates the actual A198 handler; the public handler is `0x1BA` bytes in all three canonical generations. Treating the entire vector-to-vector span as A198 would incorrectly absorb a private routine.

## Caller correlation

Exactly one direct absolute firmware caller was found in each canonical ROM. The equivalent callers pass token zero (the already-established active-file token), a buffer pointer, a byte count, and a byte-sized mode. This independently confirms the four-argument ABI and the active-file use case.

The previously reconstructed clipboard paste path also reaches the A198 write family and independently corroborates the 1/2/3 append/insert/overwrite mode semantics.

## Generation comparison

AS3000 2005, NEO 2005 and NEO 2013 share the same `0x1BA`-byte public control-flow structure. Differences are limited to relocations of private globals/calls and diagnostic metadata. Normalizing only those operands yields the same handler body.

## Regression status

Static regression: **EXECUTED — 100/100 PASS**.

It verifies canonical ROM hashes, vector entries, the real first-RTS handler boundary, four argument slots, descriptor resolution, all three mode branches, capacity arithmetic, append/insert/overwrite destinations and state changes, invalid-mode `-5`, effective-count return, descriptor synchronization, direct caller count/shape and normalized three-generation equivalence.

Dynamic/emulator regression: **SPECIFIED / NOT EXECUTED**. Useful future cases include each mode, zero-length writes, sufficient and insufficient capacity, active versus non-active descriptors, automatic growth, partial writes, invalid mode, invalid token and live-mirror updates.

## Confidence

- **CONFIRMED:** identity, four-argument ABI, modes 1/2/3, append/insert/overwrite behavior, partial-write semantics, size/cursor mutations, invalid-mode error, synchronization, return value, public handler boundary, caller shape and three-generation equivalence.
- **UNKNOWN:** original vendor enum constant names, private capacity-growth helper symbol, and the complete nominal resolver error taxonomy.

Raw firmware, private offsets and disassembly remain outside the public repository.
