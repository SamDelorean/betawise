# Manager selector 0x09 — SET_BAUDRATE and Small-ROM dispatcher error 0x92

This note records the source-first and firmware-correlated reconstruction of Manager selector `0x09` across the principal NEO OS and both canonical Small ROM generations. It also corrects an earlier interpretation that treated response `0x92` as an unresolved condition internal to the baud-rate handler.

## Source-first identification

AlphaSync names request `0x09` as `ASMESSAGE_REQUEST_SET_BAUDRATE`, with `(baud32, z16)`, and names response `0x4A` as the baud-rate response. AlphaSync also records `0x86` as the invalid-baudrate error of the normal Manager protocol implementation.

## Small-ROM two-generation closure

Canonical Small-ROM handler entrypoints:

- AS3000 Small ROM: file `0x2976`, runtime `0x00402976`
- NEO Small ROM: file `0x1FC2`, runtime `0x00401FC2`

Both handlers are exactly `0x108` bytes and implement the same functional contract. They accept exactly five requested rates:

- 9600
- 19200
- 38400
- 57600
- 115200 bit/s

For an accepted rate, both implement the same staged transaction:

`0x09(requested_baud) -> 0x49(requested_baud) -> program/wait -> 0x4A(requested_baud)`

The complete AS3000/NEO Small-ROM handlers differ at only five bytes, all within the generation-specific per-rate hardware-programming parameter block. Their control topology, response `0x49`, final response `0x4A`, `0x92` block placement and UART MMIO accesses are otherwise homologous. A dedicated two-generation regression passed **42/42** assertions.

## Hardware correlation

The NEO Small ROM directly accesses the MC68VZ328 UART1 block:

- `0xFFFFF900` — `USTCNT1`
- `0xFFFFF902` — `UBAUD1`
- `0xFFFFF904` — `URX1`
- `0xFFFFF906` — `UTX1`

The first response is emitted before the link is reconfigured; the final response follows the hardware transition. Thus `0x49` is mechanically the pre-change acknowledgement stage for selector `0x09`, while `0x4A` is the post-change confirmation.

No vendor internal routine name is assigned.

## Principal OS versus Small ROM

The principal NEO13 OS and Small ROM share the five-rate contract and the staged `0x49 -> reconfigure -> 0x4A` success topology.

They do not share the same unsupported-rate handling. The principal OS reaches `0x86`, consistent with AlphaSync's `ASMESSAGE_ERROR_INVALID_BAUDRATE`. The Small-ROM handler's initial five-rate validation does not itself produce `0x86`.

## Correction: Small-ROM response 0x92 belongs to the dispatcher

Fresh direct analysis of the canonical NEO Small ROM shows that the `0x92` block at file `0x20A6` / runtime `0x004020A6` is not entered from the body of selector `0x09`.

The Manager dispatcher performs two independent checks before entering a command case:

1. it loads the command selector and compares it against upper bound `0x19`; a value greater than `0x19` branches directly to `0x20A6`;
2. for values `<= 0x19`, it searches the explicit supported-selector whitelist `00 02 07 08 09 0A 0B 16 17 18 19`; if no entry matches, it also branches directly to `0x20A6`.

At `0x20A6`, the common response path constructs response code `0x92`.

There is no branch from the `0x09` handler body to `0x20A6`; its successful `0x4A` path explicitly branches over the `0x92` block. Therefore the previous classification of `0x92` as an unresolved baud-handler condition is refuted.

The stable contract is:

`Small ROM unsupported Manager selector -> response 0x92`.

This also explains AlphaSync's historical observation that request `0x1A` returns `0x92` while the NEO is running Small ROM: `0x1A > 0x19` and is outside the Small-ROM 11-selector namespace.

A fresh directed regression over canonical NEO Small ROM SHA-256 `6402c40878ba28747642ce474361b1596555b2a1f137db79c7ea8bb5b35a9523` passed **16/16** assertions covering the upper-bound branch, whitelist search, not-found branch, common `0x92` response block, absence of any `0x09 -> 0x92` branch, and the successful `0x09` branch that skips this error block.

## Status

- `0x09 SET_BAUDRATE` Small-ROM contract, five accepted rates and two-generation homology: **CONFIRMED / CERRADO**.
- `0x49` as pre-change acknowledgement stage for `0x09`: **CONFIRMED mechanically**.
- `0x4A` as post-change baud response: **CONFIRMED**.
- Small-ROM `0x92` as unsupported-selector response: **CONFIRMED**.
- Principal-OS `0x86` invalid-baud response remains distinct: **CONFIRMED**.
- Exact vendor names for internal helpers/globals: **DESCONOCIDO / EVIDENCIA_INSUFICIENTE**.

No A-line ABI entry is implied. The demonstrated A-line frontier remains A470.
