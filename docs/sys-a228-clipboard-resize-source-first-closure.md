# SYS_A228 — clipboard resize/state update (source-first closure)

## Status

`A228 / index 138` is closed at **confidence A** under the renewed source-first audit.
No reliable original vendor symbol was recovered, so the generic `SYS_A228`
identifier is intentionally retained.

```c
uint32_t SYS_A228(uint32_t requested_size);
```

The existing declaration in `os3k/file_clipboard_state.h` is therefore confirmed
without an ABI change.

## Contract

`SYS_A228` requests a clipboard backing-store allocation/resize for
`requested_size`, then derives the effective logical size from the capacity that
actually results:

```c
effective = min(requested_size, resulting_max_size);
```

The comparison is unsigned. The handler stores `effective` into the clipboard
size-state fields at descriptor offsets `+0x08` and `+0x04`, invokes the same
private final synchronization/trace path used by neighboring clipboard
mutators, and returns the value stored at `+0x04`.

It does **not** copy or initialize clipboard payload bytes, change the clipboard
tag, or guarantee that allocated capacity becomes exactly `requested_size`.

## Source correlation

The source-first pass did not recover an independent vendor name from the
historical SDK/documentation anchors. Earlier reconstructed clipboard headers
already proposed this signature, but that proposal was treated only as a search
hypothesis until direct firmware verification.

Neighboring closures provide independent structural anchors:

- A224 identifies descriptor `+0x0c` as clipboard `max_size`;
- A20C/A214 establish the same dedicated clipboard descriptor and the same
  private allocation/reclamation helper used here; and
- the File API reconstruction associates `+0x04`/`+0x08` with clipboard size
  state.

These correlations support field naming, while the A228 contract itself is
established from the handler and callers.

## Firmware verification

The handler was re-extracted independently from the canonical November 2005
AlphaSmart 3000 ROM, November 2005 NEO ROM and July 2013 NEO/System 3.15 ROM.
All three canonical SHA-256 values were rechecked before analysis.

A228 is exactly `0x4c` bytes in all three generations. The implementations are
structurally equivalent apart from relocated absolute globals/helper targets.
Each handler:

1. loads one 32-bit `requested_size` ABI argument;
2. calls the clipboard allocation/reclamation helper with the descriptor and
   requested size;
3. reads resulting `max_size` from descriptor `+0x0c`;
4. uses an unsigned comparison to clamp requested size to that capacity;
5. writes the effective size to descriptor `+0x08` and `+0x04`;
6. calls the established private final synchronization/trace helper; and
7. returns descriptor `+0x04` in `D0.L`.

The next handler, A22C/index139, starts exactly at the byte following this
`0x4c`-byte body in all three ROMs.

## Caller evidence

A complete-ROM absolute-JSR search found **3 / 3 / 3** direct references to
A228.

A matched caller sequence in all three generations first obtains the raw
clipboard length via A21C with selector `1`, preserves that value, calls A228
with a requested new size, and compares A228's return value with the requested
size. Its failure/restoration path calls A228 again using the previously saved
raw length. A second matched path calls A228 when the desired size differs from
the current state.

This caller behavior independently confirms that the return is the achieved
size, not a Boolean/status value or an opaque capacity handle.

## Refutation

Alternative interpretations were checked against the full handler and callers:

- **capacity getter** is excluded because A228 receives an argument, calls the
  allocator and mutates size state;
- **direct `max_size` setter** is excluded because A228 never writes `+0x0c`;
  capacity is produced by the allocation helper and then read back;
- **logical-field setter with no allocation effect** is excluded because the
  allocation/reclamation helper runs before the size fields are committed;
- **always returns `requested_size`** is excluded because the code explicitly
  clamps against resulting capacity and callers compare returned versus
  requested size; and
- a signed-size contract is unsupported because clipping uses unsigned branch
  semantics and no sign-error guard exists.

## Confidence classification

**CONFIRMED:** `uint32_t(uint32_t)` ABI, `0x4c` handler length 3/3, allocation
helper call, resulting-capacity read at `+0x0c`, unsigned clamp, writes to
`+0x08`/`+0x04`, returned effective size, 3/3/3 direct callers and
cross-generation structural equivalence.

**STRONG INFERENCE:** the fine-grained labels “allocation/reclamation helper”
and “synchronization/trace helper”, based on their already-correlated behavior
through the surrounding File/clipboard subsystem.

**UNKNOWN:** original vendor name and the allocator's complete internal policy
for changing physical backing capacity.

## Regression state

Static regression: **EXECUTED — 48/48 PASS** across the three canonical ROMs.
The harness covers hashes, boundaries, `0x4c` length, ABI argument load,
descriptor relocation, both helper calls, `+0x0c` capacity read, unsigned
clipping, both size-field writes, return path and the 3/3/3 direct-JSR count.

Dynamic emulator/hardware regression: **SPECIFIED / NOT EXECUTED**.

A minimal dynamic regression should exercise requested sizes below, equal to
and above available capacity; verify the returned effective size and the two
size-state fields; and confirm payload bytes and clipboard tag are not changed
by A228 itself.

The next sequential audit target is A22C/index139. Higher A500+ work remains
preserved and paused by project policy.
