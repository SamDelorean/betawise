# A174 / index93 — SYS_A174 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
uint8_t SYS_A174(void);
```

`SYS_A174` is the direct A-line syscall at index 93 (`A174`). No vendor semantic name has been recovered with sufficient evidence, so the neutral trap-derived name is retained.

## Confirmed behavior

Across the canonical AS3000 2005, NEO 2005 and NEO 2013 System 3 firmware, the public handler is the same semantic two-instruction getter after normalizing a relocated global address:

1. load one byte from an OS-owned global into `D0`;
2. return.

Only `D0.B` is contractual. The routine takes no arguments, performs no validation or transformation, calls no helper and has no observable side effect beyond reading the byte.

## Paired state operations

The adjacent syscall `SYS_A178` (index94) writes the low byte of its first ABI slot directly to the same state byte and returns. This independently confirms that A174 is the getter half of a raw byte-state getter/setter pair.

`SYS_A17C` (index95) is also a byte getter, but it reads a distinct adjacent state byte. The two states must not be conflated merely because firmware often uses them together.

## Caller evidence

The private firmware audit found direct absolute calls to A174 in all three generations. Callers consume or preserve the returned byte, and several paths save the A174 and A17C values together before temporarily modifying the paired states and later restoring them.

That pattern proves that the returned byte is meaningful OS state, but it does not uniquely recover the original vendor property name or its semantic value domain.

## Refutation and naming discipline

A174 is not the keyboard-layout state already reconstructed through the A168/A170 layout subsystem. It is also not an alias for A17C, because the handlers reference different bytes.

Historical/system-settings material contains several plausible stateful features, but no available correlation uniquely maps one of those feature names to A174. Accordingly, no speculative public name such as an idle, transfer-speed or mode getter is introduced.

## Generation comparison

No semantic generation difference was found in A174 itself. AS3000 2005, NEO 2005 and NEO 2013 differ only in relocation of the OS-owned state byte.

## Regression status

Static regression: **EXECUTED — 40/40 PASS**.

It verifies the three canonical firmware hashes, vector index93 resolution, exact two-instruction handler grammar, generation-specific state relocation, pairing with A178, separation from A17C, direct caller counts, total entry-point references and normalized three-generation equivalence.

Dynamic regression: **SPECIFIED / NOT EXECUTED**. A future probe should observe the state across official paths that save, temporarily modify and restore this byte, without assigning semantic labels until the corresponding user-visible setting is independently identified.

## Confidence

- **CONFIRMED:** index/trap identity, no-argument ABI, low-byte return, raw getter behavior, OS-owned state target, A178 pairing, separation from A17C, three-generation equivalence.
- **STRONG INFERENCE:** A174/A178 expose one member of a pair of related global System 3 state properties.
- **UNKNOWN:** vendor name, user-visible meaning and valid semantic value domain.

Raw firmware, private disassembly, addresses, binary corpus and detailed xrefs remain outside the public repository.
