# OS3K Small ROM entry 0x00400420: selector 0x18 response transports

## Status

`PARCIAL_CERRADO / NO_RESUELTO` for the complete transport set. This is a physical Small ROM entry/protocol object, **not** an A-line syscall.

The recognized response modes currently have these evidence levels:

- mode 3: ISP1763 Device Controller **EP2 IN/TX response path — CONFIRMADO**;
- mode 4: three-wire GPIO transport — **CONFIRMADO** mechanically; external endpoint/protocol remains `NO_RESUELTO / EVIDENCIA_INSUFICIENTE`;
- mode 5: direct DragonBall UART response transport — **CONFIRMADO**.

## Source-first context

The NEO updater host flow enters Small ROM with Manager selector `0x18` and expects status `0x56`. Independent canonical-firmware work shows that NEO13 Manager selector `0x18` calls the physical Small ROM entry at `0x00400420`; that entry masks interrupts and transfers to a worker that discriminates modes `3`, `4`, and `5`. Each recognized mode builds an 8-byte response with status `0x56` before using a mode-specific delivery path.

Historical updater material establishes a USB response/TX architecture but uses an earlier endpoint/mode numbering domain. No numerical equivalence to NEO Small-ROM modes is assumed.

The names below are descriptive analyst labels only. No AlphaSmart vendor symbols are asserted.

## Mode 3: ISP1763 EP2 IN/TX path

Mode 3 passes the built 8-byte response buffer to a three-argument transport wrapper `(mode=3, source_buffer, length=8)`.

The wrapper's selector helper maps visible values `0,1,2,3` to raw ISP1763 `ENDP_INDEX` values `0,1,2,5` respectively. Independent ISP1763 documentation defines bits `[3:1]` as endpoint index and bit `0` as direction (`0=OUT/RX`, `1=IN/TX`). Thus mode 3 maps to raw `0x05`, i.e. **endpoint 2 IN/TX**.

Cross-image correlation closes the low-level implementation. The Small ROM mode-3 core is a relocation-adjusted homologue of the NEO13 main-OS endpoint-transfer object `F40226`: 415 of 430 bytes are identical, and the remaining 15 bytes are confined to five three-byte relocation fields. Its local read/write helpers are likewise byte-identical modulo placement to the main-OS MMIO helper pair.

The corrected core signature is:

`F40226(epindex_selector, source_buffer, length)`

The core writes the first argument's low byte to ISP1763 `ENDP_INDEX`, consumes the second argument as the source pointer, and uses the third argument's low byte as the transfer length. The observed four-longword stack cleanup around a direct caller includes one residual selector-helper argument plus the three core arguments; it does **not** establish a fourth F40226 argument.

Therefore:

- mode 3 transport family = ISP1763 Device Controller — **CONFIRMADO**;
- mode 3 raw selector = `0x05` — **CONFIRMADO**;
- mode 3 physical target = endpoint 2 IN/TX — **CONFIRMADO**;
- transfer direction = source buffer to Device Controller data port — **CONFIRMADO**;
- the previous four-argument core interpretation is **withdrawn**.

The helper also mechanically maps wrapper values `0→0x00`, `1→0x01`, and `2→0x02`, corresponding physically to EP0 OUT, EP0 IN, and EP1 OUT. Their live upper callers/roles are not asserted here and remain to be correlated independently.

## Mode 4: three-wire GPIO path

Mode 4 is mechanically distinct from mode 3. It uses two DragonBall GPIO inputs and one GPIO output: PF7 and PM1 are configured as inputs without pull-ups, while PE0 is configured as an output. The worker waits on the two input handshakes and serializes each source byte through PE0, testing bit 7 and repeating for eight bit cycles.

Therefore the three-wire bit-bang transport itself is **CONFIRMADO**. The external device and upper protocol remain `NO_RESUELTO / EVIDENCIA_INSUFICIENTE`. Correlation with the NEO 2 RF subsystem is an **INFERENCIA FUERTE**, not a confirmed electrical net mapping.

Do not confuse this Small-ROM transport selector value `4` with internal MMIO accessor mode numbers used elsewhere in the firmware; they are unrelated numbering domains.

## Mode 5: UART response path

Mode 5 is semantically identified at the hardware-transport level.

Canonical NEO13 Small ROM copies exactly eight bytes from the constructed response buffer to absolute-short address `0xF907`, one byte per iteration.

The Motorola/NXP DragonBall UART transmitter register occupies the corresponding peripheral address, so the low-byte write is to UART TX data rather than ordinary RAM.

Therefore:

- mode 5 response destination = DragonBall UART transmitter data register;
- transfer length = exactly 8 bytes;
- payload = the Small ROM response packet whose status byte is `0x56`;
- classification: **CONFIRMADO** for direct UART transmission of the response.

## Service-loop numbering caution

The later Small-ROM service loop dispatches selector values `3`, `4`, and `5`. Those service-loop selector values form a distinct numbering domain from the mode-to-EPINDEX helper described above. In particular, service-loop selector `5` is the UART/event route; raw ISP1763 EPINDEX `0x05` means EP2 IN/TX. They must not be conflated.

## ABI boundary

Neither Manager selector `0x18` nor physical address `0x00400420` is promoted to the OS3K A-line ABI. This analysis concerns an internal Manager/Small-ROM protocol and physical firmware objects.

## Reproducibility

Private canonical-firmware regressions retained by project policy include:

- Small-ROM entry18 response-mode regression: **31/31 PASS**;
- mode-3 transport homology regression: **18/18 PASS**;
- mode-3 TX contract regression: **22/22 PASS**;
- mode-3 EPINDEX/calling-convention regression: **19/19 PASS**;
- mode-4 GPIO protocol regression: **23/23 PASS**.

No binary firmware, large disassembly, or Ghidra artifact is published here.
