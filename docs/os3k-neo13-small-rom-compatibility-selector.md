# NEO 2013 Small ROM compatibility selector

Status: structural research note. No firmware image, large disassembly, or private Ghidra material is included.

## Summary

NEO 2013 contains an internal one-byte selector at RAM address `0x041A` that chooses between legacy and newer implementation paths throughout the USB/controller compatibility layer.

Primary-firmware correlation establishes the selector contract:

- `0x041A = 2` when the installed **Small ROM version is earlier than 1.4**;
- `0x041A = 1` when the installed **Small ROM version is 1.4 or later**.

The selector should therefore be described as a **Small ROM version compatibility selector**. It must not be renamed to a board-revision flag without independent evidence.

## Version source

The initializer reads two bytes at `0x00400414` and `0x00400415`.

An independent formatter in the same canonical firmware passes those same two bytes to the literal format string:

`Small ROM %d.%d `

This confirms that they are the Small ROM major and minor version components.

## Threshold calculation

The initializer mechanically computes the decimal version key:

`10 * major + minor`

and compares it with decimal `14`.

The resulting state is:

| Condition | `0x041A` |
| --- | ---: |
| `10*major + minor < 14` | `2` |
| `10*major + minor >= 14` | `1` |

Thus the actual compatibility boundary is Small ROM **1.4**.

Only one immediate write of value `1` and one immediate write of value `2` to this selector occur in the canonical NEO 2013 package.

## Wrapper family

A complete scan of the canonical NEO 2013 image finds:

- **37** comparisons of `0x041A` against `2`;
- **1** comparison of `0x041A` against `1`, used as a boolean-style accessor;
- **8** canonical simple old/new wrapper pairs of the form `legacy when ==2`, `new when !=2`;
- additional gated-new, gated-legacy, and argument-adaptation wrappers.

The compatibility layer includes pairs leading into the newer ISP1763 implementation. Examples include:

- legacy `0x00441494` ↔ new `0x0045217E` (chip-ID stability path);
- legacy `0x00441842` ↔ new `0x004525BE` (PTD descriptor normalizer path);
- legacy `0x004416F8` ↔ new `0x004526E2` (ATL PTD prepare/submit path);
- legacy `0x0041EB8E` ↔ new `0x0045239A` (HC ATL/INT interrupt-service path).

The last two pairs are reached through wrappers that preserve a single transfer-structure pointer across both implementations.

## Interpretation

This explains why the NEO 2013 system image contains two substantial generations of low-level controller code in parallel. The runtime does not select them by A-line syscall number; it selects them through an internal compatibility layer keyed to the Small ROM version.

For the observed wrappers, `0x041A == 2` is the legacy path and `0x041A != 2` is the newer path. The newer path includes code positively identified as ISP1763 through an explicit `DC_CHIPID == 0x00176320` test and matching register layout.

This does **not** by itself prove what hardware physically differs between Small ROM revisions. It proves only the firmware selection criterion and the implementation paths selected by that criterion.

## Reproducibility

A private canonical-image regression verifies:

- canonical NEO 2013 SHA-256;
- both Small ROM version-byte reads;
- the `10*major+minor` calculation;
- the threshold `14`;
- unique immediate writes of selector values `1` and `2`;
- 40 total literal references to `0x041A`;
- 37 `==2` gates;
- 8 simple old/new pairs;
- the unique `==1` accessor.

Result: **16/16 PASS**.

## Classification

- Small ROM major/minor source: **CONFIRMED**.
- Threshold 1.4: **CONFIRMED**.
- Selector values `{1,2}` and their conditions: **CONFIRMED**.
- Use as legacy/new compatibility selector: **CONFIRMED**.
- Exact physical hardware distinction represented by the Small ROM threshold: **UNRESOLVED / INSUFFICIENT EVIDENCE**.
- ABI status: **internal compatibility mechanism; not an A-line syscall**.
