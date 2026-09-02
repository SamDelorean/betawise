# A0F4 / index 61 — `DialogAddItem` source-first closure

Status: **CONFIRMED** for the raw ABI and core semantics in AS3000 2005, NEO 2005, and NEO 2013.

## Contract

```c
int DialogAddItem(char *text,
                  uint8_t text_len,
                  char marker,
                  int id,
                  Key_e shortcut_key,
                  size_t file_size);
```

The System 3 handler consumes six compiler ABI stack slots. After its 16-byte saved-register frame, the observed operands are:

- `text`: full 32-bit pointer slot;
- `text_len`: low byte of its slot;
- `marker`: low byte of its slot;
- `id`: full 32-bit value;
- `shortcut_key`: low byte of its slot;
- `file_size`: full 32-bit value.

The modern OS3K dialog has capacity for **64 items**. Adding an item when full returns `-1`; successful insertion increments the item count and returns `0`.

## Source-first genealogy

The early dialog API exposed a smaller `DialogMenuAddItem(text, text_len, marker)` operation. That implementation established the insertion-order menu model and the historical role of `marker` as the glyph immediately preceding an item's text.

System 3 extends that model with three per-item metadata fields: `id`, `shortcut_key`, and `file_size`. Existing dialog-family analysis independently correlates the stored `id` with `DialogGetChoiceId` / `DialogGetItemId`, the shortcut byte with `DialogRun`, and `file_size` with dialog rendering.

Unsupported shortcut values are normalized to `KEY_NONE` (`0xff`) before they are stored. Valid shortcut labels are produced through the same key-translation path used by dialog drawing.

`file_size` is presentation metadata, not a request to resize or otherwise mutate a file. Existing dialog rendering uses it to describe item size; `(size_t)-1` is the normal no-size sentinel in callers.

## Three-generation firmware comparison

The canonical A0F4 handlers are all exactly `0x106` bytes long. After masking only fixed relocation operand fields, the complete handler bodies are byte-identical across:

- AlphaSmart 3000 System 3, November 2005;
- NEO System 3, November 2005;
- NEO System 3, July 2013.

Only 30 raw bytes differ across the three images, and every difference lies inside those relocation operands. In particular, the ABI slot accesses, capacity constant (`64`), shortcut normalization (`0xff`), item-count update, and return behavior are invariant.

This closes an earlier open question in the dialog-family notes: the 2013 NEO implementation does **not** introduce a different A0F4 capacity or core algorithm.

## Refuted alternatives

- The early 25-item historical limit does **not** carry into this System 3 syscall; all three canonical handlers test against 64.
- NEO 2013 does not use a distinct A0F4 ABI or item-capacity rule.
- `file_size` is not evidence of a file-storage mutation operation; it belongs to the dialog item's presentation metadata path.

Any later-OS3K-specific meaning of unusual `marker` byte values beyond the confirmed historical prefix-glyph role remains **UNKNOWN** and is not assigned a vendor name or enum here.

## Callers / xrefs

Direct absolute calls to the handler are present in each canonical ROM: 5 in AS3000 2005, 5 in NEO 2005, and 6 in NEO 2013. Independent System code also exercises A0F4 as part of the coherent dialog lifecycle (`DialogInit` → add items / exit keys → draw / run → get choice).

Official SmartApplet caller analysis is consistent with the six-slot prototype above.

## Regression

A private static/contract regression was **EXECUTED: 47/47 PASS**. It verifies canonical hashes, handler boundaries, ABI slot widths/positions, the 64-item limit, `-1`/`0` return paths, `KEY_NONE` normalization, item-count increment, and complete three-generation structural equivalence after a fixed relocation mask.

Dynamic emulator/hardware regression remains a separate execution-level test; it is not being represented as executed here.

## Provenance / privacy

This document contains a functional contract and independently written analysis only. Canonical firmware images, complete handler bytes, and extended disassembly remain in the project's private evidence corpus.