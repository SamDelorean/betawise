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

## Complete simple-pair inventory

The eight mechanically simple wrappers contain no argument adaptation between the version gate and the selected direct call. At the wrapper boundary, the legacy and newer callees therefore occupy the same caller-visible operation slot. This is a **contract-equivalence anchor**, not permission to copy an internal vendor name or every implementation detail from one callee to the other.

| wrapper runtime | Small ROM <1.4 | Small ROM >=1.4 | classification |
| --- | --- | --- | --- |
| `0x0041F864` | `0x00441550` | `0x00451344` | simple legacy/new pair |
| `0x0041F890` | `0x0043FE3E` | `0x004515C6` | simple legacy/new pair; newer side in controller setup/init path |
| `0x0041F8AA` | `0x0043FF18` | `0x00451602` | simple legacy/new pair; newer side delegates to controller initialization |
| `0x0041FB5E` | `0x0043FEAC` | `0x0045183C` | simple legacy/new pair; newer side performs role/OTG setup and initialization |
| `0x0041FC4E` | `0x0043FEF4` | `0x0045192A` | simple legacy/new pair; newer side wraps controller initialization in an interrupt critical section |
| `0x0041FD26` | `0x004416A4` | `0x00452176` | simple legacy/new pair |
| `0x0041FD40` | `0x00441494` | `0x0045217E` | simple legacy/new pair; controller identity/stability gate |
| `0x0041FF82` | `0x0041EB8E` | `0x0045239A` | simple legacy/new pair; newer side is HC ATL/INT interrupt-service path |

This table makes the compatibility layer useful as a reverse-engineering bridge: once one side of a simple pair is independently reconstructed, the other side inherits the same **wrapper-level role and caller-side contract hypothesis**, which must then be checked against its own body before any stronger semantic claim is promoted.

## Per-pair semantic refinement

Several newer-side callees are now independently correlated strongly enough to narrow the caller-visible role of their legacy partners without transferring vendor-specific internals.

The newer targets `0x004515C6`, `0x00451602`, `0x0045183C`, and `0x0045192A` all converge on the newer Device Controller setup/initialization path. The first combines global/GPIO setup with initialization in an interrupt-critical section; the second delegates directly to initialization; the third performs controller role/OTG setup before the same initialization; and the fourth wraps initialization while preserving interrupt state. Consequently, the paired legacy targets `0x0043FE3E`, `0x0043FF18`, `0x0043FEAC`, and `0x0043FEF4` are strong hypotheses for the corresponding **caller-visible controller lifecycle operations**. Their controller identity, register map, electrical sequence, and internal implementation remain unresolved.

Likewise, new `0x0045239A` is independently reconstructed as the newer HC ATL/INT interrupt-service path. Therefore legacy `0x0041EB8E` is a strong hypothesis for the same caller-visible interrupt-service role. This does not establish that the legacy controller is an ISP1763, and the ISP1763 name must not be propagated to that callee.

New `0x0045217E` is a stronger anchor. It repeatedly reads the newer controller identification register, requires the expected identification value to be observed on five consecutive reads, resets the stability count after a mismatch, and delays before retrying. The paired legacy `0x00441494` can therefore be classified as the legacy **controller presence/identity/stability gate** at the caller boundary. Its expected identifier and hardware-specific implementation remain unresolved.

### Emulation consequence

The newer identity/stability gate can wait indefinitely if controller MMIO never produces the expected stable identification value. For NEO emulation this is a concrete diagnostic rule: an early firmware hang at this gate can indicate incomplete controller/MMIO emulation rather than a CPU-core or ROM-layout error. The same type of precondition is a strong hypothesis for the legacy branch, but its hardware-specific identifier must not be invented.

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

Separate previously executed canonical-image regressions cover the newer controller helper/initialization cluster (**79/79 PASS**) and the newer HC ATL/INT service (**36/36 PASS** in its focused regression). The per-pair refinements above are derived correlations over those already executed checks; no additional binary regression is claimed for this documentation update.

## Classification

- Small ROM major/minor source: **CONFIRMED**.
- Threshold 1.4: **CONFIRMED**.
- Selector values `{1,2}` and their conditions: **CONFIRMED**.
- Use as legacy/new compatibility selector: **CONFIRMED**.
- Eight simple wrappers as direct legacy/new contract-equivalence anchors: **CONFIRMED mechanically**.
- Legacy controller-lifecycle and interrupt-service roles derived from independently reconstructed newer partners: **STRONG INFERENCE at caller-visible contract level**.
- Legacy `0x00441494` as controller presence/identity/stability gate: **STRONG INFERENCE at caller-visible contract level**.
- Transfer of exact internal semantics, identifiers, register maps, or vendor names across a pair: **UNRESOLVED / INSUFFICIENT EVIDENCE**.
- Exact physical hardware distinction represented by the Small ROM threshold: **UNRESOLVED / INSUFFICIENT EVIDENCE**.
- ABI status: **internal compatibility mechanism; not an A-line syscall**.
