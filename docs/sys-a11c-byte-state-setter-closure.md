# A11C / index71 — SYS_A11C — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

No original AlphaSmart vendor name has been recovered. BetaWise upstream and `ioma8/neo-re` both retain `SYS_A11C`, so this closure keeps the neutral symbol.

## Recovered contract

```c
int32_t SYS_A11C(uint8_t value);
```

The low byte of the first ABI argument slot is interpreted as an enum-like state value.

For AS3000 2005 and NEO 2005:

- values `1..4`: store the byte into the current state and return `0`;
- values outside `1..4`: leave the state unchanged and return `-1`.

`SYS_A118` reads the same current-state byte, making A11C/A118 a setter/getter pair.

## NEO 2013 extension

NEO 2013 preserves the normal `1..4` contract but adds one real generation-specific command path:

- `value == 0xDE`: set both the related/default state and the current state to `4`, then return `0`.

The value `0xDE` is therefore a command selector, **not** a fifth stored enum value.

Other out-of-range values continue to return `-1`.

## State correlation

A neighboring private getter exposes the related/default state byte. The single direct internal A11C caller found in each canonical ROM reads that related/default byte, promotes it, and passes it to A11C. This establishes a firmware pattern that restores or synchronizes the current state from its related/default state.

The exact semantic name of the enum remains unknown. This documentation intentionally does not label it as font, style, layout, mode, or another specific concept without source evidence.

## Generation comparison

- AS3000 2005 and NEO 2005: same 0x22-byte handler semantics after relocation normalization.
- NEO 2013: 0x3C-byte handler due to the additional `0xDE` command path.

Thus the ordinary `1..4` API remains compatible, while the private command extension is a genuine generation difference that must be preserved by accurate emulation.

## Regression status

Static regression: **EXECUTED — 37/37 PASS**.

It verifies canonical ROM hashes, vector index71 resolution, low-byte ABI argument access, explicit lower/upper range checks, return values, current-state targets, direct caller/reference counts, the reset/synchronization caller pattern, AS3000/NEO 2005 equivalence, and the NEO 2013 `0xDE` extension with both state writes.

Dynamic/emulator regression: **SPECIFIED / NOT EXECUTED**. Values `1..4` and ordinary invalid values can be characterized in emulation. The private `0xDE` command should remain emulator-only until its semantic purpose is recovered.

## Confidence

- **CONFIRMED:** index71/A11C identity, byte argument, normal range `1..4`, `0/-1` result contract, A118 setter/getter relationship, internal reset-from-related-state pattern, and NEO 2013 `0xDE` behavior.
- **STRONG INFERENCE:** the neighboring byte is a related/default value used to restore current state.
- **UNKNOWN:** original vendor name, meaning of enum values `1..4`, and semantic purpose of the private `0xDE` command.

Raw firmware, complete disassembly, offsets and private audit material remain outside the public repository.