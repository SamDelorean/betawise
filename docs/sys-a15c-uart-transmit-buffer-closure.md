# A15C / index87 — SYS_A15C — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered raw contract

```c
typedef void (*SYS_A15C_TxCallback)(uint8_t tx_status);

int32_t SYS_A15C(
    const uint8_t *buffer,
    uint32_t length,
    uint32_t *sent_out,
    uint32_t flags,
    uint16_t wait_delay,
    SYS_A15C_TxCallback callback,
    uint8_t callback_events);
```

No original vendor function name has been recovered, so `SYS_A15C` remains the public neutral name.

## Correction to the inherited BetaWise note

The historical syscall-table comment calls `A15C` “serial send byte”. That identifies the correct subsystem and direction, but it is not the full contract.

Firmware shows a seven-slot **buffer transmit** API with a 32-bit length, partial-send count, behavior flags, timer-backed waiting, and an optional one-shot transmit-status callback.

## Confirmed transmit behavior

The routine sends bytes from `buffer` while the UART transmit FIFO reports an available slot. Each transmitted byte:

1. is written to the UART TX data register;
2. advances the source pointer;
3. increments the local sent count;
4. decrements the remaining `length`.

`*sent_out` receives the number of bytes actually submitted to the transmitter on normal, partial and timer-error exits. The raw firmware does not NULL-check `sent_out`; `buffer` must be valid when `length` is nonzero.

A zero `length` is a successful no-op and reports zero bytes sent.

## Flags

The following control-flow bits are mechanically established when the transmit FIFO does not currently report an available slot:

- `0x02`: return success immediately with the partial sent count.
- `0x04`: after the UART `CTS_STATUS` condition is observed, return success immediately with the partial count.
- `0x08`: after the preceding conditions, use the OS timer service for a bounded wait. Timer expiration is normal partial completion, not an error.

The routine does not explicitly test `flags & 0x01`, although many native callers pass `flags == 1`. Its original source-level meaning therefore remains unknown.

The exact firmware precedence in the unavailable-slot path is: test `0x02`, inspect `CTS_STATUS`, test `0x04`, then select the `0x08` timer path or continue polling.

## Timer relationship

The wait path calls the already reconstructed OS timer services directly:

- `A0E8` / index58 — arm timer;
- `A0EC` / index59 — cancel timer.

`wait_delay` is passed as the low word of the timer delay. The independent `A0E8` reconstruction strongly supports milliseconds as the unit, but no vendor parameter name has been recovered here.

If arming the timer fails, `A15C` returns `-515`. A normal timer expiration returns `0` with the number of bytes sent so far.

## Optional transmit callback

`callback` and `callback_events` install a one-shot UART transmit-status callback under the same interrupt-masking critical-section helpers used elsewhere in the serial subsystem.

When `callback` is NULL, the persistent callback event mask is cleared. For a non-NULL callback, firmware keeps only `callback_events & 0x47` and translates those UART event-enable bits into the corresponding high-byte transmit-status mask:

| `callback_events` bit | UART event | transmit-status mask |
| ---: | --- | ---: |
| `0x01` | TX available | `0x20` |
| `0x02` | TX FIFO half-empty | `0x40` |
| `0x04` | TX FIFO empty | `0x80` |
| `0x40` | CTS delta | `0x01` |

The later UART service route compares the current transmit status with that mask. On a match it consumes the callback as one-shot state and invokes it with the transmit status byte.

Concrete AS3000 and NEO 2013 callers install a non-NULL callback with `callback_events == 0x04`, independently corroborating the FIFO-empty callback path.

## Return values

Confirmed public results are:

| Return | Meaning established from firmware |
| ---: | --- |
| `0` | normal completion, including zero-length and partial completion caused by flags/timer expiration |
| `-515` | timer-arm failure |
| `-516` | UART subsystem inactive |

No other negative public return code is produced by the audited handler.

## Caller evidence

Direct absolute firmware callers found during the private audit:

- AS3000 2005: 12
- NEO 2005: 4
- NEO 2013: 7

The callers construct seven ABI slots and include ordinary `flags == 1` calls, timed `flags == 0x08` calls, a `flags == 0x04` path, and non-NULL callback / `callback_events == 0x04` cases.

An AlphaWord Plus 2005 scan finds `A15C` only in the normal contiguous A-line stub table; no additional inline caller was established there.

## Generation differences

AS3000 2005 and NEO 2005 use the UART register bank at `F90x` for this transmit service.

NEO 2013 contains a real platform-specific preamble that clears receiver-enable state in the `F910` control register, then transmits through the `F91x` UART bank. After preserving and normalizing that explicit register-bank difference and normalizing relocated globals/helpers, the remaining `A15C` control flow is equivalent to the 2005 implementation.

This is deliberately kept separate from `A158`: the NEO 2013 receive-buffer compatibility service audited at `A158` still references its inherited receive path, whereas `A15C` uses the later `F91x` transmit bank. The exact vendor architectural rationale is unknown.

## Regression status

Static regression: **EXECUTED — 121/121 PASS**.

It verifies canonical ROM identities, handler bounds, seven-slot ABI, UART register usage, platform-specific NEO 2013 preamble, transmit loop, flags, timer services, callback/event mapping, return values, concrete caller patterns, normalized cross-generation equivalence and the AlphaWord Plus stub-table negative cross-check.

Dynamic UART/loopback regression: **SPECIFIED / NOT EXECUTED**. Future probes should cover multibyte transmission, partial/nonblocking behavior, CTS-conditioned behavior, timer expiration and FIFO-empty callback delivery.

## Confidence

- **CONFIRMED:** seven-slot ABI, buffer transmit operation, sent count, flag-bit control flow, timer relationship, callback/event mapping, return values, callers and NEO 2013 register-bank difference.
- **STRONG INFERENCE:** `wait_delay` is expressed in milliseconds via the independently reconstructed `A0E8` timer domain.
- **UNKNOWN:** original vendor name, original flag/typedef names and the source-level meaning of `flags & 0x01`.

Raw firmware, extensive disassembly and private audit material remain outside the public repository.
