# System 3 filesystem ABI — research notes

This document tracks reconstruction of the System 3 / OS3K file subsystem. Unlike the dialog family, these names are still under active review and must not be treated as public SDK contracts until their handlers and callers agree.

## Current BetaWise names under review

The current syscall table contains research-derived names in this range:

```text
A198  FileWriteBuffer
A19C  FileReadBuffer
A1C0  FileSetFolder
A1C8  FileOpen
A1CC  FileClose
```

These names originated in earlier BetaWise reverse engineering. They are useful hypotheses, not sufficient evidence by themselves.

A newer independent `neo-re` analysis labels `A1C8` as a two-argument scalar query (`query_object_metric`) and `A1CC` as a zero-argument editable-buffer finalizer. Because those interpretations conflict with the older BetaWise names, this SDK will resolve the handlers directly before promoting either naming scheme.

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

The historical API includes `FileGetCurrentFile`, `FileSetCurrentFile`, `FileGetFileInfo`, editing/navigation operations, and clipboard storage. It does **not** expose historical functions named `FileOpen` or `FileClose`.

That absence does not disprove later OS3K open/close semantics; it means the later ABI must be established independently rather than projected backwards from the early source.

## A1C8 direct firmware evidence

### Handler locations

In the November 2005 System 3 images:

```text
AS3000: A1C8 -> 0x4E1B18  (file offset 0x21B18)
NEO:    A1C8 -> 0x5E4474  (file offset 0x24474)
```

The two handlers are the same size (`0x130` bytes) and are structurally equivalent apart from relocated RAM/ROM addresses and helper targets.

### Calling shape

Direct stack-access analysis establishes two ABI argument slots:

```text
argument 0: only the low 16 bits are consumed by the resolver path
argument 1: the low byte is inspected later in the handler
```

The handler returns a value in `D0`; its exact public meaning is still being reconstructed.

Therefore the current working shape is no more specific than:

```c
/* research shape only — NOT public API */
uint32_t SYS_A1C8(uint16_t object_or_file_id, uint8_t mode);
```

The type/name of the return and the semantic name of `mode` are intentionally provisional.

### Resolver and active-object behavior

`A1C8` passes argument 0 through the same internal resolver used by `A1C4`. The resolver returns a descriptor/object pointer. This establishes that the first argument is an identifier/token resolved into a runtime descriptor rather than a raw data pointer.

On the successful path `A1C8`:

1. resolves the first argument to a descriptor;
2. invokes the `A1CC` entry;
3. performs setup on the resolved descriptor;
4. stores the resolved descriptor pointer into a firmware global representing the current/active object;
5. continues initialization and returns a scalar/status value.

The active-object globals differ only by relocation:

```text
AS3000: 0x00003442
NEO:    0x0000349A
```

This behavior materially strengthens the old BetaWise interpretation that `A1C8` enters/opens/selects a current file-like context. It is not consistent with treating the trap as a passive metric query alone.

### Second argument

The low byte of argument 1 is explicitly compared with `1`. In that case a 32-bit descriptor field at offset `+0x14` is cleared. The meaning of that field is not yet established, so the second parameter must remain an unnamed mode/flag rather than being guessed as read/write, create, truncate, etc.

## A1CC direct firmware evidence

### Handler locations

```text
AS3000: A1CC -> 0x4E1C48  (file offset 0x21C48)
NEO:    A1CC -> 0x5E45A4  (file offset 0x245A4)
```

Both handlers reduce to the same operation:

```text
clear current/active descriptor global
return
```

Conceptually:

```c
current_active_descriptor = NULL;
return;
```

No stack argument is consumed. This strongly supports a close/release-current-context role, although the exact public name should remain provisional until surrounding callers and ownership semantics are reconstructed.

## Immediate conclusions

Confidence at this stage:

- `A1C8` has **two arguments** and resolves a 16-bit identifier into a descriptor: **A**.
- `A1C8` installs that descriptor as the current/active object after clearing the previous one through `A1CC`: **A**.
- `A1C8` second argument is a byte-sized mode/flag; `mode == 1` clears descriptor field `+0x14`: **A for mechanics, D for semantic name**.
- `A1CC` consumes **zero arguments** and clears the current/active descriptor global: **A**.
- The names `FileOpen` / `FileClose`: **B working names**, strengthened by handler behavior but not yet promoted as final SDK contract.
- The newer `query_object_metric` interpretation for `A1C8` is insufficient to describe the full handler because direct firmware shows descriptor resolution, current-context replacement, and substantial setup work.

## Next reconstruction steps

Before exposing prototypes in `os3k.h`:

1. identify the descriptor fields touched by `A1C8`, especially offset `+0x14`;
2. reconstruct the common identifier resolver shared by `A1C4`/`A1C8`;
3. inspect official callers to determine what values are supplied as argument 0 and argument 1;
4. determine the exact `D0` return contract and error values;
5. correlate the surrounding `A1C0`, `A1C4`, `A1D0` calls to establish whether the identifier is a file number, file token, applet-owned file identifier, or a more general object ID;
6. only then decide whether the stable public names should remain `FileOpen` / `FileClose`.

## SDK rule

No public prototype is added for `A1C8` or `A1CC` yet. The existing names in `syscall.c` remain untouched while reconstruction is in progress so that historical provenance is preserved and a speculative rename does not create a second false certainty.
