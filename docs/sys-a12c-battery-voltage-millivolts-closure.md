# A12C / index75 — SYS_A12C — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
uint32_t SYS_A12C(void);
```

`A12C` converts the persistent battery measurement refreshed by `A120` into the battery-pack voltage representation used by OS3K callers. The supported unit is millivolts. No reliable vendor symbol has been recovered, so the neutral `SYS_A12C` name is retained.

If no measurement is available (`measurement == 0`), the function returns `0`.

## Firmware behavior

The canonical AS3000 2005 implementation tests the measurement state, returns zero when absent, and otherwise applies its platform-specific fixed-point conversion with a calibration factor and additive offset. NEO 2005 and NEO 2013 use the same semantic structure with a different shared NEO calibration factor/offset.

The routine takes no public arguments and returns a fully defined 32-bit value in `D0`.

## Millivolt correlation

Multiple firmware callers execute `A120` followed by `A12C`, consume the returned value, and compare it directly with battery-protection thresholds including decimal 3500 and, on AS3000 paths, 3700. These magnitudes correspond to the documented three-cell battery pack when expressed in millivolts.

The Battery Status display path also consumes the `A12C` result through its numeric formatter. This separates the layers cleanly:

- `A120`: refresh calibrated battery measurement state;
- `A12C`: convert that measurement to pack voltage in mV;
- `A128`: convert the measurement/profile model to capacity percentage 0..100.

## Generation differences

AS3000 and NEO use genuinely different conversion coefficients and offsets, consistent with the different measurement hardware/calibration paths already observed in `A120`. NEO 2005 and NEO 2013 retain the same conversion semantics after relocation.

## Caller evidence

Direct firmware callers found in the canonical corpus:

- AS3000 2005: 6
- NEO 2005: 5
- NEO 2013: 5

Callers consume the returned value; unlike `A120`, the `D0` result is therefore contractual.

## Confidence

- **CONFIRMED:** `uint32_t(void)` ABI; zero return when no measurement exists; calibrated linear conversion of `A120` state; caller consumption; 3500/3700 battery thresholds; millivolt representation; caller counts 6/5/5; real AS3000/NEO calibration differences.
- **STRONG INFERENCE:** the factory conversion coefficients encode the physical calibration of the platform-specific analog measurement circuit.
- **UNKNOWN:** original vendor routine name and factory tolerance/derivation of the calibration coefficients.

Static regression: **42/42 PASS**. Dynamic calibrated virtual-battery regression is specified but not executed.

Raw firmware, exact caller offsets and private disassembly remain outside the public repository.
