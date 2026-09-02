# A180 / index96 — SYS_A180 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
void SYS_A180(uint8_t value);
```

`SYS_A180` is the direct A-line syscall at index 96 (`A180`). No vendor semantic name has been recovered, so the neutral syscall name is retained.

## Confirmed behavior

The canonical AS3000 2005, NEO 2005 and NEO 2013 handlers implement the same public behavior after relocation normalization:

1. read the byte state returned by `SYS_A17C`;
2. compare it with the low byte of the first 32-bit ABI argument slot;
3. if unchanged, return immediately;
4. if changed, store the new byte literally;
5. call a private state-change helper;
6. return.

The raw syscall performs no range validation, clamp or boolean normalization, and does not establish a stable public return value.

## Relationship to A17C

`SYS_A17C()` is the read-only getter for the same byte that `SYS_A180(value)` modifies. This makes A17C/A180 a getter/change-aware-setter pair.

A174/A178 operate on a separate adjacent byte. Firmware callers often save both A174 and A17C state, temporarily modify the corresponding A178/A180 pair, and later restore the saved values. The two properties are therefore related operationally but remain distinct state variables.

## Conditional side effect

The private helper is invoked only after an actual state change. Its three-generation structure establishes that:

- state values other than exactly `1` enter a private disable/cancel path;
- state value `1` is combined with another private gate byte;
- gate zero also selects disable/cancel;
- gate nonzero selects a private scheduling/arming path.

This supports a boolean-like normal usage model, but the raw ABI accepts every byte. The vendor meaning of the property and the high-level purpose of the private scheduling path remain **unknown** and are deliberately not named.

## Callers and generations

Direct absolute firmware callers found during the private audit: 12 in AS3000 2005, 14 in NEO 2005 and 16 in NEO 2013. Call contexts include constants `0` and `1` as well as restoration of previously saved state bytes.

The public A180 handler is semantically equivalent across all three canonical generations after normalizing the relocated state address and helper branch displacement.

## Regression status

Static regression: **EXECUTED — 64/64 PASS**.

It verifies canonical ROM hashes, index96 resolution, exact public handler length and grammar, A17C state linkage, ABI byte selection, no-op-on-equal behavior, write-and-helper-on-change behavior, caller counts, the helper's exact-1 and private-gate branches, and normalized three-generation equivalence.

Dynamic regression: **SPECIFIED / NOT EXECUTED**. A future probe should save A17C, apply `A180(0)`, repeat `A180(0)`, apply `A180(1)`, then restore the saved state, verifying that the repeated unchanged value produces no secondary state-change action.

## Confidence

- **CONFIRMED:** index/identity, one-byte argument ABI, same-state no-op, changed-state write, conditional helper invocation, pairing with A17C, three-generation equivalence.
- **STRONG INFERENCE:** normal callers treat the state as boolean-like.
- **UNKNOWN:** original vendor name and high-level semantic purpose of the private schedule/disable machinery.

Raw firmware, detailed private targets, disassembly and ROM offsets remain outside the public repository.
