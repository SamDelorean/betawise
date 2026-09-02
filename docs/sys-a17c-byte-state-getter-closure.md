# A17C / index95 — SYS_A17C — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
uint8_t SYS_A17C(void);
```

`SYS_A17C` is the direct A-line syscall at index 95 (`A17C`). No sufficiently supported vendor semantic name has been recovered.

## Confirmed behavior

Canonical AS3000 2005, NEO 2005 and NEO 2013 firmware implement the same semantic two-instruction getter after normalizing relocation:

1. load one OS-owned state byte into `D0`;
2. return.

Only `D0.B` is contractual. There are no arguments, helper calls, validation, transformation or observable side effects.

## Relationship to adjacent state

A17C reads a byte immediately adjacent to, but distinct from, the byte exposed by the A174/A178 getter/setter pair. Proximity is therefore structural, not aliasing.

The following syscall, `SYS_A180` (index96), operates on the same byte as A17C. Unlike A178, however, A180 is not a literal setter: it compares the requested byte with current state, returns immediately if unchanged, and otherwise stores the new byte and invokes a private helper. The helper semantics remain a separate A180 investigation and are not projected backward onto A17C.

## Caller evidence

Direct absolute A17C callers exist in all three firmware generations. Several routes obtain both A17C and A174 state bytes before temporarily modifying the associated A180/A178 states and later restoring them. This establishes two related but independent System 3 properties.

## Refutation and naming discipline

- A17C is not an alias for A174; the global bytes differ in every canonical ROM.
- The getter itself is not a policy or conversion routine; it performs one byte read only.
- No candidate user-facing setting has been uniquely correlated with this state, so no speculative name is introduced.

## Generation comparison

No semantic difference was found in A17C itself across AS3000 2005, NEO 2005 and NEO 2013. Only relocation of the OS-owned byte changes.

## Regression status

Static regression: **EXECUTED — 37/37 PASS**.

It verifies canonical hashes, vector index95, exact handler grammar, separation from the A174/A178 byte, A180 correlation with the same state, write-on-change/helper structure of A180, direct caller counts and normalized three-generation equivalence.

Dynamic regression: **SPECIFIED / NOT EXECUTED**.

## Confidence

- **CONFIRMED:** identity, no-argument ABI, low-byte return, raw getter behavior, distinct state target, structural pairing with A180 and three-generation equivalence.
- **STRONG INFERENCE:** A17C/A180 expose the second of two related global System 3 state properties.
- **UNKNOWN:** vendor name, semantic value domain and meaning of A180's change hook.

Raw firmware, detailed xrefs, addresses and disassembly remain private.
