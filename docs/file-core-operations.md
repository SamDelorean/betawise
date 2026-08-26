# System 3 core file operations — A1A0 and A1A8

This note closes two neighboring System 3 file syscalls whose inherited BetaWise comments were misleading.

## Evidence basis

The handlers were compared directly in three firmware images:

- AlphaSmart 3000 System 3, November 2005
- AlphaSmart NEO System 3, November 2005
- AlphaSmart NEO System 3, July 2013

For both A1A0 and A1A8, the three implementations are logically identical aside from relocated calls/globals and debug source-line values.

Historical naming evidence comes from the original AS3000 `FileModule.c` / `FileModule.h` from 2000. Modern names are therefore recorded with **behavior/ABI confidence A** and **name-continuity confidence B (strong)** unless a later symbol-bearing source is recovered.

## A1A0 — `FileSmashFile`

Modern research prototype:

```c
uint16_t FileSmashFile(void);
```

### Mechanical contract — confidence A

A1A0 resolves token `0`, which is the current active file descriptor. If no active descriptor resolves, it returns `0`.

On success it performs the equivalent of:

```text
memset(descriptor->storage, 0, descriptor->max_size)
descriptor->cursor = 0
descriptor->previous_size = 0
descriptor->current_size = 0
synchronize_live_mirrors(descriptor)
return zero_extend(descriptor->canonical_file_token)
```

The relevant descriptor offsets are:

```text
+0x00 storage/base pointer
+0x04 current size
+0x08 previous/recoverable size
+0x0C maximum size
+0x14 cursor
+0x44 canonical 16-bit token
```

The modern handler also invokes the same live-mirror synchronization helper used by A1C4 / `FileGetFileInfo`, so registered size/cursor mirrors observe the destructive reset.

### Historical relationship

The original AS3000 `FileSmashFile(void)` zeroes every byte up to `ulMaxSize` and then clears `ulOldSize`, `ulSize`, `ulCursor`, `ulRowOneStart`, and the read-only state. The modern A1A0 handler preserves the defining operation — irreversible zero-fill plus destruction of current/recoverable size and cursor — while the later descriptor layout and return convention have evolved.

The modern routine returns the canonical 16-bit token; the historical function returned `void`. This ABI evolution is explicitly preserved rather than hidden.

### Official callers

Four A1A0 callers were traced in the official 2012 SmartApplet set:

- AlphaWord Plus 3.4: offsets `0x292A`, `0x3BAA`
- KeyWords 3.6: offsets `0x2642`, `0x3A02`

No A1A0 caller was found in AlphaQuiz, Calculator, or ControlPanel, nor in the 2005 Calculator/ControlPanel binaries examined.

The callers do not consume D0 as a meaningful return value, which is consistent with the historical void-style use even though modern firmware supplies the token.

A particularly clear AlphaWord sequence is:

```text
A1C8(0, 1)       select active context and reset cursor
A1A0             smash/reset the active file
A198             FileWriteBuffer
...
A1CC             FileClose
```

KeyWords contains two sequences `A1A0 -> A1CC -> FileSetFolder`, consistent with destructive cleanup before leaving a file namespace.

## A1A8 — `FileGetCurrentFile`

Modern research prototype:

```c
uint16_t FileGetCurrentFile(void);
```

### Mechanical contract — confidence A

A1A8 resolves token `0`, the active descriptor. If resolution fails it returns `0`. On success it zero-extends and returns the descriptor's canonical 16-bit token (`descriptor+0x44`). It does not modify file data or descriptor state.

Equivalent operation:

```text
descriptor = resolve_current_active_descriptor()
if descriptor == NULL:
    return 0
return descriptor->canonical_file_token
```

This is the operation that the old BetaWise comment near A1A0 was trying to describe. The comment `get current file number?` was attached to the wrong syscall; A1A8 is the mechanical match.

### Official callers

Ten A1A8 callers were found in the same official 2012 set:

- AlphaWord Plus 3.4: 7
- AlphaQuiz 1.0: 1
- KeyWords 3.6: 2

Several callers explicitly zero-extend A1A8's returned word and immediately pass it as a file token to A1E0. This confirms that D0 is treated as a 16-bit file identifier/token, not as a size or count.

### Historical relationship

The original `FileGetCurrentFile(void)` returned the active 1-based `UInt8` file number. Modern System 3 has a broader 16-bit token namespace — including reserved tokens such as clipboard `0x00CB` — so the later function should be understood as returning the **current canonical file token**, not necessarily a small sequential user-file number.

The functional lineage and the misplaced BetaWise comment strongly support the public name `FileGetCurrentFile`, while the changed token width/namespace is documented as ABI evolution.

## Naming decision

The project treats these mappings as SDK-ready:

```text
A1A0 -> FileSmashFile
A1A8 -> FileGetCurrentFile
```

Behavior and argument/return mechanics are confidence A. Exact modern textual-name continuity is confidence B because no later symbol-bearing source with these names has been recovered.

A1A0's return value and A1A8's wider token namespace are modern extensions relative to the 2000 source and must remain visible in the SDK documentation.

## Regression guidance

`FileGetCurrentFile` is safe to exercise directly and should return `0` with no active file, or the canonical token of the selected descriptor.

`FileSmashFile` is destructive and must **not** be tested against user data. Emulator-first regression should create/select a disposable descriptor with known nonzero data, current size, previous size, cursor, and live mirrors; call `FileSmashFile`; then verify zero-filled storage, zero size states/cursor, synchronized mirrors, and canonical-token return.
