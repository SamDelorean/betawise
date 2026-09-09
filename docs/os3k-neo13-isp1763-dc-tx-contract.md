# NEO13 / Small ROM ISP1763 Device Controller transmit contract

## Status

`PARCIAL_CERRADO / NO_RESUELTO` for the complete low-level contract. This document describes an internal physical firmware object, **not** an A-line syscall and not a recovered AlphaSmart vendor symbol.

The transmit direction and the central buffer/length contract are now **CONFIRMADO**. The exact vendor semantics of the two endpoint/control selector arguments remain `NO_RESUELTO / EVIDENCIA_INSUFICIENTE`.

## Source-first correlation

Historical updater source establishes the semantic direction independently of the later firmware analysis: updater responses are expressed as a pointer to response data plus a response length, and the USB response path primes endpoint transmission. Historical updater mode numbers are a separate numbering domain and are not mapped numerically onto Small-ROM transport modes 3/4/5.

In the canonical NEO Small ROM, transport mode 3 receives the already-built 8-byte response packet and passes `(mode=3, buffer, length=8)` through its transport wrapper. The wrapper derives one control value from the mode and forwards four machine-word arguments to the low-level transfer core.

## Cross-image identity

The Small-ROM transfer core is a relocation-adjusted homologue of the NEO13 main-OS object documented as `F40226`: 415 of 430 bytes are identical, with all 15 differing bytes confined to five three-byte relocation fields. The local read/write helper implementations also match their NEO13 counterparts modulo placement.

The object operates on the ISP1763 Device Controller register family, including DMA endpoint selection, endpoint index, data port, and endpoint control.

## Confirmed data direction

The second low-level argument is loaded as the source pointer. The third argument's low byte initializes the transfer count. During transfer the core consumes bytes sequentially from the source pointer, packs one or two bytes as required, and sends them through the Device Controller data-port write path. The count advances toward completion in two-byte units with an odd-byte tail path.

This establishes:

- source-buffer direction: memory buffer **to** Device Controller data port — **CONFIRMADO**;
- operation class: endpoint write / transmit — **CONFIRMADO**;
- argument 2: source buffer pointer — **CONFIRMADO**;
- argument 3 low byte: transfer length — **CONFIRMADO**;
- argument 1: derived endpoint/control value — structurally bounded, exact vendor semantics `NO_RESUELTO`;
- argument 4: mode/endpoint selector carried into the transfer core — structurally bounded, exact vendor semantics `NO_RESUELTO`.

The earlier alternative that this object might be a receive path is refuted by the observed source-buffer-to-data-port flow and by the independent updater response/TX source correlation.

## Caller shape

The canonical NEO13 direct caller uses the same four-machine-word calling convention and removes 16 bytes of arguments after the transfer call. Small ROM exposes a three-argument wrapper `(mode, buffer, length)` that derives the additional control value and enters the homologous four-argument core.

No original AlphaSmart function name is claimed.

## Reproducibility

Private regression `verify_neo13_smallrom_mode3_tx_contract_2026-09-09` was **EXECUTED** against canonical NEO13 and Small ROM images: **22/22 checks passed**. It checks canonical hashes, the Small-ROM mode-3 wrapper bridge, cross-image homology boundaries, source-buffer and length argument loads, the data-port write loop and tail, relevant Device Controller register use, and the NEO13 caller's 16-byte argument cleanup.

Binary firmware, extensive disassembly, and private regression artifacts remain outside this repository by project policy.

## ABI boundary

Nothing in this result promotes `F40226`, the Small-ROM worker, Manager selector `0x18`, or address `0x00400420` into the OS3K A-line ABI. The A-line frontier is unchanged.
