# OS3K updater: OS segment preparation (0x18 / 0x16 / 0x17)

## Status

`PARCIAL_CERRADO / NO_RESUELTO` at the protocol-policy level. The host-side contract and sequencing are reproducible; exact vendor-internal helper names and every flash-policy edge case are not claimed. These selectors belong to the Manager/Small ROM updater protocol and are **not** promoted into the A-line ABI.

## Source-first contract

Primary reproducible correlation source: `ioma8/neo-re` commit `732814871fe493e80ed1ab1f959d8dad174ee540`, `alpha-core/src/neo_client.rs` and `alpha-core/src/protocol.rs`.

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

## Special host policy

neo-re exposes an optional `reformat_rest_of_rom` policy. When enabled and the segment destination is exactly `0x005FFC00`, the host sends `erase_kb = 0` rather than the rounded segment length. This is **CONFIRMADO as host behavior** at the pinned commit. The precise Small ROM interpretation of zero for this case remains `NO_RESUELTO / EVIDENCIA_INSUFICIENTE` here until independently demonstrated from the handler; it must not be paraphrased as a vendor-defined "erase to end" contract without that verification.

## Firmware correlation already established privately

Existing private canonical-firmware work establishes the cross-ROM transition and the Small ROM segment-mapping family without publishing ROM bytes or extended disassembly. In particular, the NEO13 host package contains a segment descriptor for `0x005FFC00`, and the private Small ROM mapper verification places the secondary transfer unit there. The previously archived two-generation `0x16/0x17/0x18` regression remains the binary evidence surface for this protocol family.

This document adds a source-first semantic constraint to that evidence rather than claiming a new binary execution run.

## Refutation checks

The following narrower interpretations are rejected by the source correlation:

- `0x16` is not a per-segment erase request: it is issued once before the segment loop and carries zero fields.
- `0x17` is not a generic zero-argument mode switch: it is issued once per parsed segment and carries both destination address and an erase-size field.
- `0x17.trailing` is not merely an arbitrary host token in the normal path: it is computed as `ceil(segment.length / 0x400)` KiB.
- The `0x18/0x16/0x17` family is not evidence for new Line-A syscalls; it is an updater-protocol sequence.

## Remaining verification target

The next firmware-first closure target is the zero-length `0x17` special case at destination `0x005FFC00`: identify the exact Small ROM branch/effect, compare available Small ROM generations, and either confirm a concrete erase-range policy or retain it as `NO_RESUELTO / EVIDENCIA_INSUFICIENTE`.

Firmware binaries, full disassembly and private regression artifacts intentionally remain outside the public repository.
