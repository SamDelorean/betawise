# A124 / index73 — SYS_A124 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
uint32_t SYS_A124(void);
```

No reliable vendor symbol has been recovered, so the neutral `SYS_A124` name is retained.

## Confirmed behavior

`A124` is a pure getter for the battery chemistry/profile selector used by the OS battery model.

The normal values demonstrated by firmware callers and resource tables are:

- `0` — NiMH
- `1` — Alkaline

The canonical AS3000 2005, NEO 2005 and NEO 2013 handlers are all eight bytes long and consist semantically of a 32-bit load of the generation-specific selector state into `D0`, followed by `RTS`. There are no public arguments, helper calls, validation steps or side effects.

## Caller correlation

Each canonical ROM has two direct callers. Both test the return against 0 and 1 explicitly and retain a fallback path for other values.

One caller selects localized resource IDs `0x62` and `0x63`. In the NEO 2005 and NEO 2013 resource tables these resolve directly to `NiMH` and `Alkaline`, respectively, establishing the value mapping independently of the getter itself.

## Relationship to battery percentage

The battery-percentage routine reads the same selector state and chooses between two distinct discharge/measurement curves before converting the measurement refreshed by `A120` into a 0..100 capacity result. This establishes that `A124` exposes the profile used by the battery model rather than a generic boolean flag.

## Confidence

- **CONFIRMED:** index73/A124 identity, `uint32_t(void)` ABI, pure getter semantics, normal value mapping `0=NiMH`, `1=Alkaline`, caller behavior, and use by the battery-capacity curve selector.
- **UNKNOWN:** original vendor routine name and semantics of any value outside 0/1; callers retain a fallback but normal initialization produces the demonstrated profiles.

Static regression: **47/47 PASS**. Dynamic profile-switch regression is specified but not executed.

Raw firmware, private disassembly and caller offsets remain outside the public repository.
