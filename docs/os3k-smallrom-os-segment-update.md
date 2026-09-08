# OS3K Small ROM OS-segment update handshake

This note documents three Manager/update-protocol selectors that are implemented by both the historical AlphaSmart 3000 Small ROM and the later NEO Small ROM. They are protocol handlers, **not A-line syscalls**.

## Source-first host sequence

Historical AlphaSync identifies selector `0x18` with the Small-ROM updater flow, while leaving `0x16` and `0x17` without vendor names. A later independent host implementation in `ioma8/neo-re` makes the update sequence explicit:

1. send `0x18` and require status `0x56`;
2. send `0x16` and require status `0x54`;
3. for each parsed OS segment, send `0x17(address, erase_kb)` and require status `0x55`;
4. after the segment erase/setup pass, stream the OS image through the common `0x02` / raw / `0x0B` block engine.

The firmware analysis below verifies what those three selectors actually do inside the Small ROMs.

## Selector `0x18`: Small-ROM presence handshake

In both Small-ROM generations the complete handler is only `0x0c` bytes. It prepares zero-valued payload fields, selects response `0x56`, and branches to the common response builder.

There is no restart, stack reload, mode switch, hardware reconfiguration, callback change, or other transition in the Small-ROM implementation of this selector.

This is an important scope distinction. At the host-protocol level `0x18` participates in “enter Small ROM”, but **once Small ROM is already executing, its `0x18` handler is a presence/acknowledgement handshake**. The transition into Small ROM must occur outside this handler (for example in the standard-OS implementation or surrounding updater flow).

## Selector `0x16`: logical segment-map reset

The `0x16` handler is `0x14` bytes in both generations. It:

- clears the current segment-count byte;
- clears one adjacent state byte;
- prepares response `0x54` with zero payload fields;
- branches to the same common response builder used by `0x18`.

It does **not** bulk-zero the address and length arrays. Thus the host-facing “clear OS segment map” operation is implemented by invalidating the old entries through a count/state reset rather than by erasing every stored table word.

## Selector `0x17`: register and erase one OS segment

The `0x17` handler is `0xf6` bytes in both generations and preserves the same control-flow geometry.

Its caller-visible mechanics are:

- display the system-update progress message;
- reject a fifth segment record: the current count is compared against four before a new entry is accepted;
- store the 32-bit `address` argument in an address table indexed with a four-byte stride;
- read the trailing 16-bit `erase_kb` argument, shift it left by ten (`erase_kb * 1024`), and store the resulting byte length in a parallel table;
- reload the just-recorded base and length and compute the range end as `base + length`;
- execute the flash write/erase helper loop over that range;
- take a dedicated error path associated with the firmware message “Error: Unable to write/erase flash.” if the low-level operation fails;
- on success, prepare response `0x55` and increment the segment count.

The progress message associated with the handler is “New AlphaSmart System is being loaded...”. Together with the host-side `(address, erase_kb)` contract, the parallel base/length tables, and the explicit flash-error path, this establishes `0x17` as a segment registration **and erase** operation rather than a descriptor-only append.

## Cross-generation result

The AlphaSmart 3000 and NEO `0x17` handlers have identical `0xf6`-byte geometry. Only 28 byte positions differ. Those changes are dominated by relocated strings, globals and helper targets plus one generation-specific bookkeeping immediate; no block is inserted or removed and the opcode/control-flow skeleton is preserved.

The important invariant is therefore the update protocol state machine, not the generation-specific absolute addresses.

## Verification boundary

A private two-generation regression over canonical Small-ROM images passes **52/52 assertions**. It verifies image identities, exact handler boundaries, the `0x18` response-only behavior, `0x16` count/state reset without table bulk-clear, the four-entry `0x17` cap, address-table stride, `erase_kb << 10`, range-end construction, flash-helper/error envelope, success response/count increment, and exact two-generation handler geometry.

Status:

- `0x18` Small-ROM presence/acknowledgement mechanics: **CONFIRMED**;
- claim that the Small-ROM `0x18` handler itself performs the transition into updater ROM: **REFUTED**;
- `0x16` logical segment-map reset mechanics: **CONFIRMED**;
- claim that `0x16` zeroes the full segment arrays: **REFUTED**;
- `0x17(address, erase_kb)` segment registration + erase mechanics: **CONFIRMED**;
- maximum of four stored segment records: **CONFIRMED**;
- exact historical/vendor names for selectors `0x16` and `0x17`: **UNRESOLVED / INSUFFICIENT EVIDENCE**.

No ROM image or extended disassembly is published here, and no A-line ABI promotion is implied.
