# A21C / index135 — source-first audit closure

Status: **mechanical confidence A / source-first revalidated**.

The modern vendor symbol for this trap has not been recovered, so the SDK keeps the neutral name `SYS_A21C`.

```c
uint32_t SYS_A21C(uint8_t read_variant);
```

## Source correlation

Existing BetaWise/System 3 material places A21C immediately after the known clipboard buffer operations A20C `ClipboardSet`, A210 `ClipboardGet`, A214 `ClipboardClear`, and A218 (shared clipboard tag getter). Historical material supplies clipboard genealogy and the filtered/raw representation concept, but no reliable modern public symbol for A21C. Those sources were therefore used only as anchors; the contract below was revalidated against the three canonical firmware images.

## Canonical firmware revalidation

The canonical ROM SHA-256 values were rechecked before analysis:

- AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Handler entries and file offsets:

- AS3000: `0x004E0CF6` / `0x020CF6`
- NEO 2005: `0x005E3652` / `0x023652`
- NEO 2013: `0x00439A54` / `0x029A54`

All three handlers are exactly `0x4A` bytes and have the same control-flow shape. They allocate a four-byte local, resolve the dedicated clipboard descriptor, compare the low byte of the first ABI slot with literal `3`, and return the local as a 32-bit value.

For any selector other than `3`, the local is assigned descriptor `+0x04` (`current_size`) and returned directly.

For selector `3`, the handler initializes the local from `current_size` **before** invoking the same filtered/control-code-aware helper family used by `ClipboardGet`. Conceptually:

```text
length = clipboard.current_size
filter(clipboard.storage, NULL, clipboard.current_size, &length)
return length
```

The helper targets are generation-specific but structurally equivalent:

- AS3000: `0x004D9DC0`
- NEO 2005: `0x005DC4F4`
- NEO 2013: `0x0042E81E`

This directly confirms that A21C is a size query for the selected clipboard representation and does not copy clipboard payload bytes.

## Important correction relative to A210

The 2005 `ClipboardGet(..., 3)` implementations contain the already-documented uninitialized-local quirk in their filtered path. A21C does **not** reproduce that defect: both 2005 A21C handlers explicitly initialize their in/out length local from `clipboard.current_size` before calling the filter helper. NEO 2013 does the same. Therefore A21C is the reliable filtered-length query even on the two 2005 firmwares.

## Caller/xref check

Direct System 3 JSR xrefs to A21C were rescanned in all three ROMs: **18 / 18 / 18**. Two callers per ROM push selector `3` immediately before the call, while fifteen per ROM push selector `1`; one additional caller per ROM prepares the argument through a less immediate stack sequence. This independently demonstrates both filtered and raw call modes in native firmware.

The three handler neighborhoods also preserve the expected A21C -> A220 -> A224 sequence: A220 returns clipboard storage/base pointer and A224 returns descriptor `+0x0C` capacity, reinforcing the interpretation of A21C as the logical-length member of the same descriptor API.

## Side effects and return contract

**CONFIRMED**:

- one byte-sized selector argument is consumed from the first ABI slot;
- `read_variant == 3` selects filtered/control-code-aware length calculation;
- every other value selects raw `current_size`;
- the returned value is a 32-bit count in `D0.L`;
- the handler does not alter clipboard tag, current size, capacity, cursor, or payload;
- no allocation occurs in A21C itself.

**DESCONOCIDO**:

- original modern vendor function name;
- symbolic name of selector value `3`.

## Refutation pass

The firmware rules out the main alternatives:

- not a Boolean/status getter: raw path returns a full descriptor length;
- not a capacity getter: A224 separately returns `+0x0C`;
- not a storage-pointer getter: A220 separately returns `+0x00`;
- not a mutating read: no payload destination is supplied and no clipboard descriptor field is written;
- not identical to A210 filtered behavior in 2005: A21C initializes the length local correctly.

## Regression status

Static source-first regression: **EXECUTED — 36/36 PASS**.

The checks cover canonical hashes, handler boundaries, selector compare, local initialization, filtered-helper target, helper argument shape, 32-bit local return, 18 direct JSR xrefs per ROM, selector-3 and selector-1 native callers, and the A210-vs-A21C initialization difference.

Dynamic emulator/hardware regression remains **SPECIFIED / NOT EXECUTED**. The preferred regression compares `SYS_A21C(3)` against the byte count produced by a sufficiently large filtered `ClipboardGet(..., 3)` separately on each firmware generation, while `SYS_A21C(1)` must equal raw `clipboard.current_size`.
