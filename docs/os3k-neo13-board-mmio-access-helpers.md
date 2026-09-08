# NEO 2013 ISP1763 MMIO access helper pair

Status: structural research note. This document intentionally contains no firmware image, large disassembly, or Ghidra-derived binary material.

## Scope and canonical input

This note records the NEO 2013 internal MMIO helper pair used by the late USB host/device-controller subsystem. The canonical System 3 Neo package has SHA-256:

`32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Package coordinates use the established mapping `runtime = 0x00410000 + file_offset` for the main OS segment.

## Hardware identity correction

Earlier work correctly established `0x02000000` as the base of a board/ASIC MMIO window, but left the exact register identities unresolved. Later primary-firmware correlation closes that uncertainty.

The NEO 2013 firmware reads the controller chip-ID register at offset `0x70` and compares the result with the exact value `0x00176320`. That value and the surrounding register layout match the ISP1763. The same layout identifies the offsets used through these helpers:

| Offset | ISP1763 register / role |
| ---: | --- |
| `0x18` | `DC_INTERRUPT` |
| `0x70` | `DC_CHIPID` |
| `0x7C` | `DC_UNLOCKDEV` |
| `0xA0` | `HC_PORTSC1` |
| `0xD4` | `HC_INTERRUPT` |
| `0xD6` | `HC_INTERRUPT_ENABLE` |
| `0xB6` | `HC_HW_MODE_CTRL` |
| `0xB8` | `HC_RESET` |
| `0xBA` | `HC_BUFFER_STATUS` |

The firmware writes `0xAA37` to `DC_UNLOCKDEV`, matching the unlock sequence used by the device-controller path.

This supersedes the earlier provisional description that treated the exact hardware-register identities as unresolved. Historical material associating this area with an ISP1161-class device is retained as historical evidence, but the canonical NEO 2013 firmware's explicit chip-ID test prevails for this image.

## Physical objects

Two adjacent 68k routines form an inverse pair:

| File range | Runtime range | Neutral role |
| --- | --- | --- |
| `[0x400B8,0x400FC)` | `[0x004500B8,0x004500FC)` | ISP1763 MMIO read helper |
| `[0x400FC,0x40150)` | `[0x004500FC,0x00450150)` | ISP1763 MMIO write helper |

Both construct a target address as `0x02000000 + uint16_offset`. Their low-byte mode dispatch is mechanically symmetric:

- mode `1`: one byte;
- mode `2`: one 16-bit word;
- default path: two 16-bit words, with the pair exchanged between the mapped representation and the caller buffer.

For the write helper, the mechanically demonstrated argument order is `(offset_word, source_pointer, mode)` under the compiler's stack calling convention. The routine does not construct an explicit semantic return value; volatile-register contents at return are not promoted to an API contract.

## Direct write callers

Exactly five direct absolute callers were found at file offsets:

`0xFBAA`, `0xFBCC`, `0xFD8A`, `0xFDA2`, `0xFF58`.

Their observed `(offset, mode)` pairs are:

- `0x007C`, mode `2` — `DC_UNLOCKDEV`;
- `0x0018`, mode `4` — `DC_INTERRUPT`;
- `0x007C`, mode `2` — `DC_UNLOCKDEV`;
- `0x00D4`, mode `2` — `HC_INTERRUPT`;
- `0x00D6`, mode `2` — `HC_INTERRUPT_ENABLE`.

The final access starts at `0x020000D6` and writes a 16-bit word, directly demonstrating use through byte address `0x020000D7`.

The smaller `0x02000000..0x02000007` range documented by current `alpha-emu` work should therefore be understood as a modeled/observed subset rather than the hardware window's demonstrated upper bound.

## Classification

- Helper mechanics: **CONFIRMED**.
- ISP1763 identity for this NEO 2013 path: **CONFIRMED** by canonical firmware chip-ID test and register-layout correlation.
- Register identities listed above: **CONFIRMED** at the structural level.
- Unlock value `0xAA37` at `DC_UNLOCKDEV`: **CONFIRMED**.
- Vendor/internal function names for the helper pair: **UNKNOWN**.
- Structural state: **CLOSED for mechanics and hardware-register identity; semantic names of higher-level callers remain partially resolved**.
- ABI status: **internal code, not an A-line syscall promotion**.

## Adversarial checks

1. Treating `+0x07` as the end of the controller window is contradicted by canonical accesses through `+0xD7`.
2. Treating incidental `D0` contents as a defined helper return remains unsupported.
3. Treating the controller as ISP1161 is contradicted for NEO 2013 by the explicit `0x00176320` chip-ID comparison and the matching ISP1763 register map.

The original private MMIO regression remains valid for routine mechanics and direct callers. Later structural regressions independently cover the ISP1763 chip-ID and higher-level initialization/interrupt paths.

The late active region has since been structurally segmented through its final active `RTS`; this file is therefore no longer used as a sequential-frontier checkpoint.
