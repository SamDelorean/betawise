# A128 / index74 — SYS_A128 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
uint32_t SYS_A128(void);
```

`A128` returns a profile-dependent battery curve percentage/coordinate in the range 0..100. No reliable vendor symbol has been recovered, so the neutral `SYS_A128` name is retained.

**Semantic correction after the sequential A138 revalidation:** this result must not be described unqualified as the user-visible *remaining battery capacity*. The Battery Status handler at `A138` consumes `A128` and explicitly computes `100 - A128()` for the displayed remaining percentage. Thus `A128` is the curve-side complementary percentage (equivalently a depletion/used-capacity coordinate in that UI path), while `A138` presents its complement as remaining capacity.

## Confirmed behavior

The routine takes no public arguments. It reads the battery profile exposed by `A124`:

- `0` selects the NiMH curve;
- `1` selects the Alkaline curve;
- any other profile returns `0`.

It then invokes a private interpolation helper over the persistent battery measurement refreshed by `A120` and returns a longword result in the range 0..100.

The interpolation helper walks private 12-byte curve records. A measurement below the first breakpoint returns 0; a terminal table record returns 100; intermediate points use the stored linear transform for the selected curve. AS3000 and NEO use different internal fixed-point scaling, but expose the same public numeric domain and curve orientation.

## Caller evidence

Each canonical ROM has two direct callers. Both consume the return value. The Battery Status display path computes the complementary `100 - A128()` quantity before formatting the user-visible battery capacity. The internal battery-management path also preserves the returned value for subsequent logic.

This caller evidence fixes the orientation that the earlier A128-only audit could not safely name: the raw 0..100 result is **not** itself the remaining-capacity percentage shown to the user.

## Correlation

Historical `BatteryTester.c` establishes the earlier battery-voltage acquisition architecture. `A120` refreshes the modern calibrated measurement, `A124` identifies the NiMH/Alkaline profile, and `A128` applies the corresponding curve. `A138` then complements the `A128` result for the remaining-capacity presentation. These independent layers distinguish measurement, chemistry selection, curve conversion, and UI representation.

## Confidence

- **CONFIRMED:** `uint32_t(void)` ABI; NiMH/Alkaline curve selection; use of `A120` measurement state; lower result 0, upper result 100; interpolated curve percentage; callers 2/2/2 and caller consumption; `A138` computes `100 - A128()` for remaining-capacity display; real AS3000/NEO fixed-point differences.
- **STRONG INFERENCE:** the most useful neutral semantic description is a depletion/used-capacity curve percentage rather than remaining capacity.
- **UNKNOWN:** original vendor routine name and factory derivation of the private curve breakpoints.

Static regression: **56/56 PASS**. Dynamic virtual-battery sweep is specified but not executed.

Raw firmware, complete curve tables and private disassembly remain outside the public repository.
