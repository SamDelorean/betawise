# A118 / index70 — SYS_A118 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

No original AlphaSmart vendor name has been recovered. BetaWise upstream and `ioma8/neo-re` both retain `SYS_A118`, so this closure keeps the neutral symbol.

## Recovered contract

```c
uint8_t SYS_A118(void);
```

The handler has no arguments and performs only two operations:

1. load one System 3 state byte into `D0.B`;
2. return.

Only the low byte of `D0` is established by firmware. The upper bits are not part of the reconstructed contract.

The routine is read-only: it performs no helper calls, branches, validation, or other state changes.

## Relationship with A11C

`SYS_A11C` writes the same state byte that `SYS_A118` reads. The normal A11C path accepts byte values 1 through 4, making A118 the getter half of that state pair.

The exact vendor meaning of this byte has not yet been recovered. It is therefore intentionally not named as a font, style, mode, layout, or similar concept without source evidence.

All direct System 3 callers located during the private audit compare the returned byte with value `4`, confirming that callers consume this as a byte-valued enum/state rather than a boolean or fully defined 32-bit result.

## Generation comparison

The canonical AS3000 2005, NEO 2005 and NEO 2013 handlers are all 8 bytes long and are byte-identical after normalizing the relocated state-byte address.

Direct internal caller counts differ by generation: AS3000 has three, while NEO 2005 and NEO 2013 each have one. This is a usage difference, not a handler-contract difference.

## Regression status

Static regression: **EXECUTED — 34/34 PASS**.

It verifies canonical ROM hashes, vector index70 resolution, exact two-instruction handler grammar, state-byte location by generation, caller/reference counts, the caller-side comparison with value 4, and normalized three-generation equivalence.

Dynamic/emulator characterization: **SPECIFIED / NOT EXECUTED**. Semantic labels for values 1–4 remain deliberately open until caller context or historical source establishes them.

## Confidence

- **CONFIRMED:** index70/A118 identity, zero-argument byte getter, read-only behavior, low-byte-only return contract, relationship with the A11C setter, caller comparison against 4, and three-generation equivalence.
- **STRONG INFERENCE:** the byte is an enum-like subsystem state with normal operational values 1–4.
- **UNKNOWN:** original vendor name and semantic meaning of each value.

Raw firmware, complete disassembly, offsets and private audit material remain outside the public repository.