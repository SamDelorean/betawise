# AlphaSmart NEO SD / removable-media hardware reverse engineering

Status: **SD footprint confirmed; electrical implementation still under reconstruction**.  
Scope: AlphaSmart NEO / NEO 2 hardware and OS3K firmware.

This note records the current evidence for reconstructing the unpopulated SD/MMC circuit found on NEO logic boards. The objective is to distinguish confirmed board facts from firmware inference, reconstruct the missing support components, and eventually verify the interface using a reversible OS3K diagnostic application.

## Current conclusions

The NEO board contains a nine-contact footprint whose silkscreen numbering, in the photographed orientation, is:

`8 7 6 5 4 3 2 1 9`

This matches the reversed physical contact order of a standard SD card interface. The working classification is therefore:

- **CONFIRMED:** the unpopulated connector footprint is intended for SD/MMC-class card hardware.
- **UNRESOLVED:** the complete electrical route from the socket to the processor or intermediate logic.
- **UNRESOLVED:** which support components must be populated.
- **UNRESOLVED:** whether production OS3K contains dormant SD support.

The standard SD contact functions are:

| Pin | Function |
| --- | --- |
| 1 | DAT3 / CS in SPI mode |
| 2 | CMD / DI in SPI mode |
| 3 | VSS1 |
| 4 | VDD |
| 5 | CLK |
| 6 | VSS2 |
| 7 | DAT0 / DO in SPI mode |
| 8 | DAT1 |
| 9 | DAT2 |

In the photographed NEO orientation the visible sequence is therefore:

`DAT1, DAT0, VSS2, CLK, VDD, VSS1, CMD, DAT3, DAT2`.

## Processor

The NEO hardware examined by this project uses the **MC68VZ328 DragonBall VZ**, not the later MC68SZ328 Super VZ. This distinction matters because the MC68VZ328 does not provide the integrated MMC/SD controller present in the Super VZ family.

## SPI1 result: active, but associated with another subsystem

Reverse engineering of the canonical NEO 2013 OS3K image confirms active use of MC68VZ328 SPI1 through:

- PJ0 / MOSI
- PJ1 / MISO
- PJ2 / SPICLK1
- PJ3 / SS

The firmware explicitly configures Port J for its dedicated SPI1 functions and performs synchronous 8-bit master transfers.

Further caller analysis links this transaction family to the NEO 2 wireless subsystem. The working classification is therefore:

- **CONFIRMED:** SPI1 is actively used by NEO 2013.
- **STRONG INFERENCE:** the currently reconstructed A3B4/A3B8 SPI1 family belongs to the NEO 2 wireless path rather than being the SD driver.
- **NOT EXCLUDED:** SPI1 could still have been electrically shareable through external selection, buffering or unpopulated logic.

The absence of an SD-like command sequence in this firmware path must not be treated as evidence that the PCB lacks SD routing.

## PB5 / CSD1 result

The second Group-D chip-select initially appeared interesting because the MC68VZ328 address map permits another external window. Firmware reconstruction, however, shows that PB5/CSD1 remains configured as GPIO and is actively used as one row of the keyboard scan matrix.

Therefore:

- **CONFIRMED:** PB5 is used as a keyboard matrix drive line in the examined firmware.
- **EXCLUDED:** PB5/CSD1 as a direct active SD chip-select in that configuration.
- **NOT EXCLUDED:** an external multiplexer, buffer, ASIC, CPLD or DNP device could still reuse or reroute signals in a different hardware configuration.

## External bus map

Small-ROM initialization maps the principal external chip-select regions as follows:

| Group | Base | Current interpretation |
| --- | ---: | --- |
| A | `0x00400000` | Flash / ROM |
| B | `0x00000000` | RAM |
| C | `0x01000000` | LCD subsystem |
| D | `0x02000000` | USB controller region |

The populated NEO board photographed by this project contains **U9 = ISP1161ABD**, an external USB Host/Device controller. Its physical proximity to the SD footprint does not by itself establish an SD connection; the ISP1161A has no native SD/MMC interface.

## Unpopulated support circuit around the SD socket

The photographed NEO board shows several unpopulated support positions immediately below the SD footprint, including:

- `R51..R55`
- `C60..C63`
- `U11`
- `Q9`

The present interpretation is deliberately provisional.

### R51..R55

Five nearby resistor footprints are an especially good match for the five pull-ups normally required by native SD wiring:

- CMD
- DAT0
- DAT1
- DAT2
- DAT3

This is a **strong topology-based inference**, not yet a measured net assignment.

### U11, C60..C63 and Q9

The arrangement around U11 looks more like a power stage than a digital data buffer.

Visual observations currently support the following working model:

- C60 and C62 appear to sit on opposite supply-side nodes of U11 and may be bulk input/output capacitors;
- C61 and C63 are plausible high-frequency bypass capacitors;
- U11 appears to use a small approximately five-pin package compatible with an LDO or load switch;
- Q9 appears connected toward U11 through R52, suggesting an enable/shutdown or bias-control path.

A plausible topology is:

```text
system rail (possibly 5 V)
        |
       C60
        |
       U11  ---- R52 ---- Q9 ---- control GPIO / logic
        |
       C62
        |
      VDD_SD
        |
     SD pin 4
```

The input voltage is **not yet confirmed**. A 5 V source is plausible for a 3.3 V SD regulator stage but must be measured on the real board before component selection.

## Why missing software does not disprove missing hardware

The board may have been designed for a feature that was not populated in production. In that case all of the following can be true at once:

- the SD socket is absent;
- support passives or an intermediate IC are absent;
- production OS3K never initializes the interface;
- direct CPU-to-socket continuity does not exist because a DNP device interrupts the route.

For that reason every software-based exclusion in this project is scoped narrowly. A pin being used by another subsystem does not eliminate the possibility of external multiplexing, buffering, tri-state sharing, programmable glue or an unpopulated controller.

## Dana comparison

AlphaSmart Dana remains a useful reference because it implements working SD/MMC storage around the same MC68VZ328 processor family. Dana PCB photographs show two populated SD/MMC sockets with intermediate logic below them.

Dana is now treated as a **secondary reference**, not the primary research path. A full reverse engineering of its Palm OS slot driver is deferred unless the NEO board investigation reaches an ambiguity that Dana can resolve efficiently.

## Physical verification plan

The next board investigation should proceed from the SD footprint outward, with the unit unpowered first:

1. confirm VSS pins 3 and 6;
2. trace pin 4 VDD toward C60/C62/U11;
3. verify whether C60 and C62 are respectively U11 input and output capacitors;
4. trace R52 between U11 and Q9;
5. identify the remaining Q9 terminal destinations;
6. map R51..R55 against CMD and DAT0..DAT3;
7. trace CLK separately and identify any series resistor or protection element;
8. follow every signal through vias and DNP footprints rather than assuming direct CPU continuity;
9. photograph and trace the reverse PCB side where necessary.

The canonical evidence record is:

`SD pin -> PCB net -> passive / DNP device -> IC pin -> CPU/GPIO/MMIO -> confidence`

## Preferred active test: OS3K diagnostic application

Before considering a replacement firmware or bare-metal monitor, the preferred active test vehicle is a small **`.os3kapp` diagnostic application** running on top of normal OS3K.

OS3K would be used only for application loading, keyboard input and display output. The diagnostic code would access known MC68VZ328 GPIO/MMIO registers directly, using 68k assembly or tightly controlled C/assembly helpers.

This has several advantages:

- no ROM replacement;
- no bootloader modification;
- immediate text feedback on the NEO display;
- controlled one-line-at-a-time experiments;
- register state can be saved and restored before exit.

### Stage 1: passive register monitor

Read and display selected `DIR`, `DATA`, `SEL` and `PUEN` registers without changing them.

### Stage 2: controlled GPIO toggle

Only after continuity identifies a candidate control net, save its original register state, configure only the required bit, and toggle it slowly while observing the corresponding board node with a multimeter or oscilloscope.

Conceptual 68k sequence:

```asm
; save original byte
move.b  $FFFFFxxx,D0
move.b  D0,saved_reg

; alter only the verified target bit
bset    #n,D0
move.b  D0,$FFFFFxxx

; perform measurement

; restore original state
move.b  saved_reg,$FFFFFxxx
```

The actual implementation must use verified register addresses and preserve all unrelated bits.

### Stage 3: SD power-control test

Once the Q9/R52/U11 path is electrically understood, the application can toggle only the suspected enable line while measuring:

- U11 input;
- U11 output;
- SD pin 4 VDD.

No SD card is required for this stage.

### Stage 4: mechanical-input tests

If the socket exposes card-detect or write-protect contacts, monitor their candidate GPIO inputs while mechanically actuating them.

### Stage 5: bus connectivity

After the netlist is known, the application can drive or observe individual verified bus lines to prove CPU-to-footprint connectivity.

### Stage 6: minimum SD transaction

Only after voltage levels, bus direction and pin assignments are confirmed should the application attempt a real card transaction, beginning with the minimum initialization sequence required to establish card response.

## Safety rule for the diagnostic app

The diagnostic application must never perform a blind GPIO sweep. OS3K already owns keyboard, LCD, USB, wireless and other hardware. Every writable test must therefore:

1. target only a net already identified by continuity;
2. save the original register state;
3. modify the minimum possible bit set;
4. restore the original state on normal exit and error paths;
5. avoid enabling a supply until its destination and voltage range are known.

A bare-metal monitor remains a fallback only if OS3K ownership of the required hardware prevents a safe application-level test.

## Publication boundary

This repository documents independently reconstructed behavior and hardware observations. Proprietary ROM images and bulk firmware disassembly are intentionally not distributed here.

The purpose is to make the investigation reproducible from public MC68VZ328 documentation, board-level observations and clean-room functional conclusions.