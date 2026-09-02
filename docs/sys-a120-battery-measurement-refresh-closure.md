# A120 / index72 — SYS_A120 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
void SYS_A120(void);
```

No reliable vendor function name has been recovered, so the neutral `SYS_A120` name is retained.

## Confirmed behavior

`A120` refreshes the OS battery-measurement state. It takes no public arguments and has no stable public return value.

When the measurement subsystem is unavailable/disabled, the firmware exits without producing a defined result. On the active path it temporarily configures platform-specific measurement hardware, performs a timed acquisition through a private helper, applies the generation-specific calibration/normalization path, stores the resulting measurement in persistent OS state, restores the saved control state, and returns.

The result stored by `A120` is subsequently consumed by other battery routines. In particular, one downstream family converts it to a 0..100 battery-capacity value, while another produces the converted representation used by system callers.

## Source-first correlation

Historical AlphaSmart `BatteryTester.c` code provides an independent ancestor for this architecture. Its `BTester_GetNewVoltage(void)` routine discharges the measurement capacitor, waits on the low-battery comparator transition, counts the timing interval, and periodically records that count as a battery-voltage measurement. Modern System 3 firmware expands this into a calibrated, platform-specific acquisition path but retains the same basic measurement model.

## ABI evidence

Across the canonical AS3000 2005, NEO 2005 and NEO 2013 firmware:

- the public handler performs no stack-argument reads;
- direct firmware callers number 7, 6 and 6 respectively;
- none of those callers consumes an `A120` return value;
- callers instead invoke a downstream conversion/percentage function when a value is needed.

Therefore an incidental register value left on one active path is not part of the public ABI; `void` is the supported contract.

## Generation differences

The semantic pipeline is shared, but the hardware implementation is not byte-identical. AS3000 and NEO use different GPIO/control bits, and their normalization paths use different calibration constants. NEO also manipulates an additional adjacent hardware-control byte. These differences are preserved rather than normalized away.

## Refuted interpretations

- `A120` is not a Dialog operation merely because of its numerical neighborhood.
- `A120` does not directly return battery percentage; the percentage routine consumes state refreshed by `A120`.
- `A120` does not expose a stable voltage value through `D0`; the inactive path does not define it and callers ignore it.

## Confidence

- **CONFIRMED:** index/trap identity, `void(void)` ABI, battery-measurement refresh role, acquisition side effects, persistent measurement state, downstream battery consumers, caller behavior, and real AS3000/NEO hardware differences.
- **STRONG INFERENCE:** the stored longword is the calibrated battery level/voltage measurement used as input to percentage and display/conversion layers.
- **UNKNOWN:** original vendor routine name and exact physical engineering unit of the calibrated internal longword.

Static regression: **61/61 PASS**. Dynamic battery-input regression is specified but not executed.

Raw firmware, private disassembly, exact caller offsets and hardware reconstruction remain outside the public repository.
