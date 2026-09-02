# A18C / index99 — Idle Time active-state restore

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered ABI

```c
void SYS_A18C(void);
```

The original vendor symbol has not been recovered, so the neutral `SYS_A18C` name is retained.

## Confirmed behavior

`A18C` belongs to the System 3 Idle Time / automatic power-off state cluster already correlated through `A184` and `A188`.

Its firmware handler performs one state transition only:

```text
active_idle_time = companion_idle_time_state
```

It consumes no arguments, performs no validation, calls no helper, and establishes no contractual return value.

`A184` reads the active Idle Time value and `A188` changes that active value after enforcing the documented 4..59 minute range. In contrast, the companion byte used by `A18C` is maintained separately: System 3 initialization starts both states at the same value, and a private validated configuration path synchronizes both. This lets `A188` change the operational value independently and lets `A18C` restore it from the companion state.

## Meaning of the companion state

The firmware mechanics establish that this byte is the companion/baseline state for the active Idle Time setting. The exact original vendor distinction among terms such as *default*, *saved*, *configured* or *persisted* has not been recovered, so no stronger name is assigned.

Historical AS3000 requirements independently describe an adjustable Power Off Timer and identify four minutes as the earlier baseline behavior. Modern NEO documentation and firmware establish the later public Idle Time range as 4..59 minutes. Historical design terminology is therefore used only as subsystem context, not as a substitute for the modern firmware contract.

## Generation comparison

AS3000 2005, NEO 2005 and NEO 2013 implement the same semantic two-operation handler after normalizing the relocated state addresses. The companion state is immediately adjacent to the active state in all three generations.

No direct absolute firmware JSR caller to `A18C` was found in any of the three canonical ROMs. This is consistent with an exposed System 3 service whose invocation can originate through the A-line ABI rather than an internal absolute call.

## Regression status

Static regression: **EXECUTED — 40/40 PASS**.

The private regression revalidates canonical ROM hashes, index99 resolution, exact handler grammar and length, source/destination state correlation, adjacency, the A184/A188 relationship, negative direct-JSR search, companion-state reference counts and normalized three-generation equivalence.

Dynamic regression: **SPECIFIED / NOT EXECUTED**. A future probe can establish a configured baseline through the owning configuration path, temporarily change the active value through `A188`, invoke `A18C`, and confirm through `A184` that the baseline is restored.

## Confidence

- **CONFIRMED:** `void(void)` ABI, companion-to-active copy, Idle Time subsystem identity, A184/A188 state relationship, absence of validation/helpers, three-generation structural equivalence.
- **STRONG INFERENCE:** the companion byte is the configured/baseline Idle Time state used to restore the operational value.
- **UNKNOWN:** original vendor symbol and exact persistence terminology for the companion byte.

Raw firmware, private offsets, disassembly and binary-analysis artifacts remain outside the public repository.
