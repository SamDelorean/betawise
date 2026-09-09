# OS3K updater: OS segment preparation (0x18 / 0x16 / 0x17)

## Status

`PARCIAL_CERRADO / NO_RESUELTO` at the protocol-policy level. The host-side contract, sequencing, and the Small ROM behavior of the `0x17` zero-length case are reproducible; the exact physical flash span affected by one helper invocation and vendor-internal helper names are not claimed. These selectors belong to the Manager/Small ROM updater protocol and are **not** promoted into the A-line ABI.

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

A subsequent private two-generation Small ROM verification now constrains the device side as well:

- command `0x17` converts its trailing KiB field to a byte length by shifting it left by 10 bits;
- it computes the end address as `base + byte_length`, so `erase_kb = 0` produces `length = 0` and `end = base`;
- there is no pre-helper branch that recognizes zero as a special `erase-to-end` sentinel;
- the flash-helper loop is post-tested: the helper is invoked before the first comparison against the computed end address.

Therefore, within command `0x17`, zero is **not** an internal `erase-to-end` sentinel and it is also **not** a no-op. It causes at least one flash-helper invocation before the loop can terminate. This behavior is **CONFIRMADO** in both compared Small ROM generations.

What remains `NO_RESUELTO / EVIDENCIA_INSUFICIENTE` is the exact physical flash span affected by that single helper pass. That question belongs to the copied-to-RAM flash routine and flash geometry, not to the `0x17` loop-control field itself.

## Firmware correlation and regression status

Private canonical-firmware work establishes the cross-ROM transition and the Small ROM segment-mapping family without publishing ROM bytes or extended disassembly. The NEO13 host package contains a segment descriptor for `0x005FFC00`, and the private Small ROM mapper verification places the secondary transfer unit there.

The zero-length control-flow property was rechecked mechanically across the older and NEO Small ROM generations with `verify_smallrom_command17_zero_length_posttest_2026-09-09.py`. The executed regression reports **21/21 PASS**. The script, output, firmware binaries and extended disassembly remain in the private evidence archive.

## Refutation checks

The following narrower interpretations are rejected by source and firmware correlation:

- `0x16` is not a per-segment erase request: it is issued once before the segment loop and carries zero fields.
- `0x17` is not a generic zero-argument mode switch: it is issued once per parsed segment and carries both destination address and an erase-size field.
- `0x17.trailing` is not merely an arbitrary host token in the normal path: it is computed as `ceil(segment.length / 0x400)` KiB.
- `erase_kb = 0` is not decoded by command `0x17` as an `erase-to-end` sentinel.
- `erase_kb = 0` is not a no-op: the post-test loop invokes the flash helper at least once.
- the `0x18/0x16/0x17` family is not evidence for new Line-A syscalls; it is an updater-protocol sequence.

## Remaining verification target

The next physical-object closure target is the flash helper/routine copied to RAM and invoked by command `0x17`. The goal is to determine the erase/program quantum and thereby bound the exact physical span affected by the zero-length special case. Until that helper and the relevant flash geometry are independently correlated, no broader erase-range policy should be assigned.

Firmware binaries, full disassembly and private regression artifacts intentionally remain outside the public repository.
