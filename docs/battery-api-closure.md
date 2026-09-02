# System 3 / OS3K battery display API — A138 closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
void ShowBatteryPercentage(uint8_t time_seconds);
```

`ShowBatteryPercentage` is the direct A-line service at index 78 (`A138`). BetaWise does not add a separate C wrapper.

## Source-first correlation

The modern BetaWise header supplies the prototype and the historical syscall table labels A138 as the battery-graphic service. Independent early AlphaSmart material preserves the symbol `PowerShowBatteryPercentage`, establishing nominal/functional genealogy.

The historical function is **not ABI-identical** to modern A138: its argument was used as a display row, whereas all three canonical 2005/2013 System 3 handlers consume the modern argument as an 8-bit time in seconds.

An official ControlPanel caller supplies `0`, independently confirming the interactive Battery Status mode. Historical `BatteryLife/BatteryTester.c` also anchors the battery-acquisition lineage used by the lower-level A120 reconstruction.

## Public modes

### `time_seconds == 0`

A138 builds the complete Battery Status presentation and ultimately invokes `WaitForKey` before returning. Zero is therefore a distinct interactive mode, not a degenerate timer value.

### `time_seconds > 0`

A138 converts the byte argument to centiseconds:

```text
sleep_time = time_seconds * 100
```

and calls `SleepCentiseconds` (`A0D4`) before returning. This directly confirms the modern `time_seconds` parameter meaning.

## Battery-data pipeline

The sequential source-first reconstruction now closes the helpers that earlier versions of this note intentionally left unnamed:

1. `A120 / SYS_A120` refreshes the calibrated battery measurement.
2. `A124 / SYS_A124` selects the NiMH/Alkaline battery profile.
3. `A128 / SYS_A128` maps the measurement through the selected private curve into a 0..100 curve percentage/coordinate.
4. `A138` explicitly computes `100 - A128()` for the user-visible remaining-capacity presentation.

That fourth step is important: `A128` must not be described unqualified as the remaining-capacity percentage. Its raw result is the complementary curve coordinate (usefully described as a depletion/used-capacity percentage in the A138 UI path), while A138 presents the complement as remaining capacity.

A130 is also now closed independently as the board-revision getter; A138 uses it to select/support the appropriate presentation path.

## Cross-ROM evidence

| Firmware | Handler | Length |
| --- | ---: | ---: |
| AS3000 System 3, Nov 2005 | `0x004D1A32` | `0x1EA` |
| NEO System 3, Nov 2005 | `0x005D5A60` | `0x1BA` |
| NEO/System 3.15, Jul 2013 | `0x00426EC0` | `0x1BA` |

All three read the low byte of the first ABI slot, consult A130, execute the A120→A128 battery pipeline on the supported path, distinguish zero/nonzero time, and use `time_seconds * 100` for timed sleep.

AS3000 has a longer internal graphics construction than the NEO implementations. This is a real implementation difference but does not alter the portable ABI or public mode semantics.

## Caller evidence

Each canonical ROM contains two direct `JSR` callers of the A138 handler. The public contract is `void`; no stable return value is consumed as part of this API.

The official ControlPanel caller that pushes `0` selects the full interactive status mode.

## Side effects

A138 is a UI operation, not a numeric getter. It refreshes battery state, performs display composition, and either sleeps internally or consumes a key through `WaitForKey`.

Application code that only needs battery data should not substitute A138 for the lower-level battery-state functions.

## Regression status

Static contractual regression: **EXECUTED — 60/60 PASS**.

It verifies canonical hashes, index-78 vector resolution, handler boundaries, byte argument access, A130 gating, both `*100 -> A0D4` timed paths, A120/A128 call ordering, zero/nonzero mode split, `WaitForKey`, presentation helpers, direct-caller counts, and the three ROM epilogues.

Dynamic visual/keyboard regression: **SPECIFIED / NOT EXECUTED**. Minimum future cases are `0` for full status + key exit, and `1`/`2` for 100/200-centisecond timed paths on AS3000 and NEO.

## Confidence

- **CONFIRMED:** identity/index, `void(uint8_t)` ABI, zero interactive mode, nonzero seconds mode, A0D4 conversion, A120→A128 pipeline, `100 - A128()` remaining-capacity orientation, A130 gating, callers 2/2/2, and generation-specific graphics differences.
- **UNKNOWN:** private names of several internal graphics-composition helpers; these do not affect the public ABI.

Raw firmware, private disassembly, exact corpus offsets, and proprietary resources remain outside the public repository.
