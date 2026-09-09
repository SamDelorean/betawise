# OS3K Small ROM entry 0x00400420: selector 0x18 response transports

## Status

`PARCIAL_CERRADO / NO_RESUELTO` for transport-mode semantics. This is a physical Small ROM entry/protocol object, **not** an A-line syscall.

## Source-first context

The NEO updater host flow enters Small ROM with Manager selector `0x18` and expects status `0x56`. Independent canonical-firmware work shows that NEO13 Manager selector `0x18` calls the physical Small ROM entry at `0x00400420`; that entry masks interrupts and transfers to a worker that discriminates modes `3`, `4`, and `5`. Each recognized mode builds an 8-byte response with status `0x56` before using a mode-specific delivery path.

The names below are descriptive analyst labels only. No AlphaSmart vendor symbols are asserted.

## Mode 5: UART response path

Mode 5 is now semantically identified at the hardware-transport level.

Canonical NEO13 Small ROM copies exactly eight bytes from the constructed response buffer to absolute-short address `0xF907`, one byte per iteration.

The Motorola/NXP MC68EZ328 reference manual places the 16-bit UART transmitter register `UTX` at `0xFFFFF906`. Its low byte, containing `TX DATA`, is therefore `0xFFFFF907`. Because 68k absolute-short addressing sign-extends `0xF907` to the peripheral address range, the Small ROM writes target the UART TX data byte rather than ordinary RAM.

Therefore:

- mode 5 response destination = DragonBall EZ UART transmitter data register;
- transfer length = exactly 8 bytes;
- payload = the Small ROM response packet whose status byte is `0x56`;
- classification: **CONFIRMADO** for direct UART transmission of the response;
- the alternative hypothesis that `0xF907` is merely a RAM buffer is refuted.

## Modes 3 and 4

Their mechanical paths are established but their transport identities are not yet promoted:

- mode 3 passes the built 8-byte response buffer to helper `0x00404966` with an explicit length of 8;
- mode 4 builds to a fixed destination at `0x00000C1E` and then invokes helpers around `0x00402CE8` and `0x0040272A`;
- both ultimately participate in the same recognized-mode response flow.

Exact assignments such as USB, PS/2, another serial path, or an internal staging channel remain `NO_RESUELTO / EVIDENCIA_INSUFICIENTE` until independently correlated with historical source/hardware evidence.

## ABI boundary

Neither Manager selector `0x18` nor physical address `0x00400420` is promoted to the OS3K A-line ABI. This analysis concerns an internal Manager/Small-ROM protocol and physical firmware object.

## Reproducibility

Private canonical-firmware regression: `verify_neo13_smallrom_entry18_modes_response56_2026-09-08`, 31/31 checks passed. Firmware/disassembly artifacts remain private by project policy.

Hardware correlation source: Motorola/NXP `MC68EZ328 User's Manual`, UART transmitter register description (`UTX` at `0xFFFFF906`, low byte = TX DATA).
