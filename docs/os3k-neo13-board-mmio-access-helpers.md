# NEO 2013 board-MMIO access helper pair

Status: structural research note. This document intentionally contains no firmware image, large disassembly, or Ghidra-derived binary material.

## Scope and canonical input

This note records the first internally closed object pair after the NEO 2013 late-active-region frontier. The canonical input is the System 3 Neo package with SHA-256:

`32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

The same image was independently re-extracted from NEO Manager 3.9.3 (`Data1/os3kneorom.os3kos`), size `0x60800` bytes.

Package coordinates in this note use the already-established mapping `runtime = 0x00410000 + file_offset`.

## Source-first correlation

`ioma8/neo-re` / `alpha-emu` independently documents physical address `0x02000000` as an ASIC/board register window used by the full System firmware. Its currently documented emulator observation covers `0x02000000..0x02000007`.

Direct source searches for runtime addresses `0x004500B8` and `0x004500FC` in `neo-re` are negative. Therefore no vendor function names are assigned to the helpers below.

## Physical objects

Two adjacent 68k routines form an inverse pair:

| File range | Runtime range | Neutral role |
| --- | --- | --- |
| `[0x400B8,0x400FC)` | `[0x004500B8,0x004500FC)` | board-MMIO read helper |
| `[0x400FC,0x40150)` | `[0x004500FC,0x00450150)` | board-MMIO write helper |

The next independent routine begins exactly at file offset `0x40150`.

Both helpers construct a target address as `0x02000000 + uint16_offset`. Their low-byte mode dispatch is mechanically symmetric:

- mode `1`: one byte;
- mode `2`: one 16-bit word;
- default path: two 16-bit words, with the pair exchanged between the mapped representation and the caller buffer.

For the write helper, the mechanically demonstrated argument order is `(offset_word, source_pointer, mode)` under the compiler's stack calling convention. The routine does not construct an explicit semantic return value; volatile-register contents at return must not be promoted to an API contract.

## Direct write callers

Exactly five direct absolute callers were found in the canonical package at file offsets:

`0xFBAA`, `0xFBCC`, `0xFD8A`, `0xFDA2`, `0xFF58`.

Their observed `(offset, mode)` pairs are:

- `0x007C`, mode `2`;
- `0x0018`, mode `4`;
- `0x007C`, mode `2`;
- `0x00D4`, mode `2`;
- `0x00D6`, mode `2`.

The final access starts at `0x020000D6` and writes a 16-bit word, so canonical firmware directly demonstrates board-window use through byte address `0x020000D7`.

This is a useful correction to emulator documentation: the `0x02000000..0x02000007` extent in the current `alpha-emu` memory-map note is an observed subset, not a demonstrated upper bound of the hardware window.

## Classification

- Helper mechanics: **CONFIRMED** by canonical firmware.
- `0x02000000` board/ASIC window class: **CONFIRMED** by independent `neo-re` correlation plus canonical firmware access.
- Exact hardware-register identities at offsets `0x18`, `0x7C`, `0xD4`, and `0xD6`: **UNRESOLVED / INSUFFICIENT EVIDENCE**.
- Vendor names for either helper: **UNKNOWN**.
- Structural state: **PARTIALLY CLOSED**: routine boundaries and mechanical contract are closed; register semantics remain unresolved.
- ABI status: **not an A-line syscall promotion**; these are internal routines.

## Adversarial check

Two tempting interpretations are explicitly rejected:

1. Treating `+0x07` as the end of the ASIC/board window is contradicted by canonical 16-bit access through `+0xD7`.
2. Treating the write helper's incidental `D0` contents as a status return is unsupported because no return value is deliberately constructed.

A private canonical-image regression covering hash identity, routine boundaries, base-plus-offset construction, mode dispatch, swapped-word behavior, all five direct xrefs, and all five caller setup triples is **EXECUTED: 17/17 PASS**.

Next structural frontier: file offset `0x40150` / runtime `0x00450150`.
