# AlphaSmart NEO SD / removable-media hardware reverse engineering

Status: **hardware hypothesis under investigation**.  
Scope: AlphaSmart NEO / NEO 2 hardware and OS3K firmware.  

This note records the current evidence for reconstructing the unpopulated card-socket footprint observed on NEO logic boards and testing whether it is related to the SD/MMC interface used in the AlphaSmart Dana family.

The goal is deliberately narrower than claiming that the NEO already has a complete SD implementation. The present objective is to identify the electrical interface first, then correlate it with firmware and finally compare it with Dana hardware/software.

## Working hypothesis

A NEO logic-board footprint appears mechanically compatible with a memory-card socket. Because AlphaSmart Dana products use SD/MMC storage, a plausible hypothesis is that the NEO PCB retained part of a related hardware design but left the socket and/or supporting components unpopulated.

This is **not yet confirmed**. The footprint must be traced electrically.

## Processor evidence

NEO uses the Motorola/Freescale **MC68VZ328 DragonBall VZ** family. The processor contains two SPI units. SPI unit 1 is routed through Port J:

| Function | MC68VZ328 pin function |
| --- | --- |
| MOSI | PJ0 / MOSI |
| MISO | PJ1 / MISO |
| clock | PJ2 / SPICLK1 |
| chip select | PJ3 / SS |
| optional data-ready | PK0 / DATA_READY |

The relevant Port J registers are:

| Register | Address |
| --- | --- |
| PJDIR | `0xFFFFF438` |
| PJDATA | `0xFFFFF439` |
| PJPUEN | `0xFFFFF43A` |
| PJSEL | `0xFFFFF43B` |

SPI unit 1 uses:

| Register | Address |
| --- | --- |
| SPIRXD | `0xFFFFF700` |
| SPITXD | `0xFFFFF702` |
| SPICONT1 | `0xFFFFF704` |
| SPIINTCS | `0xFFFFF706` |

## Firmware result: SPI1 is real hardware in NEO 2013

Reverse engineering of the canonical NEO 2013 OS3K image confirms an active firmware path that directly accesses the MC68VZ328 SPI1 register block.

A private byte-transfer helper associated with the NEO-2013-only A3B4/A3B8 extension:

- waits on SPI interrupt/status state;
- writes a transmit byte through `SPITXD`;
- programs `SPICONT1`;
- performs the exchange;
- reads the received byte from `SPIRXD`.

This moves SPI1 from a datasheet-based possibility to a **confirmed actively used NEO 2013 peripheral**.

The observed SPI control word is `0x6707`. Decoding it against the MC68VZ328 register definition gives the following working interpretation:

- master mode;
- SPI enabled;
- exchange initiated;
- 8-bit transfer;
- SS active low;
- clock idle low / phase 0 (SPI mode 0);
- DATA_READY handshake not used by this transfer;
- clock divider consistent with SYSCLK/32.

Therefore the most important board traces to test first are **PJ0, PJ1, PJ2 and PJ3**.

## Auxiliary GPIO observed in the same hardware path

The same NEO 2013 transaction family also accesses GPIO state outside Port J:

- **PF2** is sampled through `PFDATA` (`0xFFFFF429`);
- **PB6** is sampled through `PBDATA` (`0xFFFFF409`);
- **PB7** is actively forced low while the other Port B bits are preserved.

Their exact meanings are still unknown. They must not yet be labeled card-detect, write-protect, power-enable, reset, or similar names.

For board tracing, however, they form a useful secondary candidate set:

`PJ0, PJ1, PJ2, PJ3, PF2, PB6, PB7`

## SD-in-SPI-mode correspondence

If the unpopulated footprint is an SD socket operated in SPI mode, the expected core mapping would be approximately:

| SD signal in SPI mode | Candidate NEO signal |
| --- | --- |
| CMD / DI | PJ0 / MOSI |
| DAT0 / DO | PJ1 / MISO |
| CLK | PJ2 / SPICLK1 |
| DAT3 / CS | PJ3 / SS |

This correspondence is electrically attractive, but it is still a **hypothesis until PCB continuity is demonstrated**.

Unused SD-native data pins, power, ground, mechanical card-detect and write-protect contacts must also be identified from the board.

## Important negative result

The currently reconstructed A3B4 transaction must **not** be renamed an SD driver.

Its higher-level framing includes private protocol behavior (including an `0xA5` marker) that does not immediately resemble the ordinary low-level SD SPI command/response sequence. SPI1 may therefore be servicing another peripheral, or the observed path may represent only one user of a shared SPI bus.

Current classification:

- **CONFIRMED:** MC68VZ328 SPI1 exists and is actively used by NEO 2013 firmware.
- **CONFIRMED:** NEO 2013 performs synchronous 8-bit master SPI transfers through the SPI1 register block.
- **CONFIRMED:** the related hardware path also uses PF2, PB6 and PB7.
- **STRONG CANDIDATE:** PJ0-PJ3 are the first signals to trace from the suspected card footprint.
- **UNRESOLVED:** whether that footprint is electrically connected to SPI1.
- **UNRESOLVED:** whether the footprint is actually SD/MMC.
- **UNRESOLVED:** whether the A3B4/A3B8 transaction family is related to removable media.

## Generational warning

The callable A3B4/A3B8 hardware-backed extension is present in the canonical NEO 2013 firmware but is not represented by valid callable handlers in the canonical AlphaSmart 3000 2005 and NEO 2005 images examined by this project.

This prevents us from assuming that the discovered SPI transaction describes every NEO board revision. Hardware revision, NEO vs NEO 2, and firmware generation must remain separate axes of evidence.

## PCB verification plan

The next physical investigation should proceed from the suspected socket footprint rather than from guessed signal names:

1. photograph both PCB sides at sufficient macro resolution;
2. number every footprint contact and mechanical contact;
3. identify ground and supply contacts first;
4. trace candidate CLK/CMD/DAT0/DAT3 contacts;
5. test continuity toward MC68VZ328 PJ0-PJ3 or intermediate components;
6. trace any remaining contacts toward PF2, PB6, PB7 or another IC;
7. record resistors, pull-ups, series resistors, capacitors and unpopulated support footprints;
8. distinguish direct CPU SPI wiring from a possible external SD/MMC controller.

A useful evidence table will be maintained in the form:

`socket pad -> PCB net -> component/pad -> MC68VZ328 pin or external controller -> firmware register -> confidence`

## Firmware follow-up

The highest-priority firmware search is now the initialization of Port J, especially writes affecting:

`PJSEL = 0xFFFFF43B`

Finding the code that selects PJ0-PJ3 for their dedicated SPI1 functions should reveal when the bus is enabled and may expose the surrounding peripheral initialization sequence.

Additional searches should include:

- SPI1 initialization outside the known A3B4/A3B8 path;
- all readers/writers of PF2, PB6 and PB7;
- 512-byte block-transfer patterns;
- SD/MMC command-like constants and response state machines;
- removable-media, volume or filesystem abstractions;
- board-revision-dependent initialization.

## Dana comparison stage

Once the NEO electrical nets are known, the AlphaSmart Dana becomes a reference implementation rather than a source of assumptions.

The intended comparison is:

1. identify Dana SD/MMC socket wiring and any external controller;
2. identify the corresponding Dana low-level driver/register access;
3. compare Dana and NEO signal topology;
4. extract only the hardware-facing behavior needed to understand the interface;
5. implement a clean OS3K-side driver if the NEO hardware supports the interface.

Palm OS / Dana filesystem layers are not assumed to be directly reusable on OS3K. The useful target is the hardware abstraction and low-level transaction behavior.

## Publication boundary

This repository documents independently reconstructed behavior and register-level observations. Proprietary ROM images and bulk firmware disassembly are intentionally not distributed here.

The purpose of this document is to make the investigation reproducible from publicly documented MC68VZ328 hardware behavior, independently obtained board observations, and project-derived functional conclusions.
