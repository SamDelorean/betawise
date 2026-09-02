# A140 / index80 — SYS_A140 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

The original vendor symbol has not been recovered, so the neutral `SYS_A140` name is retained. BetaWise historically comments this entry as `configure serial`; direct firmware analysis confirms that role and recovers the actual ABI.

## Recovered contract

```c
int32_t SYS_A140(uint32_t baud_rate, uint32_t config);
```

`A140` takes **two 32-bit argument slots**. The second slot is a packed configuration value; the first is the requested baud rate. This corrects any interpretation of the routine as a one-argument configuration call.

## Supported baud rates

`A140` delegates baud programming directly to `A144` / index81. The audited firmware accepts:

- 9600
- 19200
- 38400
- 57600
- 115200

An unsupported rate produces status `-502`, propagated from `A144`.

The actual baud-generator constants differ between AS3000 and NEO, which is a real platform difference rather than an ABI difference.

## Packed configuration fields

Four nibbles map directly to documented DragonBall UART controls.

### Parity — `config & 0x000000F0`

- `0x10`: no parity
- `0x20`: even parity
- `0x30`: odd parity
- otherwise: `-503`

### Stop bits — `config & 0x0000000F`

- `1`: one stop bit
- `2`: two stop bits
- otherwise: `-504`

### Character length — `config & 0x00000F00`

- `0x100`: 8-bit characters
- `0x200`: 7-bit characters
- otherwise: `-505`

### CTS handling — `config & 0x0000F000`

- `0x1000`: ignore CTS (`NOCTS` enabled)
- `0x2000`: honor CTS hardware flow control
- otherwise: `-509`

### Platform/mode selector — `config & 0x000F0000`

The original enum/name for this field is not recovered.

- `0x10000` is accepted by all three audited generations.
- AS3000 also accepts `0x20000` and changes platform transceiver state.
- NEO 2005 rejects `0x20000`.
- NEO 2013 accepts `0x20000` through a distinct private early path.
- unsupported values return `-511`.

The `0x20000` path is deliberately left semantically unnamed until independent source evidence establishes its original meaning.

## Hardware behavior

The normal path constructs the UART status/control value from the selected parity, stop-bit and character-length fields, configures CTS behavior in the transmitter register, applies platform-specific communications GPIO state, writes the UART control state, and performs the receiver read required by the DragonBall initialization sequence.

`A140` then updates runtime serial state initialized by `A13C`. This confirms the division of responsibility:

- `A13C`: base UART/serial state initialization/reset;
- `A140`: operational serial configuration;
- `A144`: baud-generator programming;
- later syscalls in the family perform open/runtime I/O operations.

## Return status

Confirmed public results include:

- `0`: success
- `-502`: unsupported baud rate
- `-503`: invalid parity field
- `-504`: invalid stop-bit field
- `-505`: invalid character-length field
- `-509`: invalid CTS/flow-control field
- `-511`: invalid platform/mode selector

A NEO firmware startup caller tests the returned value and reports a UART startup error on non-zero status, independently confirming that the return value is contractual.

## Concrete firmware callers

The audited firmware uses ordinary settings such as `9600` and `115200` baud with packed values including `0x11111`, `0x12111` and `0x21111`. These callers independently confirm the two-argument order and the packed-field interpretation.

## Generation comparison

The high-level contract is shared by AS3000 2005, NEO 2005 and NEO 2013, but platform implementation details differ:

- AS3000 has additional external RS-232/IrDA GPIO handling.
- NEO has a reduced/different GPIO sequence.
- support for packed selector `0x20000` differs by generation.
- baud-generator constants used by `A144` differ between AS3000 and NEO.

These differences are preserved rather than normalized away.

## Regression status

Static regression: **EXECUTED — 156/156 PASS**.

It verifies canonical firmware identities, A140/A144 vector boundaries, the two argument slots and their order, packed-field masks/accepted values/hardware effects, status codes, serial-state relationships, direct caller counts and concrete caller values, and the direct delegation to A144.

Dynamic UART regression: **SPECIFIED / NOT EXECUTED**. It should be performed emulator-first before exercising invalid configurations on physical hardware.

## Confidence

- **CONFIRMED:** two-argument ABI, status return, supported baud rates through A144, parity/stop/7-8-bit/CTS fields, opaque high selector behavior, UART hardware effects, A13C/A144 relationships, callers and generation differences.
- **STRONG INFERENCE:** the high selector represents a transport/platform operating mode.
- **UNKNOWN:** original vendor symbol and enum names, exact semantic name of the `0x20000` mode, and names of private helper routines.

Raw firmware, ROM offsets, extensive disassembly and private reverse-engineering corpus remain outside the public repository.