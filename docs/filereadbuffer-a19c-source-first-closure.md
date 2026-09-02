# A19C / index103 — FileReadBuffer

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered ABI

```c
int32_t FileReadBuffer(
    uint16_t file_id,
    uint8_t *buffer,
    uint32_t count);
```

`FileReadBuffer` is the historical BetaWise syscall name at index 103 (`A19C`). Direct firmware analysis confirms that it is the System 3 buffered file-read operation.

## Common behavior

The syscall resolves `file_id` through the same File API descriptor resolver used by the surrounding file-position and file-write operations. On resolution failure, descriptor mutation is skipped and the resolver-provided status is returned.

For a valid descriptor, a null output buffer returns `-6` before any data copy or cursor advance.

The read source is `storage + cursor`. The available byte count is `current_size - cursor`. If the requested count exceeds the remaining bytes, System 3 clamps the effective read count to that available range. Reads therefore terminate cleanly at EOF rather than failing all-or-nothing.

If the effective count is nonzero, exactly that many bytes are copied to the caller buffer and the descriptor cursor advances by the same amount. The descriptor synchronization path then updates registered live mirrors.

A zero effective count performs no data copy and returns zero. The handler does not append a NUL byte or perform character interpretation; this is a binary buffer operation.

## Return values

- normal read: number of bytes actually read;
- EOF or zero effective count: `0`;
- valid descriptor with `buffer == NULL`: `-6`;
- unresolved `file_id`: resolver-provided status.

## Handler boundary

The public A19C body is exactly `0x7C` bytes in AS3000 2005, NEO 2005 and NEO 2013. The first `RTS` is the final word of that vector-to-vector span, immediately before A1A0. Unlike A198, no private helper body is embedded between A19C and the next syscall vector.

## Caller correlation

No direct absolute `JSR` to the A19C entry point was found inside any of the three canonical OS ROMs, and no aligned A19C A-line opcode occurs inside those ROMs. This is recorded as a negative internal-xref result; external SmartApplets can still invoke the syscall through the A-line ABI.

The surrounding reconstructed File API independently confirms the descriptor fields used here: storage, current size and current cursor, and the same synchronization helper is used by related file mutations.

## Generation comparison

The three canonical generations share the same `0x7C`-byte control-flow structure and descriptor semantics. Raw differences are limited to private-call displacements and diagnostic metadata. No semantic divergence was found.

## Regression status

Static regression: **EXECUTED — 56/56 PASS**.

It verifies canonical ROM hashes, vector pairing with A1A0, handler boundary, three argument slots, descriptor resolution, null-buffer `-6`, source and available-byte calculations, EOF clamping, zero-count handling, copy preparation, cursor advance, descriptor synchronization, effective-count return, negative direct-xref search and three-generation structural equivalence.

Dynamic/emulator regression: **SPECIFIED / NOT EXECUTED**. Future cases should include reads wholly before EOF, exact-to-EOF reads, truncation at EOF, cursor already at EOF, zero count, null buffer, invalid file token, active/clipboard tokens and live cursor-mirror verification.

## Confidence

- **CONFIRMED:** identity, three-argument ABI, descriptor resolver, binary buffer semantics, EOF clamp, null-buffer error, cursor advance, synchronization, effective-count return, public handler boundary and three-generation equivalence.
- **UNKNOWN:** original public names of private copy/synchronization/diagnostic helpers and the complete nominal resolver error taxonomy.

Raw firmware, private offsets and disassembly remain outside the public repository.