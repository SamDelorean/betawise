# A038 / index 14 — selection refresh / redraw primitive

Status: **mechanical confidence A**, with one intentionally unresolved ABI detail documented below.

## Neutral identity

The modern symbol remains `SYS_A038`.

The historical AS3000 source contains the direct functional ancestor:

```c
LCDRefreshSelection(selection_state, start, end, cursor, anchor)
```

The System 3 firmware strongly preserves that algorithmic lineage, but no modern exported vendor name has been recovered.

## Confirmed System 3 inputs

The three canonical handlers consume four semantic inputs from the caller stack:

1. a selection-state value, consumed by its low byte;
2. a pointer to a two-byte start point `(row, col)`;
3. a pointer to a two-byte end point `(row, col)`;
4. a pointer to a two-byte cursor point `(row, col)`.

The historical function has a fifth `anchor` pointer. The System 3 handlers never read the corresponding fifth stack slot. In the historical source the anchor and generic selection-block glyph constants are identical, so the compiler can optimize all anchor-dependent distinctions away. This makes five-argument continuity plausible, but **does not prove that the modern ABI still requires that fifth caller argument**.

For that reason this audit intentionally does **not** publish a five-argument C prototype or wrapper.

## Confirmed behavior

The operation redraws a rectangular/linear text-selection span in row-major screen-coordinate order from `start` through `end`.

- The cursor is moved/hidden at the start and again when wrapping to a new row.
- Columns wrap after physical column 40.
- In normal state (`1`), backing-store characters are restored; an unwritten `0xFF` entry becomes ASCII space.
- At completion, the OS cursor row/column state is restored from the supplied cursor point.
- No portable return-value contract has been established.

This is not a generic memory `move block` primitive.

## Generation differences

### AS3000 System 3 (2005)

Uses the fixed 40-column selection backing store. Non-normal selection rendering uses special display character codes and distinguishes the current cursor location.

### NEO System 3 (2005)

Preserves the AS3000 control structure while using the NEO selection backing store with a 132-byte row stride.

### NEO System 3.15 (2013)

Still restores characters from the 132-byte-stride backing store and preserves the same start/end/cursor traversal contract. Normal state renders through the ordinary character path; non-normal state delegates selected rendering to a separate internal helper. This is a real implementation evolution, not an ABI reason to reinterpret A038 as a memory mover.

## Historical correlation

The historical `LCDRefreshSelection` routine independently matches all of the following System 3 mechanics:

- `selection_state` controls normal versus selected rendering;
- traversal begins at `start` and terminates at `end`;
- normal rendering reads the selection backing store and maps `0xFF` to space;
- row wrapping moves the hidden cursor;
- final cursor coordinates are restored from `cursor`.

The historical fifth `anchor` input remains **INFERENCIA FUERTE / PROVISIONAL for modern ABI arity**, not CONFIRMADO.

## Callers / negative evidence

Structural target resolution was performed rather than searching raw `A038` words.

AlphaWord Plus 2005 and AlphaWord Plus NEO 2012 both contain the A038 import veneer but have zero executable xrefs to it.

The search was then expanded to fifteen additional official SmartApplet binaries across AS3000 and NEO families, including KeyWords, Thesaurus, SpellCheck, AcceleratedReader, Control Panel, and wireless variants. Every tested applet contains the A038 veneer; every one produced zero executable xrefs to that veneer.

Therefore no official caller in the tested corpus can resolve the modern four-versus-five argument question. This is recorded as a demonstrated negative result rather than silently converting the historical signature into a modern contract.

## Validation

Private static regression: **EJECUTADA, 95/95 PASS**.

It verifies canonical ROM identity, handler boundaries/hashes, reproducible 68000 decoding, the four consumed System 3 inputs, selection traversal, backing-store/sentinel behavior, generation-specific rendering paths, historical-source correlation, and the expanded official-caller negative search.

A dynamic hardware/emulator probe is additional validation and is **not claimed as executed**.

Firmware bytes, extensive disassembly, proprietary binaries, and private workpapers remain outside the public repository.
