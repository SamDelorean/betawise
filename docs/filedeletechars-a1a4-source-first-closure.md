# A1A4 / FileDeleteChars — source-first re-audit

Status: **closed A / source-first**.

```c
uint32_t FileDeleteChars(uint32_t delete_count);
```

## Source correlation

The early AS3000 `FileModule.c` contains `FileDeleteChars(UInt32 new_cursor_position, UInt32 delete_count)` and `FileRemoveChar()`. The historical implementation positions the cursor and repeatedly removes characters by shifting the suffix left and reducing the logical file size. This establishes a genealogical anchor, not proof of the modern ABI.

The previously reconstructed System 3 File API independently fixes descriptor fields used here: `+0x04 = current_size`, `+0x14 = cursor`, plus the storage pointer and the live-info synchronization helper. The A204 clipboard cut path is a decisive independent caller: it copies bytes beginning at the active file cursor into the clipboard and then invokes A1A4 with the same count.

The exact modern vendor symbol is not present in recovered modern symbols. `FileDeleteChars` is therefore retained as a **strong genealogical name**, while the ABI and behavior below are firmware-confirmed.

## Canonical ROM revalidation

All three canonical ROM SHA-256 values were rechecked before extracting A1A4:

- AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Handler locations and exact span:

| ROM | Runtime | File offset | Length |
| --- | ---: | ---: | ---: |
| AS3000 2005 | `0x004E1632` | `0x21632` | `0x70` |
| NEO 2005 | `0x005E3F8E` | `0x23F8E` | `0x70` |
| NEO 2013 | `0x0043A390` | `0x2A390` | `0x70` |

Of the 112 handler bytes, 107 are identical across all three ROMs. AS3000↔NEO2005 differs at four bytes, NEO2005↔NEO2013 at one byte, and AS3000↔NEO2013 at five bytes; the differences are call displacement / trace metadata only.

## Firmware-confirmed contract

A1A4 consumes one 32-bit argument, resolves the active file through token `0`, and performs:

```text
available = current_size - cursor
actual = min(delete_count, available)

if actual != 0:
    memmove(storage + cursor,
            storage + cursor + actual,
            available - actual)
    current_size -= actual

sync_live_file_info_mirrors(descriptor)
return actual
```

Consequences confirmed directly from the handler:

- deletion begins at the current cursor;
- cursor itself is unchanged;
- deletion is clamped at EOF;
- return value is the actual number removed;
- zero count is a no-op returning zero;
- trailing storage beyond the new logical EOF is not zero-filled;
- recoverable/high-water size is not decremented by A1A4;
- the handler contains no explicit read-only check;
- if no active descriptor resolves, the path returns zero without mutating file storage.

## Caller/xref check

Each canonical ROM contains exactly two absolute occurrences of the A1A4 handler address: the vector-table entry and one direct `JSR abs.l` caller. The direct caller is structurally equivalent 3/3 and pushes the same `D7` count immediately before A1A4. This is the internal cut/delete path correlated with A204.

No contradiction was found with the previously inspected official applets: A1A4 is primarily exercised internally by System 3 rather than by the five direct SmartApplet callers searched earlier.

## Refutation

The following alternatives are rejected by primary evidence:

- **generic resize** — the handler shifts the suffix from `storage+cursor+actual` to `storage+cursor`;
- **truncate-at-cursor** — the requested count is independently clamped and returned;
- **delete-and-advance** — descriptor `cursor (+0x14)` is read but not rewritten;
- **two-argument historical ABI** — modern A1A4 consumes only one 32-bit syscall argument; cursor positioning has been separated from deletion;
- **secure erase** — bytes beyond the new EOF are not cleared.

## Classification

- **CONFIRMADO:** one-argument ABI; active-descriptor resolution; clamp; suffix shift; current-size decrement; unchanged cursor; live-mirror sync; actual-count return; 3/3 equivalence.
- **INFERENCIA FUERTE:** continuity of the public name `FileDeleteChars` from the early AS3000 source.
- **DESCONOCIDO:** exact modern vendor symbol spelling, if it differed from the historical source.

## Regression

Static regression was **EXECUTED: 94/94 PASS** against the three canonical ROMs. It checks canonical hashes, handler boundaries, instruction/field anchors, clamp/memmove/size-update/sync/return sequence, vector entries, direct caller structure, and cross-ROM byte equivalence.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**: middle deletion, EOF clamp, zero deletion, unchanged cursor, live mirrors, no-active-descriptor behavior, and preservation of trailing/recoverable storage semantics.
