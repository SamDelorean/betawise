# A158 / index86 — SYS_A158 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered raw contract

```c
int32_t SYS_A158(
    uint8_t *buffer,
    uint32_t capacity,
    uint32_t *received_out,
    uint32_t flags,
    uint8_t terminator,
    uint16_t wait_delay);
```

No original vendor name has been recovered, so the neutral `SYS_A158` name is retained.

`buffer` and `received_out` are raw mandatory pointers. The firmware handler does not perform NULL validation.

## Confirmed operation

`A158` is a UART **receive-buffer** service, not a transmit operation. Across the canonical AS3000 2005, NEO 2005 and NEO 2013 firmware implementations it:

- reads the UART receive register;
- copies ready bytes sequentially into `buffer`;
- stops when `capacity` bytes have been stored or another enabled completion condition occurs;
- writes the number of stored bytes to `*received_out` on normal and error exits.

The three public handlers have the same `0x106`-byte length and the same semantics after normalizing generation-specific state and timer-service relocations.

## Flags

The following bits are mechanically established:

- `0x04`: **stop on terminator**. The received byte is stored first; if it equals the low byte of `terminator`, the call completes successfully. The terminator byte is included in `*received_out`.
- `0x08`: **nonblocking / drain currently available receive data**. Once no more data is ready, the call returns success immediately, including the zero-byte case. This mode suppresses the timed-wait behavior below.
- `0x10`: **timed empty-FIFO wait**. When no receive byte is ready, the routine arms the OS timer service. If data arrives first it cancels that timer, consumes data, and rearms a fresh timer when the receive FIFO becomes empty again.

With neither `0x08` nor `0x10`, the routine waits for receive data without a timer-backed empty-FIFO completion.

The `0x10` behavior is therefore an inactivity/inter-byte wait rather than one absolute transaction deadline.

## Timer relationship

The two internal service calls made by `A158` resolve directly to:

- `A0E8` / index58 — timer arm;
- `A0EC` / index59 — timer cancel.

`wait_delay` is passed directly as the timer delay argument. Existing reconstruction of `A0E8` strongly supports milliseconds as the unit, but no vendor spelling for this `A158` parameter has been recovered; the ABI declaration therefore keeps the neutral `wait_delay` name.

A timer expiration is a normal partial-completion condition, not an `A158` error.

## Return values

Confirmed public return values are:

| Return | Meaning established from firmware |
| ---: | --- |
| `0` | normal completion, including timeout/nonblocking partial receive |
| `-512` | zero capacity |
| `-513` | UART receive overrun |
| `-514` | UART receive framing error |
| `-515` | timer-arm failure |
| `-516` | UART subsystem inactive |

The routine explicitly tests receive-data-ready, overrun and framing-error state. Other UART status bits are not assigned new meanings here.

## Caller evidence

Direct absolute firmware callers found during the private audit:

- AS3000 2005: 5
- NEO 2005: 0
- NEO 2013: 0

All five AS3000 callers construct six ABI argument slots compatible with the recovered contract. Their observed capacities include `1`, `2`, `0x400` and a variable count. They all use flag `0x10`, terminator `0`, and wait values including `30`, `100`, `250`, `1000` and `3000`.

An AlphaWord Plus 2005 scan finds `A158` only in its normal contiguous A-line stub table; it does not establish another inline caller.

## Generation note

The NEO 2013 `A158` implementation still reads the same UART receive-register address used by the 2005 implementations, while other serial-control routines in that generation include a second UART-register-bank difference. `A158` also has no direct internal NEO 2013 caller in the audited firmware.

Those facts are confirmed, but the precise relationship between this compatibility-facing service and NEO 2013's internal secondary-UART architecture remains unknown and is intentionally not inferred into the public API.

## Correction to the preliminary reconstruction

An early checkpoint described four public arguments. Full stack accounting proved that incorrect: the routine also consumes the low byte of a fifth 32-bit ABI slot and the low word of a sixth slot. Concrete AS3000 callers independently corroborate all six slots.

## Regression status

Static regression: **EXECUTED — 128/128 PASS**.

It verifies the canonical ROM identities, vector/handler bounds, six-slot ABI, receive-register access, flags, return map, timer-service relationships, three-generation normalized equivalence, internal caller counts and the AlphaWord Plus stub-table negative cross-check.

Dynamic UART/loopback regression: **SPECIFIED / NOT EXECUTED**. Future emulator or hardware probes should cover blocking receive, nonblocking drain, terminator completion, inter-byte timeout, overrun and framing-error paths.

## Confidence

- **CONFIRMED:** six-slot ABI, receive direction, buffer/capacity/count behavior, flags `0x04`/`0x08`/`0x10`, terminator handling, timer arm/cancel relationship, return values, direct caller evidence, three-generation handler equivalence.
- **STRONG INFERENCE:** `wait_delay` uses milliseconds, inherited from the independently reconstructed `A0E8` timer domain.
- **UNKNOWN:** original vendor function/flag names and the exact architectural relationship to NEO 2013's alternate UART bank.

Raw firmware, extensive disassembly and private audit material remain outside the public repository.
