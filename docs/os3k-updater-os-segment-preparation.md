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

## Physical flash worker closure

A later private NEO Small ROM decode closes the previously open physical-helper question without publishing ROM bytes or long disassembly. Command `0x17` reaches a trampoline that copies a 0x1C2-byte flash worker to RAM and executes the RAM copy. The worker:

- enters Common Flash Interface query mode and validates the `QRY` signature;
- reads the device-size and erase-region geometry fields;
- walks the erase-region descriptors and derives block boundaries from the live device geometry;
- reads the primary command-set identifier;
- follows an AMD/Fujitsu-style unlock and sector-erase path for the accepted command-set family;
- polls the target for completion and returns a status value.

The worker therefore does **not** erase in a fixed 1 KiB quantum. It dynamically resolves the physical erase block that contains the requested address from CFI geometry. No fixed 1 KiB or 64 KiB erase quantum is embedded in this worker.

Combining this with the post-test `0x17` loop gives a narrower and stronger zero-length result: `erase_kb = 0` still performs the first block-level erase attempt, and that attempt targets the single CFI erase block containing the requested base address. The byte span of that block is hardware-dependent and must not be universalized without the actual chip/CFI geometry.

One correction is important: an intermediate reverse-engineering hypothesis interpreted the worker's final `D0` as an erase-block size. That is false. The final return is status; the CFI-derived block size is internal working state. That earlier interpretation is retracted.

## Firmware correlation and regression status

Private canonical-firmware work establishes the cross-ROM transition and the Small ROM segment-mapping family without publishing ROM bytes or extended disassembly. The NEO13 host package contains a segment descriptor for `0x005FFC00`, and the private Small ROM mapper verification places the secondary transfer unit there.

The zero-length control-flow property was rechecked mechanically across the older and NEO Small ROM generations with `verify_smallrom_command17_zero_length_posttest_2026-09-09.py`: **21/21 PASS**.

The copied-to-RAM CFI worker was then checked mechanically against the NEO Small ROM with `verify_neo_smallrom_cmd17_cfi_erase_worker_2026-09-09.py`: **35/35 PASS**. That regression verifies the trampoline/copy envelope, CFI query and `QRY` checks, erase-region descriptor parsing, command-set tests, unlock/sector-erase sequence, completion poll, status return, the post-test relationship, and negative checks against fixed 1 KiB/64 KiB erase quanta.

Scripts, outputs, firmware binaries and extended disassembly remain in the private evidence archive.

## Refutation checks

The following narrower interpretations are rejected by source and firmware correlation:

- `0x16` is not a per-segment erase request: it is issued once before the segment loop and carries zero fields.
- `0x17` is not a generic zero-argument mode switch: it is issued once per parsed segment and carries both destination address and an erase-size field.
- `0x17.trailing` is not merely an arbitrary host token in the normal path: it is computed as `ceil(segment.length / 0x400)` KiB.
- `erase_kb = 0` is not decoded by command `0x17` as an `erase-to-end` sentinel.
- `erase_kb = 0` is not a no-op: the post-test loop invokes the flash worker before its first end comparison.
- the worker does not use `erase_kb` as a direct physical block size; flash geometry is read through CFI.
- the worker's final `D0` is not the physical erase-block size.
- the `0x18/0x16/0x17` family is not evidence for new Line-A syscalls; it is an updater-protocol sequence.

## Remaining verification target

The generic firmware-side erase quantum is now closed as **one CFI erase block at a time**. What remains hardware-specific is the exact block size and boundaries on each NEO board revision. That should be resolved from direct chip identification or captured CFI geometry, then fed into the hardware and emulator fronts. A board report identifying an ST M29W320EB is useful corroboration, but it is not treated as a universal NEO bill of materials.

Firmware binaries, full disassembly and private regression artifacts intentionally remain outside the public repository.
