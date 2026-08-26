# System 3 filesystem ABI — research notes

This document tracks reconstruction of the System 3 / OS3K file subsystem. Unlike the dialog family, several names in this range originated in reverse-engineering hypotheses and must not be treated as public SDK contracts until handler behavior and real callers agree.

## Current BetaWise names under review

The current syscall table contains research-derived names in this range:

```text
A198  FileWriteBuffer
A19C  FileReadBuffer
A1A0  SYS_A1A0      // old comment: get current file number?
A1A8  SYS_A1A8      // old comment: get file count?
A1C0  FileSetFolder
A1C4  SYS_A1C4      // old comment: set file properties... also name?
A1C8  FileOpen
A1CC  FileClose
```

The names are useful provenance, but each contract is being re-established from firmware, historical source and official callers.

Independent `neo-re` work currently labels `A1C8` as `query_object_metric` and `A1CC` as `commit_editable_buffer`. Its argument-count analysis is useful comparative evidence, but direct System 3 firmware shows that `A1C8` is not merely a passive metric query: it resolves a descriptor, replaces the active descriptor, initializes the selected context and optionally resets its stream position.

## Historical AS3000 lineage

The original April 2000 `FileModule.c` describes a per-file structure containing at least:

```text
maximum size
current size
old size
cursor position
row-one display position
read-only state
storage pointer
```

It also exposes an explicit current-file model:

```c
UInt8 FileGetCurrentFile(void);
OSStatus FileSetCurrentFile(UInt8 file_number);
```

The historical implementation stores an internal zero-based current-file index and exposes a one-based file number. It does **not** contain functions literally named `FileOpen` or `FileClose`.

The later OS3K descriptor should not be assumed to have the same binary layout as the 2000 structure. Historical source is used for genealogy and terminology, while modern field offsets come from the modern handlers themselves.

## Modern descriptor fields established directly

The November 2005 AS3000 and NEO firmware expose the same logical descriptor layout at the fields needed by the calls studied so far:

```text
+0x00  storage/data pointer
+0x04  current content size
+0x14  current stream/file position
+0x44  16-bit descriptor identifier
```

The `+0x14` interpretation is confirmed by `A19C`: the read handler addresses `storage + position`, limits the operation to `size - position`, then increments the field by the number of bytes actually read.

This is an important correction to an earlier hypothesis: `descriptor + 0x14` is **not** the read-only field from the historical structure.

## Common descriptor resolver

`A1C4`, `A1C8`, `A19C` and nearby operations use a common internal resolver.

For `A1C8` the resolver receives:

```text
input:  low 16 bits of requested identifier
out 1: 32-bit status/result value
out 2: 16-bit resolved descriptor identifier
return: descriptor pointer, or no descriptor on failure
```

Identifier `0` has special meaning in the resolver: it addresses the current active descriptor rather than an ordinary numbered object.

The resolver also contains a direct special case for identifier `0x00CB`, whose descriptor pointer comes from a dedicated global rather than the ordinary lookup path. An official `AlphaQuiz` caller invokes `A1C8(0x00CB, 1)`. This strongly suggests a special system file/object, but it is not yet named as clipboard or any other semantic object until the constant is independently identified.

Observed negative resolver/status values include at least `-1` and `-7`; their exact symbolic meanings remain open.

## A19C — FileReadBuffer

### Handler locations

```text
AS3000: A19C -> 0x4E155A
NEO:    A19C -> 0x5E3EB6
```

The handler resolves the first argument to a descriptor and performs a bounded sequential read.

Conceptually:

```c
remaining = descriptor->size - descriptor->position;
actual = min(requested, remaining);
memcpy(dst, descriptor->storage + descriptor->position, actual);
descriptor->position += actual;
return actual;
```

A null destination follows an explicit `-6` error path. Resolver errors can also propagate.

The current research-level shape is therefore:

```c
/* research shape only — not yet public API */
int32_t FileReadBuffer(uint16_t file_id, void *dst, uint32_t requested);
```

Confidence:

- three-argument calling shape: **A**;
- first argument is a resolvable 16-bit file/object identifier: **A**;
- sequential position semantics at `+0x14`: **A**;
- read is clipped to remaining size: **A**;
- return is actual byte/character count or negative error: **A for mechanics**, exact public error enum pending;
- legacy name `FileReadBuffer`: **A-/B+**, strongly consistent with the handler and likely suitable for promotion after the paired write contract is closed.

## A198 — FileWriteBuffer

### Handler locations

```text
AS3000: A198 -> 0x4E1196
NEO:    A198 -> 0x5E3AF2
```

Direct firmware establishes four arguments and a descriptor-backed write/update operation. The fourth argument is byte-sized and has distinct cases for at least values `1`, `2` and `3`. The handler updates descriptor storage/size/position according to that mode.

Independent official-caller analysis also finds the stable sequence:

```text
A190(id, 0, 2)
A198(id, src, len, 1)
```

and `AlphaQuiz` contains a helper that performs:

```text
A190(0, 0, 2)
A198(0, &byte, 1, 1) repeatedly
```

This strongly supports the legacy `FileWriteBuffer` family interpretation, but the symbolic meanings of modes `1`, `2` and `3` are still being reconstructed before a public prototype or enum is introduced.

## A1C8 — active-file/context selection

### Handler locations

```text
AS3000: A1C8 -> 0x4E1B18  (file offset 0x21B18)
NEO:    A1C8 -> 0x5E4474  (file offset 0x24474)
```

The handlers are structurally equivalent apart from relocated RAM/ROM addresses and helper targets.

### Calling shape

Direct stack analysis establishes two arguments:

```text
argument 0: 16-bit descriptor/file identifier
argument 1: byte-sized mode; official callers use 0 and 1
```

The successful path:

1. resolves argument 0 to a descriptor;
2. obtains a 32-bit status/result from the resolver;
3. invokes `A1CC`, clearing the previous active descriptor;
4. initializes the resolved descriptor/context;
5. stores the resolved descriptor pointer as the current active descriptor;
6. if argument 1 is `1`, clears `descriptor->position` at `+0x14`;
7. returns the 32-bit resolver status/result in `D0`.

The active-descriptor globals are relocations of the same logical state:

```text
AS3000: 0x00003442
NEO:    0x0000349A
```

### Second argument: position reset

Because `A19C` independently establishes `+0x14` as the sequential file position, the second argument can now be characterized mechanically:

```text
mode == 0: preserve current position
mode == 1: reset current position to zero
```

Official callers use both values. This raises the semantic confidence substantially above the earlier unnamed-mode state. A public name such as `reset_position` is plausible, but is withheld until the whole open/select contract is packaged consistently with the surrounding file API.

### Return contract

An earlier working note incorrectly treated the `A1C8` return as the descriptor identifier. Direct reconstruction of the local stack variables corrects that: `A1C8` returns the **32-bit status/result output from the resolver**, while the descriptor identifier is a separate 16-bit resolver output.

A particularly strong `AlphaWord Plus` caller invokes `A1C8(..., 1)`, then executes a signed negative test on `D0` and branches to failure when the result is below zero. Thus:

```text
return < 0  -> failure
return >= 0 -> accepted/success path
```

Exact nonnegative values and symbolic negative error names remain to be classified.

Current research shape:

```c
/* research shape only — not yet public API */
int32_t SYS_A1C8(uint16_t file_id, uint8_t reset_position);
```

### Official caller evidence

Recovered calls include:

```text
AlphaWord Plus:
  A1C8(d7, 0)
  A1C8(0, 1)
  A1C8(d7, 1)   // followed by signed negative-result failure test
  A1C8(d7, 1)

KeyWords:
  several A1C8(d6, 0) calls

Accelerated Reader:
  A1C8(d0, 1)

AlphaQuiz:
  A1C8(0x00CB, 1) among several A1C8 callers
```

Representative `AlphaWord Plus` workflows place `A1C8` directly beside other file operations, including `A1C4`, `A198`, `A1A0`, `A1D0` and later file-context calls. This direct use is much more consistent with open/select/activate-file semantics than with a passive scalar metric query.

Confidence:

- two-argument ABI: **A**;
- first argument is a 16-bit descriptor/file identifier: **A**;
- installs the resolved descriptor as active: **A**;
- argument 1 values `0/1` mean preserve/reset sequential position mechanically: **A**;
- signed status/result return with negative failure: **A for sign contract**, symbolic result enum pending;
- legacy public name `FileOpen`: **B+ working name**, very plausible but deliberately not promoted yet.

## A1CC — release active context

### Handler locations

```text
AS3000: A1CC -> 0x4E1C48  (file offset 0x21C48)
NEO:    A1CC -> 0x5E45A4  (file offset 0x245A4)
```

Both handlers reduce to:

```c
current_active_descriptor = NULL;
return;
```

No stack argument is consumed.

The legacy `FileClose` name is therefore strongly supported as a conceptual close/release-current-context operation. Whether it performs every semantic action expected from a high-level `close` remains a naming question, because the trap itself only clears the active descriptor pointer.

Confidence:

- zero arguments: **A**;
- clears active descriptor: **A**;
- `FileClose` public name: **B+** pending final API packaging.

## A1A8 — likely current-file identifier, not file count

The current BetaWise comment says:

```c
SYS_A1A8(); // get file count?
```

Direct handler analysis contradicts that hypothesis.

`A1A8` calls the same resolver using identifier `0`, i.e. the current active descriptor path. When a descriptor is available, it obtains the resolver's 16-bit descriptor identifier (`descriptor + 0x44`) and returns that identifier zero-extended.

Mechanically it is therefore a current-active-object identifier query, closely matching the historical role of `FileGetCurrentFile()` even though the modern identifier is 16-bit and can represent more than the simple early file-number range.

Official callers were found in `AlphaWord Plus`, `AlphaQuiz` and `KeyWords`.

Current status:

```text
old "get file count?" comment: rejected
current-file/current-descriptor-id interpretation: A for mechanics, B+ for final public naming
```

Do not rename the stub until the modern identifier namespace is mapped sufficiently to choose between `FileGetCurrentFile`, `FileGetCurrentId` or a more accurate name.

## A1A0, A1C0 and A1C4 warnings

Nearby legacy comments should not be trusted by continuity:

- `A1A0` is too complex to be a simple current-file-number getter; it resolves the current descriptor and manipulates file state.
- `A1C0` consumes at least two 32-bit values and is more complex than a naïve one-argument `FileSetFolder` interpretation.
- `A1C4` uses the common resolver and writes four 32-bit descriptor fields at `+0x24`, `+0x28`, `+0x2C` and `+0x30`; it clearly configures descriptor metadata/properties, but their semantic field names are not yet closed.

These stubs stay unchanged until independently reconstructed.

## Immediate conclusions

The filesystem model is now coherent enough to state the following without relying on legacy names alone:

```text
identifier/token -> resolver -> descriptor
                          |
                          +-- storage pointer
                          +-- current size
                          +-- sequential position
                          +-- 16-bit descriptor id

A1C8: select/activate descriptor, optionally position=0
A1CC: clear active descriptor
A19C: bounded sequential read, position advances
A198: write/update descriptor data, mode-dependent
A1A8: return active descriptor id
```

This is compatible with the historical current-file model while demonstrating that OS3K expanded the identifier space and descriptor internals.

## Next reconstruction steps

Before exposing this family in `os3k.h`:

1. finish `A198` mode `1/2/3` semantics and its return/error contract;
2. identify the special resolver id `0x00CB` from original constants or unambiguous caller behavior;
3. classify `A1C4` descriptor fields `+0x24..+0x30`;
4. determine exact symbolic meanings of resolver/A1C8 errors (`-1`, `-7`, etc.);
5. decide stable public naming for `A1C8`, `A1CC` and `A1A8`;
6. then add raw prototypes/wrappers and a controlled file regression probe.

## SDK rule

No public prototype is added for `A1C8`, `A1CC` or `A1A8` yet. Existing `syscall.c` names are intentionally left untouched while reconstruction is in progress so historical provenance remains visible and speculative renames do not become a second source of false certainty.
