# OS3K Small ROM entry 0x00400420: selector 0x18 response transports

## Status

`PARCIAL_CERRADO / NO_RESUELTO` for the complete transport set. This is a physical Small ROM entry/protocol object, **not** an A-line syscall.

The three recognized response modes no longer have the same evidence level:

- mode 3: ISP1763 Device Controller data-flow transport — **CONFIRMADO** at subsystem/register level; transmit/control role **INFERENCIA FUERTE**;
- mode 4: three-wire GPIO transport — **CONFIRMADO** mechanically; external endpoint/protocol remains `NO_RESUELTO / EVIDENCIA_INSUFICIENTE`;
- mode 5: direct DragonBall UART response transport — **CONFIRMADO**.

## Source-first context

The NEO updater host flow enters Small ROM with Manager selector `0x18` and expects status `0x56`. Independent canonical-firmware work shows that NEO13 Manager selector `0x18` calls the physical Small ROM entry at `0x00400420`; that entry masks interrupts and transfers to a worker that discriminates modes `3`, `4`, and `5`. Each recognized mode builds an 8-byte response with status `0x56` before using a mode-specific delivery path.

The names below are descriptive analyst labels only. No AlphaSmart vendor symbols are asserted.

## Mode 3: ISP1763 Device Controller path

Mode 3 passes the built 8-byte response buffer to its transport worker with an explicit length of 8.

Cross-image correlation closes the hardware family. The Small ROM mode-3 worker is a relocation-adjusted homologue of the NEO13 main-OS endpoint-transfer worker: 415 of 430 bytes are identical, and the remaining 15 bytes are confined to five three-byte relocation fields. Its local read/write helpers are likewise byte-identical, modulo placement, to the main-OS MMIO helper pair.

The corresponding main-OS worker accesses a coherent Device Controller register set: DMA endpoint selection, endpoint index, buffer/data port, and endpoint control. Independent ISP1763 register definitions map those same offsets to the ISP1763 Device Controller. The surrounding NEO13 controller family also contains a consistent ISP1763 host/device register map and validates the controller chip ID against `0x00176320`.

Therefore:

- mode 3 transport family = ISP1763 Device Controller data flow — **CONFIRMADO**;
- mode 3 uses the same endpoint-transfer mechanism as the main-OS homologous worker — **CONFIRMADO** mechanically;
- the narrower analyst description “TX/control” remains **INFERENCIA FUERTE** until the upper caller contract or an original vendor symbol is recovered;
- the earlier generic `board-MMIO` label is superseded.

This conclusion does not assign an AlphaSmart vendor function name and does not promote the worker to the A-line ABI.

## Mode 4: three-wire GPIO path

Mode 4 is mechanically distinct from mode 3. It uses two DragonBall GPIO inputs and one GPIO output: PF7 and PM1 are configured as inputs without pull-ups, while PE0 is configured as an output. The worker waits on the two input handshakes and serializes each source byte through PE0, testing bit 7 and repeating for eight bit cycles.

Therefore the three-wire bit-bang transport itself is **CONFIRMADO**. The external device and upper protocol remain `NO_RESUELTO / EVIDENCIA_INSUFICIENTE`. Correlation with the NEO 2 RF subsystem is an **INFERENCIA FUERTE**, not a confirmed electrical net mapping.

Do not confuse this Small-ROM transport selector value `4` with internal MMIO accessor mode numbers used elsewhere in the firmware; they are unrelated numbering domains.

## Mode 5: UART response path

Mode 5 is semantically identified at the hardware-transport level.

Canonical NEO13 Small ROM copies exactly eight bytes from the constructed response buffer to absolute-short address `0xF907`, one byte per iteration.

The Motorola/NXP MC68EZ328 reference manual places the 16-bit UART transmitter register `UTX` at `0xFFFFF906`. Its low byte, containing `TX DATA`, is therefore `0xFFFFF907`. Because 68k absolute-short addressing sign-extends `0xF907` to the peripheral address range, the Small ROM writes target the UART TX data byte rather than ordinary RAM.

Therefore:

- mode 5 response destination = DragonBall EZ UART transmitter data register;
- transfer length = exactly 8 bytes;
- payload = the Small ROM response packet whose status byte is `0x56`;
- classification: **CONFIRMADO** for direct UART transmission of the response;
- the alternative hypothesis that `0xF907` is merely a RAM buffer is refuted.

## ABI boundary

Neither Manager selector `0x18` nor physical address `0x00400420` is promoted to the OS3K A-line ABI. This analysis concerns an internal Manager/Small-ROM protocol and physical firmware objects.

## Reproducibility

Private canonical-firmware regressions retained by project policy:

- Small-ROM entry18 response-mode regression: 31/31 checks passed;
- mode-3 transport homology regression: 18/18 checks passed;
- mode-4 GPIO protocol regression: 23/23 checks passed.

No binary firmware, large disassembly, or Ghidra artifact is published here.

Independent hardware/register correlations:

- Motorola/NXP MC68EZ328 User's Manual for UART and DragonBall GPIO registers;
- Linux `drivers/usb/isp1760/isp1760-regs.h` for the ISP1763 host/device register map;
- Linux ISP176x UDC initialization code, which accepts `0x00176320` as a valid controller chip ID.
