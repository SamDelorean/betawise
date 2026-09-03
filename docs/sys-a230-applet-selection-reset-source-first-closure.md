# SYS_A230 — selected SmartApplet context reset (source-first closure)

## Status

A230/index140 is closed at confidence A under the restarted source-first audit.
No reliable original vendor symbol was recovered, so `SYS_A230` is retained.

```c
int32_t SYS_A230(void);
```

The existing declaration in `os3k/applet_selection.h` already matches the
revalidated machine ABI and requires no change.

## Source correlation

The older A22C–A234 reconstruction described A230 as the reset operation for a
selected SmartApplet runtime context. That material was treated as a hypothesis,
not primary proof. The immediately preceding source-first closure of A22C
independently established the same generation-specific 32-bit selection global.

## Firmware verification

The three canonical ROM identities were rechecked before extraction. A230 is a
`0x0a`-byte handler in every ROM:

- AS3000 2005 entry `0x004E0DC8`, selected-context global `0x00003560`;
- NEO 2005 entry `0x005E3724`, global `0x000035B8`;
- NEO 2013 entry `0x00439B26`, global `0x000035F0`.

A234 begins exactly ten bytes later in all three images. The complete operation
is structurally identical apart from the relocated global:

```text
D0 = -1
selected_context = D0
return D0
```

Thus A230 consumes no arguments, calls no helpers, has no branches or
preconditions, stores sentinel `0xffffffff`, and deterministically returns
`-1` in the full 32-bit D0 register.

Handler SHA-256 values:

- AS3000 2005: `7edba3953a19e0e7544f0b0656fabc29f36ce47f2fd7b85b30314800f399ea78`
- NEO 2005: `ac6cd25e300c03d4578985c8edb2187f133e23115fec410c2d186636bf893ad4`
- NEO 2013: `c6d96c458a44672f9e03bd5f8948421786916a7303fcf111da586fb3fd524438`

A renewed full-ROM absolute-JSR search found `0 / 0 / 0` direct callers. That
negative result is retained explicitly; the functional relationship is instead
anchored by A22C and A234 using the same global.

## Refutation and ABI boundary

A230 is not a clipboard reset and does not touch any clipboard descriptor. It is
also not a getter: it writes the shared SmartApplet selection-context global.
The reset value is exactly `0xffffffff`, not zero.

Although an original source declaration might conceivably have ignored the
machine result, the handler explicitly initializes D0 to `-1`, writes that value,
and returns with D0 unchanged. The reconstructed ABI therefore preserves
`int32_t` rather than silently weakening it to `void`.

## Confidence

**CONFIRMED:** no-argument ABI, full 32-bit return `-1`, sentinel write to the
same global used by A22C, exact `0x0a` handler across all three ROMs, and direct
JSR count `0/0/0`.

**STRONG INFERENCE:** the neutral semantic label “selected SmartApplet context”,
based on the independently correlated A22C/A234 family.

**UNKNOWN:** original vendor symbol and whether a historical source prototype
chose to expose or ignore the deterministic D0 result.

## Regression

Static source-first regression: **EXECUTED — 27/27 PASS** (nine checks per ROM),
covering canonical ROM identity, handler length/boundary, exact `MOVEQ #-1`,
global store, `RTS`, shared-global identity, full-ROM direct-JSR count and
handler fingerprint.

Dynamic emulator/hardware regression: **SPECIFIED / NOT EXECUTED**. A minimal
dynamic test should first establish a valid selection with A22C, then call A230
and verify both the global sentinel and D0, followed by A234's sentinel path.

The next sequential source-first target is A234/index141. Higher A500+ work
remains preserved and paused.
