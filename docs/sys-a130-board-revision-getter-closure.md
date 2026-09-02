# A130 / index76 — SYS_A130 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
uint32_t SYS_A130(void);
```

`A130` returns the System 3 board/hardware revision code, zero-extended from an internal byte. No reliable vendor symbol has been recovered, so the neutral `SYS_A130` name is retained.

## Source-first correlation

Historical AlphaWord release notes explicitly track **Board Compatibility** across hardware revisions and document a PowerModule correction for 1.8 boards. In the System Status firmware path, the return of `A130` is formatted immediately beside the localized `Board revision: ` label. This resolves the identity independently of the routine's later use by battery code.

## Firmware behavior

The canonical AS3000 2005, NEO 2005 and NEO 2013 handlers have the same ten-byte semantic body:

1. clear `D0`;
2. load the generation-specific board-revision byte into `D0.B`;
3. return.

There are no public arguments, helper calls, validation steps or side effects. The public return is therefore the zero-extended revision code.

## Caller evidence

Each canonical ROM has three direct callers. The System Status caller displays the value as `Board revision`. Other power/battery callers branch on zero versus nonzero; in particular `A120` uses the same state as a hardware-capability gate before attempting the analog battery measurement path.

This means the byte's use as a battery-measurement gate is secondary to its demonstrated primary identity as the board revision.

## Generation differences

AS3000 derives the byte from hardware-identification bits and observed initialization paths produce code 4 or 0. NEO initializes the corresponding state to `0x10` in its hardware/power setup. These are internal revision codes; this audit does not reinterpret them as decimal version strings without evidence.

## Confidence

- **CONFIRMED:** index76/A130 identity; `uint32_t(void)` ABI; pure zero-extending byte getter; Board revision semantics; callers 3/3/3; use of the same state as the `A120` hardware gate; real AS3000/NEO initialization differences.
- **UNKNOWN:** original vendor routine name and the complete encoding of all board revisions across all product generations.

Static regression: **32/32 PASS**. Dynamic multi-board regression is specified but not executed.

Raw firmware, caller offsets and private disassembly remain outside the public repository.
