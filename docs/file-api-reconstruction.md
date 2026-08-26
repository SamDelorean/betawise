# System 3 / OS3K File API reconstruction

This document tracks reverse-engineered behavior of the System 3 file ABI used by AlphaSmart 3000 and NEO/NEO2. It records only behavior supported by firmware analysis and official SmartApplet callers; provisional names remain explicitly marked.

## Evidence status

Confidence **A** means the behavior is established directly by firmware analysis or equivalent primary evidence. Historical BetaWise names are retained only when they match the observed behavior.

## Descriptor fields identified so far

| Offset | Current interpretation | Confidence |
| --- | --- | --- |
| `+0x00` | storage/base pointer | A for mechanical use |
| `+0x04` | current file size | A |
| `+0x08` | previous/recoverable size (high-water mark) | A |
| `+0x0C` | maximum size/capacity | A |
| `+0x14` | current cursor/file position | A |
| `+0x44` | canonical 16-bit descriptor/file token | A |

The historical AS3000 `FileModule.c` independently describes the same conceptual model: maximum size, current size, previous size, cursor position and backing storage.

## A1C8 — provisional `FileOpen`

Current research prototype:

```c
uint16_t SYS_A1C8(uint16_t file_id, uint8_t reset_position);
```

This prototype is **not yet promoted into `os3k.h`**.

Observed behavior on AS3000 System 3 (Nov 2005), NEO System 3 (Nov 2005), and NEO System 3 (Jul 2013):

1. Resolve the 16-bit token to a runtime descriptor.
2. Clear the previously active descriptor through A1CC.
3. Install the resolved descriptor as the active file/context.
4. Perform descriptor preparation.
5. If the second argument is exactly `1`, set `descriptor+0x14` (current position) to zero.
6. Return the zero-extended 16-bit value from `descriptor+0x44`.
7. Resolution failure returns `0`.

The second argument is only compared against `1`; no other use of it was found in the handler. Official callers examined so far use only `0` and `1`.

### Official callers examined

Twenty-five real A1C8 callers were traced in official 2012 SmartApplets:

- AlphaWord Plus 3.4: 4
- Calculator 3.0: 1
- ControlPanel 1.0: 1
- KeyWords 3.6: 9
- AlphaQuiz 1.0: 10

Observed second-argument distribution: **17 calls with `1`, 8 calls with `0`, no other values**.

The 2005 Calculator and ControlPanel binaries also contain the exact call pattern:

```c
A1C8(0x00CB, 1);
```

## Reserved token `0x00CB`

`0x00CB` is a reserved token for the **clipboard descriptor**.

The A1C8/A1C4 resolver compares the requested token explicitly against `0x00CB` and, on match, takes a dedicated path to a global descriptor instead of performing the ordinary lookup. The already-identified `ClipboardSet`, `ClipboardGet`, and `ClipboardClear` syscalls operate on this same descriptor, closing the identification.

## Token `0`

Token `0` resolves to the **currently active descriptor**. This explains why several neighboring file syscalls can operate without an explicit file ID: they consume the context selected by A1C8.

## A1CC — provisional `FileClose`

A1CC consumes no arguments and clears the global active-descriptor pointer:

```c
active_descriptor = NULL;
```

The historical BetaWise name `FileClose` is therefore strongly supported, but public SDK promotion is being kept together with A1C8 until the neighboring API family is closed.

## A1A0 — inherited comment is wrong

Upstream BetaWise currently comments A1A0 as:

```c
SYS_A1A0 // get current file number?
```

Firmware analysis contradicts that interpretation. A1A0 operates on the active descriptor, clears backing storage and resets multiple descriptor fields. It is much closer to a destructive reset/smash operation than a getter. The historical AS3000 source contains a `FileSmashFile()` concept with matching semantics, but the final public OS3K name remains provisional pending full caller correlation.

## A1B4 — size query family

A1B4 resolves ordinary file IDs and returns `descriptor+0x04`, the current file size. It also recognizes special selectors in the `0xFC`–`0xFF` range. Selector `0xFC` is already tied to `descriptor+0x08` of the active file; the remaining selectors aggregate descriptor-space metrics and are still being characterized before naming.

## A1B8 — active-file resize/state operation

For ordinary non-negative requested sizes, A1B8 operates on the active descriptor and follows this core behavior:

```text
new_size = min(requested_size, max_size)
current_size = new_size
if cursor > new_size:
    cursor = new_size
if new_size > recoverable_size:
    recoverable_size = new_size
```

This provides the mechanical mapping:

- `descriptor+0x04` = current size
- `descriptor+0x08` = previous/recoverable extent / high-water mark
- `descriptor+0x0C` = maximum capacity
- `descriptor+0x14` = current position

A1B8 also recognizes negative command values (`-2`, `-3`, `-5`, `-6`). Their exact semantics remain under active analysis and should not yet be exposed as SDK constants.

## Current naming decision

The observed A1C8/A1CC behavior strongly supports the inherited `FileOpen` / `FileClose` names and conflicts with treating A1C8 as a simple scalar-property query. However, the public headers will remain unchanged until A1B4, A1B8, A1C0 and A1C4 are sufficiently characterized to publish a coherent File API rather than isolated provisional functions.

## Next work

1. Resolve A1B4 selectors `0xFC`–`0xFF` from firmware and official callers.
2. Resolve A1B8 negative commands `-2`, `-3`, `-5`, `-6`.
3. Characterize A1C0 and A1C4 and correlate their descriptor fields.
4. Revisit A1A0 against historical `FileSmashFile()` and official callers.
5. Promote stable names/prototypes into `os3k.h` only after the family is internally consistent.
