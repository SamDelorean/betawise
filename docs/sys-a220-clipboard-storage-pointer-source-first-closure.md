# SYS_A220 — clipboard storage pointer (source-first closure)

## Status

`A220 / index 136` is closed at **confidence A** under the source-first audit.
The original vendor name has not been recovered, so the generic `SYS_A220`
identifier is intentionally retained.

```c
uint8_t *SYS_A220(void);
```

## Contract

`SYS_A220` returns the 32-bit storage/base pointer held at offset `+0x00` of the
dedicated System 3 clipboard descriptor.

It:

- receives no arguments;
- performs no helper calls;
- does not modify clipboard state;
- performs no explicit validation or NULL guard; and
- returns the descriptor storage pointer directly.

The existing public declaration in `os3k/file_clipboard_state.h` is therefore
confirmed by the renewed source-first audit and does not require an ABI change.

## Source correlation

BetaWise `syscall.c` identifies index 136 only as `SYS_A220`; no reliable public
name for this trap was recovered from the reviewed SDK/documentation anchors.
Earlier project notes associated A220 with the clipboard descriptor, but those
notes were treated only as search anchors and were revalidated against canonical
firmware before closure.

## Firmware verification

The handler was independently re-extracted from the canonical November 2005
AlphaSmart 3000 ROM, November 2005 NEO ROM and July 2013 NEO/System 3.15 ROM.
The canonical ROM hashes were rechecked before analysis.

Across all three generations the handler is `0x0a` bytes and has the same
three-instruction semantics: load the generation-specific clipboard descriptor,
load its first longword into the return register, and return. Only the absolute
descriptor address relocates between generations.

The immediately following A224 handler uses the same descriptor and reads
`+0x0c`, independently preserving the structure boundary between storage pointer
and maximum allocated capacity.

## Caller evidence

A complete direct-JSR scan of each canonical ROM found **11 / 11 / 11** direct
A220 callers. In **9 / 11** callers per ROM, the return value is immediately
moved to an address register, strongly corroborating pointer semantics.

Each generation also contains two direct sequences that obtain A220, consume the
result as an address, and then query A21C with raw selector `1`. This establishes
the operational pairing of clipboard storage base plus raw logical length.

## Refutation

Alternative interpretations as a size/capacity getter, descriptor token, or
state-changing operation were rejected. The handler reads descriptor `+0x00`
verbatim, while A224 reads `+0x0c`; callers consume A220 as an address; and the
handler contains no argument or write path.

## Regression state

Static regression: **EXECUTED — 39/39 PASS** across the three canonical ROMs.
Checks cover canonical hashes, handler bounds and structure, descriptor
relocations, the A224 boundary, direct xref counts and pointer-like caller use.

Dynamic emulator/hardware regression: **SPECIFIED / NOT EXECUTED**.

The next sequential audit target is A224/index137. Higher A500+ work remains
preserved and paused by project policy.
