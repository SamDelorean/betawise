# A128 / index74 — SYS_A128 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
uint32_t SYS_A128(void);
```

`A128` returns the OS modelled battery-capacity percentage. No reliable vendor symbol has been recovered, so the neutral `SYS_A128` name is retained.

## Confirmed behavior

The routine takes no public arguments. It reads the battery profile exposed by `A124`:

- `0` selects the NiMH curve;
- `1` selects the Alkaline curve;
- any other profile returns `0`.

It then invokes a private interpolation helper over the persistent battery measurement refreshed by `A120` and returns a longword result in the range 0..100.

The interpolation helper walks private 12-byte curve records. A measurement below the first breakpoint returns 0; a terminal table record returns 100; intermediate points use the stored linear transform for the selected curve. AS3000 and NEO use different internal fixed-point scaling, but expose the same public percentage contract.

## Caller evidence

Each canonical ROM has two direct callers. Both consume the return value. The Battery Status display path computes the complementary `100 - percentage` quantity for its graphical representation, independently confirming percentage semantics. The internal battery-management path also preserves the returned value for subsequent logic.

## Correlation

Historical `BatteryTester.c` establishes the earlier battery-voltage acquisition architecture. `A120` refreshes the modern calibrated measurement, `A124` identifies the NiMH/Alkaline profile, and `A128` applies the corresponding capacity curve. These independent layers distinguish measurement, chemistry selection, and percentage conversion.

## Confidence

- **CONFIRMED:** `uint32_t(void)` ABI; NiMH/Alkaline curve selection; use of `A120` measurement state; lower result 0, upper result 100; interpolated percentage return; callers 2/2/2 and caller consumption; real AS3000/NEO fixed-point differences.
- **UNKNOWN:** original vendor routine name and factory derivation of the private curve breakpoints.

Static regression: **56/56 PASS**. Dynamic virtual-battery sweep is specified but not executed.

Raw firmware, complete curve tables and private disassembly remain outside the public repository.
