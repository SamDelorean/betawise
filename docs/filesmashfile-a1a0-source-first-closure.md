# A1A0 / index104 — FileSmashFile

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered ABI

```c
uint16_t FileSmashFile(void);
```

The modern System 3 handler takes no caller arguments. It resolves token `0`, the already-established special identifier for the currently active file descriptor.

The public name `FileSmashFile` is strongly supported by the historical AlphaSmart `FileModule.c` / `FileModule.h` lineage, but exact modern textual-name continuity remains a strong inference rather than a recovered symbol. The function and ABI behavior are directly confirmed in firmware.

## Confirmed destructive behavior

If an active descriptor resolves, the handler performs the equivalent of:

```text
memset(descriptor->storage, 0, descriptor->max_size)
descriptor->cursor = 0
descriptor->recoverable_or_old_size = 0
descriptor->current_size = 0
synchronize_live_mirrors(descriptor)
return zero_extend(active_file_token)
```

The zero-fill spans the entire configured storage capacity (`max_size`), not merely the current logical file length.

If no active descriptor resolves, the handler returns zero without modifying storage.

## Correction of inherited BetaWise commentary

The historical BetaWise comment near A1A0, `get current file number?`, is incorrect. A1A0 is destructive. The separate A1A8 syscall is the pure current-file query.

This also distinguishes A1A0 from the older `FileClearFile` behavior. Historical `FileClearFile` preserved the previous/recoverable size for recovery, while A1A0 explicitly clears both the current and recoverable/old-size fields. Its mechanical lineage is therefore with `FileSmashFile`.

## Return value

Modern firmware constructs a 16-bit canonical file token and returns it in `D0`, even though several historical SmartApplet callers ignore the value. The modern ABI is therefore not mechanically `void`.

## Caller correlation

Previously reconstructed official 2012 callers include two AlphaWord Plus uses and two KeyWords uses. Their surrounding flows are consistent with destructive reset, including an AlphaWord sequence that selects/resets a file context, invokes A1A0, then writes replacement content through `FileWriteBuffer`.

No direct absolute `JSR` to the A1A0 entry point occurs inside the canonical OS ROMs; external callers use the A-line ABI.

## Handler boundary and generation comparison

The public handler is exactly `0x5C` bytes in AS3000 2005, NEO 2005 and NEO 2013, with the first `RTS` at the end of that vector-to-vector span. The three implementations preserve the same control flow and descriptor-field mutations. Raw differences are limited to private-call relocations and diagnostic metadata.

## Regression status

Static regression: **EXECUTED — 53/53 PASS**.

It verifies canonical ROM hashes, vector pairing with A1A4, handler boundary, active-token resolver path, successful-descriptor branch, max-size zero-fill setup, storage pointer use, explicit clearing of cursor/old-size/current-size, 16-bit token construction, synchronization call, returned token and three-generation structural equivalence.

Dynamic/emulator regression: **SPECIFIED / NOT EXECUTED**. It must use only disposable data. A future test should initialize known nonzero storage, current size, recoverable/old size, cursor and live mirrors, invoke A1A0, then verify full-capacity zero-fill, all three state fields reset, mirror synchronization and token return. A separate no-active-descriptor case should verify a zero return with no mutation.

## Confidence

- **CONFIRMED:** zero arguments, active descriptor resolution, full-capacity zero-fill, clearing of cursor/recoverable/current sizes, descriptor synchronization, 16-bit token return, handler boundary and three-generation equivalence.
- **STRONG INFERENCE:** continuity of the public name `FileSmashFile` from historical AlphaSmart source.
- **UNKNOWN:** exact modern textual names of private synchronization and diagnostic helpers.

Raw firmware, private offsets and disassembly remain outside the public repository.