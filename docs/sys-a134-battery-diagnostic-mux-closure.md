# A134 / index77 — SYS_A134 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Raw recovered contract

```c
uint32_t SYS_A134(uint32_t selector);
```

No vendor semantic name was recovered, so the neutral `SYS_A134` name is retained.

The first ABI slot is consumed as a full 32-bit selector. Firmware accepts only selectors `0..10`, `41`, and `42`; unsupported values return `0`.

## Confirmed battery-state selectors

The canonical AS3000 2005, NEO 2005, and NEO 2013 implementations expose the same logical state layout after relocation:

- selector `1`: longword calibration field consumed by `SYS_A120`;
- selector `2`: second longword calibration field consumed by `SYS_A120`;
- selector `3`: persistent calibrated battery measurement written by `SYS_A120`;
- selector `0` and selectors `4..10`: zero-extended word fields in the same battery-state block. Their original vendor names and units remain unknown.

This correlation is independently anchored by the historical `BatteryLife/BatteryTester.c` acquisition model already used to close `SYS_A120`.

## Selector 42

Selector `42` is not a getter. Firmware performs sixteen `SYS_A120` acquisitions, averages the samples, checks each sample against the average with a threshold of 20, and returns `1` on the unstable/error path. On the stable path it updates the field exposed by selector `1`, invokes a persistence/storage helper, and returns `0`.

This establishes `A134` as a battery diagnostic/calibration and state-access multiplexer rather than a scalar getter.

Because this path has calibration/persistence side effects, it should not be invoked experimentally on real hardware without a controlled test plan.

## Selector 41

Selector `41` follows a separate special path that compares 18 bytes against fixed memory and can return the fixed pointer `0x004003F0`. Its exact identity/signature purpose is not yet established and is intentionally left unnamed.

## Caller evidence

No direct absolute `JSR` caller of the A134 handler was found in any of the three canonical System 3 ROMs. A scan of the 2005 ControlPanel SmartApplet finds the A134 opcode in its contiguous A-line stub table, not as a demonstrated semantic caller. Source searches in BetaWise and `ioma8/neo-re` likewise yield only the syscall declaration/stub.

## Generation comparison

All three canonical handlers are exactly `0x1B2` bytes and retain the same control structure, accepted selectors, field layout, selector-42 sampling algorithm, and public behavior. Differences are relocations of globals and helper targets.

## Regression status

Static contractual regression: **EXECUTED — 84/84 PASS**.

It verifies canonical ROM hashes, vector index 77, handler boundaries, full-slot selector access, selector whitelist, per-generation state-field targets, the selector-3 correlation with `SYS_A120`, selector-42's exact branch to `SYS_A120`, sixteen-sample loop, threshold/status behavior, default return, and the negative direct-caller search.

Dynamic regression: **SPECIFIED / NOT EXECUTED**.

## Confidence

- **CONFIRMED:** index/identity, raw signature, accepted selector set, scalar state-field mapping, relation to `SYS_A120`, selector-42 sampling/calibration behavior and status, three-generation equivalence, negative direct-caller search.
- **STRONG INFERENCE:** overall role as a battery diagnostic/calibration/state multiplexer.
- **PROVISIONAL:** precise purpose of selector `41`, semantic names/units for selectors `0` and `4..10`, and the original semantic label for selector `42`.
- **UNKNOWN:** original vendor function name and selector typedef/enumeration names.

Raw firmware, private disassembly, exact corpus offsets, and proprietary binaries remain outside the public repository.
