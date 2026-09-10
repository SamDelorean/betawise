# OS3K updater: OS segment preparation (0x18 / 0x16 / 0x17)

## Status

`PARCIAL_CERRADO / NO_RESUELTO` at the hardware-specific geometry level. The host-side contract, sequencing, the Small ROM `0x17` zero-length behavior, and the CFI-sector coverage rule are reproducible. Exact physical sector sizes and boundaries remain device-dependent until the actual flash geometry is identified or captured. These selectors belong to the Manager/Small ROM updater protocol and are **not** promoted into the A-line ABI.

## Source-first contract

Primary reproducible correlation source: `ioma8/neo-re` commit `732814871fe493e80ed1ab1f959d8dad174ee540`, `alpha-core/src/neo_client.rs` and `alpha-core/src/protocol.rs`, plus the later `real-check` client path used to exercise the updater transaction.

For a NEO OS image, the host implementation performs this sequence before entering the common `0x02 -> raw -> 0x0B -> 0x07` programming engine:

1. `0x18` with argument/trailing fields zero; require status `0x56` (`enter Small ROM`).
2. `0x16` with argument/trailing fields zero; require status `0x54` (`clear OS segment map` in neo-re analyst nomenclature).
3. Parse the OS-image segment table as big-endian `(address:u32, length:u32)` pairs.
4. For each segment, compute `erase_kb = ceil(length / 1024)` and send `0x17(address, erase_kb)`; require status `0x55`.
5. After all segment setup/erase requests succeed, transfer the complete OS package through the same common block engine used elsewhere by the updater and finalize with `0x07 -> 0x48`.

The exact function names used by neo-re are analyst nomenclature, not demonstrated AlphaSmart vendor symbols.

## Argument contract

| Selector | Host argument | Host trailing field | Expected status | Demonstrated role |
|---|---:|---:|---:|---|
| `0x18` | `0` | `0` | `0x56` | transition from main updater handling into Small ROM update environment |
| `0x16` | `0` | `0` | `0x54` | reset/clear the OS-segment preparation state before per-segment setup |
| `0x17` | segment destination address (`u32`) | erase length in KiB (`u16`) | `0x55` | prepare/erase one destination segment |

The `0x17` trailing field is therefore not an opaque checksum in this flow: the source-side caller computes it mechanically from the segment length in KiB, rounded upward.

## Special host policy and zero-length device behavior

neo-re exposes an optional `reformat_rest_of_rom` policy. When enabled and the segment destination is exactly `0x005FFC00`, the host sends `erase_kb = 0` rather than the rounded segment length. This is **CONFIRMADO as host behavior** at the pinned source lineage.

A corrected private two-generation Small ROM verification constrains the device side:

- command `0x17` converts its trailing KiB field to a byte length by shifting it left by 10 bits;
- it computes `end = base + (erase_kb << 10)`;
- before entering the erase body, an unconditional branch transfers control to the `current < end` comparison;
- only when that unsigned comparison succeeds does execution enter the body and call the flash worker.

Therefore `erase_kb = 0` produces `end == current == base`, the initial comparison is false, and **no flash erase helper is called**. Zero is neither an internal `erase-to-end` sentinel nor a one-sector erase request; it is a firmware-side erase no-op for command `0x17`. This behavior is **CONFIRMADO** in both compared Small ROM generations.

An earlier analysis incorrectly treated this loop as post-tested and concluded that zero caused one erase attempt. That conclusion and its supporting interpretation are explicitly **retracted**.

## Physical flash worker and CFI-sector coverage

Private NEO Small ROM decode shows that command `0x17` reaches a trampoline that copies a flash worker to RAM and executes the RAM copy. The worker:

- enters Common Flash Interface query mode and validates the `QRY` signature;
- reads the device-size and erase-region geometry fields;
- walks erase-region descriptors and derives block boundaries from live device geometry;
- reads the primary command-set identifier;
- follows an AMD/Fujitsu-style unlock and sector-erase path for the accepted command-set family;
- polls the target for completion and returns status.

The worker does **not** erase in a fixed 1 KiB or 64 KiB quantum. Physical erase geometry is read through CFI.

The command-`0x17` caller/worker contract is now also mechanically constrained. The worker receives `current` as its first address argument and returns both the containing sector start and the next sector boundary through output pointers. For the CFI region containing `current`, it computes conceptually:

`sector_start = region_start + align_down(current - region_start, sector_size)`

`next_sector = sector_start + sector_size`

The caller reloads `next_sector` as the next `current` value and re-tests `current < end` before another iteration.

Consequently, command `0x17` erases the complete CFI sectors that intersect the logical half-open interval:

`[base, base + (erase_kb << 10))`

This yields the following **CONFIRMADO** boundary behavior:

- zero length erases nothing;
- a base inside a sector causes the whole containing sector to be erased, including bytes before `base`;
- an end address inside a sector causes that complete final intersecting sector to be erased;
- an end address exactly on a sector boundary does not add the following sector.

Exact byte spans remain hardware-dependent because sector sizes and region boundaries come from the flash device's CFI data.

One additional correction is important: an intermediate reverse-engineering hypothesis interpreted the worker's final `D0` as an erase-block size. That is false. The final return is status; CFI-derived block size is internal working state. That earlier interpretation is retracted.

## Firmware correlation and regression status

Private canonical-firmware work establishes the cross-ROM transition and the Small ROM segment-mapping family without publishing ROM bytes or extended disassembly. The NEO13 host package contains a segment descriptor for `0x005FFC00`, and the private Small ROM mapper verification places the secondary transfer unit there.

The corrected zero-length control flow was checked mechanically across the older and NEO Small ROM generations with `verify_smallrom_command17_pretest_zero_noop_2026-09-09.py`: **24/24 PASS**. The compared Small ROMs share the same command-`0x17` pre-test skeleton apart from helper-call relocation.

The NEO CFI-sector coverage contract was then checked with `verify_neo_smallrom_cmd17_cfi_sector_coverage_2026-09-09.py`: **21/21 PASS**. It verifies the worker's CFI-region selection, containing-sector calculation, next-sector output, caller reload of that output as `current`, and the pre-test loop relationship.

The copied-to-RAM CFI worker itself was previously checked mechanically against the NEO Small ROM, establishing the trampoline/copy envelope, CFI query and `QRY` checks, erase-region descriptor parsing, command-set tests, unlock/sector-erase sequence, completion poll, status return, and negative checks against fixed erase quanta. The earlier post-test interpretation attached to that work is superseded by the corrected control-flow regression above.

Scripts, outputs, firmware binaries and extended disassembly remain in the private evidence archive.

## Historical AS3000 comparison

Historical AS3000 updater/source listings expose a sector-oriented flash interface: the updater calls `FlashSectorErase(sector, ...)`, while the flash layer provides sector mapping/base/size services such as `FlashMapAddressToSector` and `FlashGetSectorSize`. This is useful source-first evidence for the architectural lineage, but it should not be collapsed into a literal command equivalence.

The NEO Small ROM interface evolved to accept an address plus logical length in KiB and to recover physical erase geometry dynamically through CFI. The common concept is sector-granular flash erase; the exposed updater contract differs between generations.

## Refutation checks

The following narrower interpretations are rejected by source and firmware correlation:

- `0x16` is not a per-segment erase request: it is issued once before the segment loop and carries zero fields.
- `0x17` is not a generic zero-argument mode switch: it is issued once per parsed segment and carries both destination address and an erase-size field.
- `0x17.trailing` is not merely an arbitrary host token in the normal path: it is computed as `ceil(segment.length / 0x400)` KiB.
- `erase_kb = 0` is not decoded by command `0x17` as an `erase-to-end` sentinel.
- `erase_kb = 0` does not erase one sector: the loop pre-tests `current < end` and calls no erase helper when length is zero.
- the worker does not use `erase_kb` as a direct physical block size; flash geometry is read through CFI.
- the worker's final `D0` is not the physical erase-block size.
- the `0x18/0x16/0x17` family is not evidence for new Line-A syscalls; it is an updater-protocol sequence.

## Remaining verification target

The generic firmware-side coverage rule is closed: one CFI erase sector at a time, advancing by the worker-reported next-sector boundary until the logical byte extent is covered. What remains hardware-specific is the exact sector-size/region map on each NEO board revision. That should be resolved from direct flash-chip identification or captured CFI geometry and then fed into the hardware and emulator fronts. A board report identifying an ST M29W320EB is useful corroboration, but it is not treated as a universal NEO bill of materials.

For emulation, model the updater as a logical KiB interval layered over a CFI-defined physical sector map; do not use a fixed erase quantum.

Firmware binaries, full disassembly and private regression artifacts intentionally remain outside the public repository.
