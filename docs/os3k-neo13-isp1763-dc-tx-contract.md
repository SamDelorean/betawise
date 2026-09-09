# NEO13 / Small ROM ISP1763 Device Controller transmit contract

## Status

`PARCIAL_CERRADO / NO_RESUELTO` for the complete low-level transport family. This document describes an internal physical firmware object, **not** an A-line syscall and not a recovered AlphaSmart vendor symbol.

The transmit direction, three-argument core contract, and physical endpoint selector used by Small-ROM transport mode 3 are now **CONFIRMADO**. Higher-level vendor naming and the complete caller semantics for every selector remain `NO_RESUELTO / EVIDENCIA_INSUFICIENTE`.

## Source-first correlation

Historical updater source independently establishes that updater responses are represented as response data plus response length and are handed to a USB response/transmit path. Historical updater endpoint/mode numbering is not assumed to match later NEO Small-ROM numbering; canonical firmware prevails when the generations differ.

In canonical NEO Small ROM, transport mode 3 receives an already-built 8-byte response packet and calls a three-argument wrapper with `(mode=3, source_buffer, length=8)`. The wrapper calls a small selector helper, leaves that helper argument temporarily on the stack, then calls the transfer core with exactly three inputs.

## Selector helper

The helper mechanically maps wrapper-visible values as follows:

| wrapper value | raw ISP1763 EPINDEX value | physical meaning from ISP1763 register definition |
| --- | ---: | --- |
| 0 | `0x00` | endpoint 0 OUT / control OUT |
| 1 | `0x01` | endpoint 0 IN / control IN |
| 2 | `0x02` | endpoint 1 OUT / data OUT |
| 3 | `0x05` | endpoint 2 IN / data IN (TX) |

The mapping itself is firmware-confirmed. The endpoint decode follows the ISP1763 `ENDP_INDEX` definition: bits `[3:1]` select the endpoint and bit `0` selects direction (`0=OUT/RX`, `1=IN/TX`). The table does **not** assert that all four wrapper values have demonstrated live callers; only mode 3 is closed here at the upper-call level.

## Cross-image identity

The Small-ROM transfer core is a relocation-adjusted homologue of the NEO13 main-OS object documented as `F40226`: 415 of 430 bytes are identical, with all 15 differing bytes confined to five three-byte relocation fields. The local read/write helper implementations also match their NEO13 counterparts modulo placement.

The object operates on the ISP1763 Device Controller register family, including endpoint index, buffer/data port, DMA endpoint selection, and endpoint control.

## Corrected core calling convention

The transfer core is:

`F40226(epindex_selector, source_buffer, length)`

This supersedes the earlier four-argument interpretation.

The reason for the earlier mistake is now mechanically resolved: a direct sequence surrounding the call contains four longwords of total stack cleanup, but one is the argument passed to the selector helper and remains on the stack while the three core arguments are pushed. Therefore `cleanup 16` is **not** evidence that F40226 itself receives four arguments.

The first core argument's low byte is written to the ISP1763 `ENDP_INDEX` register (`0x2C`). The second argument is loaded as the source pointer. The third argument's low byte initializes the transfer count.

For Small-ROM transport mode 3, the selector helper produces raw `0x05`. Under the ISP1763 encoding this is endpoint 2, IN direction. Therefore the mode-3 path targets **EP2 IN/TX**.

## Confirmed data direction

During transfer, the core consumes bytes sequentially from the source pointer, packs one or two bytes as required, and writes through the Device Controller data-port path. The count advances toward completion in two-byte units with an odd-byte tail path.

This establishes:

- source-buffer direction: memory buffer **to** Device Controller data port — **CONFIRMADO**;
- operation class: endpoint write / transmit — **CONFIRMADO**;
- core argument 1: raw `ENDP_INDEX` selector — **CONFIRMADO**;
- core argument 2: source buffer pointer — **CONFIRMADO**;
- core argument 3 low byte: transfer length — **CONFIRMADO**;
- Small-ROM transport mode 3 mapping: raw selector `0x05` = **EP2 IN/TX — CONFIRMADO**.

The former statement that F40226 had a fourth `mode/endpoint` argument is **withdrawn**. The alternative receive-path hypothesis is also refuted by both the source-buffer-to-data-port flow and the IN/TX selector.

## Generational caution

Historical updater material refers to an Endpoint 1 TX path in an earlier architecture. Canonical NEO13/Small-ROM firmware demonstrates EP2 IN/TX for the path analyzed here. These are retained as a real generational difference rather than normalized into one endpoint number.

## Reproducibility

Two private regressions are retained outside the public repository by project policy:

- `verify_neo13_smallrom_mode3_tx_contract_2026-09-09`: **EXECUTED 22/22 PASS** for TX data-flow and cross-image homology;
- `verify_neo13_mode3_epindex_contract_2026-09-09`: **EXECUTED 19/19 PASS** for helper mapping, three-core-input stack shape, EPINDEX write, raw selector `0x05`, EP2 decode, and IN direction.

Binary firmware, extensive disassembly, and private regression artifacts remain outside this repository.

## ABI boundary

Nothing in this result promotes `F40226`, the Small-ROM worker, Manager selector `0x18`, or physical address `0x00400420` into the OS3K A-line ABI. The A-line frontier is unchanged.
