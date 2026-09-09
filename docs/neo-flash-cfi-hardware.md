# AlphaSmart NEO flash / CFI hardware notes

## Status

This note bridges the OS3K updater analysis, physical-hardware work, and emulator work. It intentionally separates firmware-demonstrated behavior from board-specific component reports.

- **CONFIRMED in NEO Small ROM firmware:** flash geometry is queried dynamically through Common Flash Interface (CFI) before the updater performs sector/block erase operations.
- **CONFIRMED in NEO Small ROM firmware:** the RAM-copied erase worker parses erase-region geometry and uses an AMD/Fujitsu-style sector-erase command path for the accepted primary command-set family.
- **NOT universalized:** the exact flash part number and erase-block size can vary by board revision and must be established independently.

No ROM image or extended disassembly is included here.

## Firmware-facing behavior

The NEO Small ROM updater command `0x17` reaches a small trampoline that copies its flash worker into RAM before executing it. Executing flash-mutating code from RAM is consistent with the requirement not to fetch instructions from the NOR array while an erase/program operation is in progress.

The copied worker performs a CFI query rather than relying on a compile-time erase geometry. The verified functional sequence is:

1. enter CFI query mode;
2. verify the `QRY` signature;
3. read device-size and erase-region fields;
4. parse erase-region descriptors to determine block boundaries for the requested address;
5. inspect the primary command-set identifier;
6. execute the supported sector-erase command path;
7. poll completion and return status.

The CFI erase-region descriptor format itself stores the number of equal-sized blocks in a region and the block size in units of 256 bytes (with the CFI-defined special case for a zero size field). This is why an emulator should model geometry rather than hard-code `0x400`-byte erase units from the host protocol.

## Consequence for updater `erase_kb = 0`

The host-side `erase_kb` field controls the command `0x17` loop bound; it is not the physical NOR erase-block size. Separate firmware verification shows that the loop is post-tested, so `erase_kb = 0` still reaches the flash worker once before the first end-address comparison.

Combining both findings gives the safe device-side model:

> `erase_kb = 0` causes one first erase attempt on the CFI erase block that contains the requested base address.

The exact byte range of that block comes from the real flash device's CFI geometry.

## Independent board report: ST M29W320EB

A 2017 Hackaday.io AlphaSmart NEO teardown discussion contains a first-person board report identifying the NOR part in one NEO as **ST M29W320EB**, 32 Mbit / 4 MiB. This is useful independent hardware evidence, but it is **not treated as a universal NEO bill of materials**.

Source: https://hackaday.io/project/25732-hacking-the-alphasmart-neo/log/62397-whats-inside

The M29W320EB datasheet describes a bottom-boot x8 geometry with eight 8 KiB parameter blocks at the bottom of the array followed by 63 main blocks of 64 KiB. One accessible datasheet copy is:

https://www.mouser.com/datasheet/2/671/M29W320E-1282934.pdf

If a particular NEO is independently confirmed to use this part and maps the NOR array at `0x00400000`, then updater address `0x005FFC00` corresponds to device offset `0x001FFC00`, which lies in the 64 KiB main block `0x001F0000..0x001FFFFF`, or CPU addresses `0x005F0000..0x005FFFFF`.

That 64 KiB result is therefore **hardware-specific / INFERENCIA FUERTE**, not a universal firmware constant.

## Emulator requirements

For `alpha-emu`, MAME, or another NEO emulator, prefer a small CFI-capable NOR abstraction rather than a flat writable byte array with a fixed erase size. At minimum the model should expose:

- CFI query mode and `QRY` signature;
- device-size field;
- erase-region count and descriptors;
- primary command-set identifier;
- sector erase using the geometry of the selected device profile;
- erased state `0xFF`;
- enough command/reset behavior for the updater's observed AMD/Fujitsu-style path.

A first profile may use M29W320EB geometry for experimentation, but it should be named as a board profile and kept replaceable. The firmware itself is geometry-driven, so making the emulator geometry-driven avoids encoding an unproven board-wide assumption.

## Cross-generation caution

Historical AlphaSmart 3000 updater sources already show that AlphaSmart supported multiple NOR families and non-uniform sector maps. The production AS3000 Micron/Intel profile and AMD/ST profiles did not share one universal sector geometry. That historical architecture is consistent with the NEO firmware's decision to query CFI dynamically.

Therefore, do not derive NEO flash geometry from the AS3000 sector table, and do not derive every NEO revision from one teardown.

## Regression provenance

Private regression `verify_neo_smallrom_cmd17_cfi_erase_worker_2026-09-09.py` is **EXECUTED: 35/35 PASS**. It checks the RAM-copy envelope, CFI query/signature and geometry accesses, command-set tests, sector-erase sequence, completion polling, status-return semantics, post-test relationship, and negative checks against fixed 1 KiB/64 KiB erase constants.

Private firmware and extended decode remain outside this repository.
