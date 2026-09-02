# A188 / index98 — SYS_A188 — source-first closure

Status: **MECHANICALLY CLOSED / SEMANTICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
int32_t SYS_A188(uint8_t minutes);
```

`SYS_A188` sets the system **Idle Time** in minutes. The original vendor function name has not been recovered, so the neutral syscall name is retained.

## Confirmed behavior

The canonical AS3000 2005, NEO 2005 and NEO 2013 handlers are semantically identical:

- consume the low byte of the first 32-bit ABI argument slot;
- values below 4 return `-1` without modifying state;
- values above 59 return `-1` without modifying state;
- values from 4 through 59 inclusive are stored in the byte returned by `SYS_A184()` and return `0`.

There is no helper call or secondary transformation in the public setter.

## Source-first semantic correlation

The official Neo User's Guide defines **Idle Time** as the time a Neo can remain idle before turning off and specifies exactly **4–59 minutes** as its setting range. Firmware independently implements that exact range and stores it in the A184 state byte. The match identifies the unit and state semantics directly.

## Generation comparison and callers

After normalizing the single relocated state address, the complete A188 handler is equivalent across all three canonical generations.

No direct absolute firmware JSR callers were found (0/0/0). This does not imply the syscall is unused: it is exposed through the A-line ABI and corresponds to a documented system setting that may be driven by configuration UI or external SmartApplet/manager paths.

## Regression status

Static regression: **EXECUTED — 42/42 PASS**.

It verifies canonical ROM hashes, index98 resolution, exact handler length and grammar, ABI byte selection, lower and upper bounds, `-1` invalid return, no invalid write, valid store to the A184 byte, success `0`, caller counts and normalized three-generation equivalence.

Dynamic regression: **SPECIFIED / NOT EXECUTED**. A minimal future matrix is `3 -> -1`, `4 -> 0`, `59 -> 0`, `60 -> -1`, checking through A184 that only valid writes change the setting.

## Confidence

- **CONFIRMED:** ABI, 4–59 validation, return values, state target, unit in minutes, Idle Time semantics and three-generation equivalence.
- **UNKNOWN:** original vendor symbol/function name.

Raw firmware, detailed offsets and private disassembly remain outside the public repository.
