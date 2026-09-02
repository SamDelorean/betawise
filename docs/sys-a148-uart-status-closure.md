# A148 / index82 — SYS_A148 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Historical correction

BetaWise inherited the comment:

```c
DEFINE_SYSCALL(82, SYS_A148); // open serial
```

Direct firmware analysis and every audited caller contradict that description. `A148` does **not** open or configure the serial port. The vector index is not displaced: index82 resolves to the same status-query routine in the canonical AS3000 2005, NEO 2005 and NEO 2013 firmware.

The vendor symbol has not been recovered, so the neutral `SYS_A148` name is retained rather than inventing a replacement name.

## Recovered contract

```c
void SYS_A148(uint8_t *tx_status_out, uint8_t *rx_status_out);
```

Both output pointers are required by the raw firmware routine; it contains no NULL check.

## Confirmed behavior

The routine consults the serial/UART active-state byte initialized by `A13C` and established on the operational configuration path by `A140`.

When the subsystem is active:

```c
*tx_status_out = high_byte(UTX);
*rx_status_out = high_byte(URX) & 0xF0;
```

where `UTX` and `URX` are the DragonBall UART transmitter and receiver registers.

The function is therefore a read-only hardware/status snapshot apart from writing the caller-provided output bytes. It does not program `USTCNT` or `UBAUD`, enable transceivers, allocate buffers, or perform an open transition.

## Inactive-state generation difference

The inactive sentinel changed across firmware generations:

- AS3000 2005: both outputs receive `0x00`;
- NEO 2005: both outputs receive `0x00`;
- NEO 2013: both outputs receive `0xFF`.

This difference is explicit in the firmware and must be preserved by an emulator or compatibility layer.

## Caller audit

Direct internal callers found:

- AS3000 2005: 5
- NEO 2005: 1
- NEO 2013: 2

They consistently prepare two output locations before calling `A148` and inspect the resulting bytes/bits afterward. A NEO 2013 caller explicitly compares one output with `0xFF`, independently corroborating the 2013 inactive sentinel.

No audited caller uses `A148` as an open operation or consumes a return status in `D0`.

## Refutation checks

- **`open serial`: refuted.** Primary firmware has no open/configuration side effects.
- **vector/index mismatch: refuted.** Index82 and all direct calls converge on the same routine in all three canonical ROMs.
- **status return in `D0`: refuted as public contract.** Callers consume the two output bytes instead.
- **cross-generation identical sentinel: refuted.** NEO 2013 uses `0xFF` instead of the 2005 `0x00`.

## Regression status

Static regression: **EXECUTED — 54/54 PASS**.

It verifies canonical firmware hashes, vector82, routine boundaries, the two output-pointer slots, active-state lookup, inactive sentinels, `UTX`/`URX` register reads, RX masking, exact direct-caller counts, caller output preparation and downstream result tests.

Dynamic UART/loopback status verification: **SPECIFIED / NOT EXECUTED**.

## Confidence

- **CONFIRMED:** two-output raw ABI, UART TX/RX status semantics, active-state dependency, 2005/2013 inactive-sentinel difference, caller behavior, and incorrect historical `open serial` comment.
- **UNKNOWN:** original vendor function name and original symbolic names for the exposed status bytes/bitfields.

Raw firmware, ROM offsets, extensive disassembly and private reverse-engineering corpus remain outside the public repository.