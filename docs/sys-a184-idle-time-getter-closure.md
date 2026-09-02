# A184 / index97 — SYS_A184 — source-first closure

Status: **MECHANICALLY CLOSED / SEMANTICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
uint8_t SYS_A184(void);
```

`SYS_A184` returns the system **Idle Time** value in minutes. The original vendor function name has not been recovered, so the neutral syscall name is retained.

## Source-first identification

The official Neo User's Guide documents **Idle Time** as the period a Neo may remain idle before turning off and gives the allowed range as **4–59 minutes**.

The firmware independently supplies the decisive correlation: `SYS_A188` writes exactly the byte returned by A184 and accepts exactly values 4 through 59. This exact range match closes the semantic identity of the state without relying on syscall-table proximity.

## Confirmed firmware behavior

Across canonical AS3000 2005, NEO 2005 and NEO 2013 firmware, A184 is a two-instruction getter:

1. read the generation-specific Idle Time byte into `D0.B`;
2. return.

Only the low return byte is contractual. The syscall has no arguments, helper calls or side effects.

## Relationship to A180/A188

- `SYS_A188(minutes)` is the validated setter for the same byte.
- `SYS_A180(value)` controls a separate operational state used by the idle-shutdown scheduling machinery. It is not the Idle Time value itself.

When A180's operational state is active, its private change helper consults the A184 Idle Time byte before entering the scheduling path.

## Generation comparison and callers

The A184 public handler is identical in all three canonical generations after normalizing only the relocated state address. One direct absolute firmware caller was found in each generation.

## Regression status

Static regression: **EXECUTED — 39/39 PASS**.

It verifies canonical ROM hashes, index97 resolution, exact 8-byte getter grammar, generation-specific state address, pairing with A188, the 4/59 bounds in the setter and one direct caller per ROM.

Dynamic regression: **SPECIFIED / NOT EXECUTED**.

## Confidence

- **CONFIRMED:** ABI, getter semantics, state identity, units in minutes, 4–59 configured domain and three-generation equivalence.
- **UNKNOWN:** original vendor symbol/function name.

Raw firmware, detailed offsets and private disassembly remain outside the public repository.
