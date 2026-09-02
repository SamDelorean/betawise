# A13C / index79 — SYS_A13C — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered public contract

```c
void SYS_A13C(void);
```

The original vendor symbol has not been recovered, so the neutral `SYS_A13C` name is retained.

## Confirmed role

`A13C` initializes the base software and hardware state used by the OS3K UART/serial subsystem before the later configuration/open operations in the same syscall family.

Across the canonical AS3000 2005, NEO 2005 and NEO 2013 firmware generations, the routine:

- consumes no public arguments;
- initializes a small group of persistent serial-state variables;
- establishes the platform-specific UART-related GPIO baseline;
- disables/resets the DragonBall UART control state before later configuration;
- invokes OS-internal setup helpers around that state transition;
- returns after restoring the surrounding OS execution state.

The implementation writes zero to `D0` near exit, but audited firmware callers do not consume that value. Therefore a public integer return is not promoted into the reconstructed ABI; `void` is the conservative contractual type.

## Hardware correlation

The AS3000 implementation manipulates the documented UART/communications pins for the built-in DragonBall UART, including the RS-232-driver and IrDA-enable paths, as well as the UART RX/TX/flow-control pin group. The NEO implementations retain the same UART-state architecture with a reduced/different platform GPIO sequence.

This hardware correlation is independent of the neighboring BetaWise comments and refutes interpreting `A13C` as an unrelated generic initialization routine.

## Relationship with A140

BetaWise identifies `A140` (index80) as the serial configuration operation. Direct firmware analysis shows that `A140` consumes and updates state initialized by `A13C` and operates on the same UART hardware domain.

Accordingly, the division of responsibility supported by firmware is:

- `A13C`: initialize/reset base serial/UART state;
- `A140`: configure the operational serial parameters;
- later entries in the family perform open/runtime I/O operations.

This does not establish an original vendor name for `A13C`.

## Generation comparison

The core contract is common to all three audited generations, but the hardware sequence is intentionally platform-specific.

- AS3000 performs additional RS-232 and IrDA GPIO initialization.
- NEO 2005 has a shorter platform sequence.
- NEO 2013 adds one private helper invocation not present in NEO 2005; its exact private purpose remains unknown and is not exposed as part of the public ABI.

## Caller audit

Direct internal firmware callers were found in all three generations. They invoke `A13C` without preparing a public argument and do not rely on a return value, supporting the reconstructed `void(void)` interface.

## Refutation checks

The closure explicitly tested and rejected the following alternatives:

- **unrelated generic subsystem initializer:** contradicted by direct UART-control and UART/communications-GPIO effects;
- **full baud/protocol configurator:** contradicted by the separate `A140` configuration path that consumes the state initialized here;
- **function with public arguments:** no public argument slot is consumed and callers do not prepare one;
- **function with contractual integer status:** although the implementation leaves zero in `D0`, callers do not consume it, so the ABI does not require such a return.

## Regression status

Static regression: **EXECUTED — 67/67 PASS**.

The private reproducible regression verifies the three canonical firmware identities, index79 vector resolution, routine boundaries, argument absence, state initialization, UART-control reset, platform GPIO anchors, helper relationships, direct caller counts, the state relationship with `A140`, and the NEO 2013-only helper difference.

Dynamic hardware/emulator regression: **SPECIFIED / NOT EXECUTED**. A future probe should observe UART-control/GPIO/software state before and after `A13C`, then exercise `A140` and the serial-open path.

## Confidence

- **CONFIRMED:** index79/A13C identity, no-argument public ABI, UART/serial base-state initialization role, persistent state initialization, UART hardware reset, platform-specific GPIO effects, relationship with `A140`, generation differences and caller behavior.
- **STRONG INFERENCE:** the paired private entry/exit helpers preserve an OS critical/execution state, and the two setup helpers belong to serial interrupt/event infrastructure.
- **UNKNOWN:** original vendor symbol, exact private names of each persistent state field/helper, and the exact purpose of the NEO 2013-only helper.

Raw firmware, ROM offsets, extensive disassembly and private reverse-engineering corpus remain outside the public repository.