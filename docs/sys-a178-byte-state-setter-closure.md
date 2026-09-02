# A178 / index94 — SYS_A178 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
void SYS_A178(uint8_t value);
```

`SYS_A178` is the direct A-line syscall at index 94 (`A178`). The original vendor semantic name has not been recovered, so the neutral trap-derived name is retained.

## Confirmed behavior

Across canonical AS3000 2005, NEO 2005 and NEO 2013 firmware, the public handler has the same semantic body after normalizing the relocated state address:

1. consume the low byte of the first 32-bit ABI argument slot;
2. store that byte directly in an OS-owned global;
3. return.

There is no comparison, range check, clamp, helper call, branch, return-value construction or secondary side effect in the handler.

## Getter/setter pairing

The destination is exactly the byte returned by `SYS_A174` (index93). Together the two syscalls form a raw byte-state getter/setter pair:

```c
uint8_t SYS_A174(void);
void SYS_A178(uint8_t value);
```

`SYS_A17C` (index95) reads a different adjacent state byte and must not be treated as an alias.

## Value-domain evidence

Firmware call sites pass at least immediate `0` and `1`, and other paths restore values previously saved from the getter. The raw setter itself accepts any 8-bit value. Therefore the ABI is deliberately expressed as `uint8_t`, not `bool`.

The observed 0/1 usage does not establish the original semantic domain or feature name.

## Refutation and naming discipline

A178 is not the keyboard-layout setter: the keyboard-layout subsystem has already been independently reconstructed through A168/A170. It is also not the setter for A17C's state because the absolute destinations differ.

System-settings material supplies several plausible feature names, but no reproducible source/caller correlation uniquely identifies this byte. No speculative public name is introduced.

## Generation comparison

No semantic difference was found among AS3000 2005, NEO 2005 and NEO 2013. Only the absolute location of the OS-owned state byte changes.

## Regression status

Static regression: **EXECUTED — 37/37 PASS**.

It verifies canonical firmware hashes, vector index94, exact handler grammar and ABI byte offset, A174 getter/setter state correlation, separation from A17C, observed zero/one caller patterns and normalized three-generation equivalence.

Dynamic regression: **SPECIFIED / NOT EXECUTED**.

## Confidence

- **CONFIRMED:** identity, one-byte argument ABI, literal state write, no validation/helper, pairing with A174 and three-generation equivalence.
- **STRONG INFERENCE:** A174/A178 expose one member of a pair of related System 3 state properties used together with A17C/A180.
- **UNKNOWN:** vendor name, user-visible meaning and semantic value domain.

Raw firmware, detailed xrefs, addresses and disassembly remain private.
