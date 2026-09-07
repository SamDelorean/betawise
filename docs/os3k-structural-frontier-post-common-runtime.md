# OS3K structural frontier after `COMMON_RUNTIME_DATA_BLOCK`

Status: structural research note. This document intentionally contains no firmware dump, large disassembly, or Ghidra-derived binary material.

## Canonical inputs

The structural observations below were rechecked against the three canonical OS3K images identified by SHA-256:

- AS3000: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

`COMMON_RUNTIME_DATA_BLOCK` is `0x63C` bytes long but ends at different file offsets by generation:

| Generation | First byte after common runtime block |
| --- | ---: |
| AS3000 | `0x02B92C` |
| NEO 2005 | `0x032150` |
| NEO 2013 | `0x03FF80` |

This distinction is important: identical nominal A-line spacing is not evidence that these locations are syscalls.

## Post-common zero-fill

Direct canonical-image verification gives:

- AS3000: `[0x02B92C, 0x03FF80)` is all zero (`0x14654` bytes).
- NEO 2005: `[0x032150, 0x03FF80)` is all zero (`0xDE30` bytes).
- NEO 2013: no corresponding gap; its common runtime block ends at `0x03FF80`.

These AS3000/NEO 2005 spans are therefore structural zero-fill/padding, **not ABI entries**. The semantic end of the common runtime block is still generation-specific; the fact that its own zero-filled tail is physically contiguous with later zero-fill does not merge the objects.

## `0x03FF80..0x03FFFF` trailer/entry block

All three canonical images contain a homologous `0x80`-byte object at `[0x03FF80,0x040000)`.

Confirmed structure:

- 68k entry scaffold that establishes an initial stack pointer and clears `A6`;
- four absolute jump slots;
- an SR mask operation using `0xF0FF`;
- a 24-byte zero-reserved area;
- `0xFFFFFFFF` marker;
- revision bytes;
- system-name and build-date metadata.

Observed initial stack values are `0x0003FFF0` for AS3000 and `0x0007FFF0` for both NEO images. Historical AS3000 initialization source independently places RAM at zero and Flash at `0x00400000`, which is consistent with treating this area as startup/layout infrastructure rather than an A-line ABI table.

The exact vendor identities of the four absolute jump destinations are intentionally left **unresolved**. Opcode shape alone is insufficient evidence for naming them.

## NEO 2013 repeated trailer evidence

NEO 2013 contains additional homologous objects:

- `[0x03FF80,0x040000)` and `[0x05FF80,0x060000)` are byte-identical.
- `[0x060780,0x060800)` repeats the same code prefix and principal metadata, with a different revision field and final padding.

The repetition is confirmed structural evidence, but it does **not** by itself prove that raw file offset `0x040000` begins another bank or independently loaded segment. The earlier bank/container-boundary interpretation is therefore withdrawn as over-strong.

Source-first correlation with `ioma8/neo-re` provides the missing coordinate model. `alpha-emu` maps the OS package at `0x00410000` and documents a late NEO main executable span of `0x60000` bytes (`0x00410000..0x00470000`). Its layout patcher independently uses the exact pair `CHAR_HOOK_CODE_OFFSET = 0x42E8E` and `CHAR_HOOK_RUNTIME_ADDRESS = 0x00452E8E`, which demonstrates the package-relative relation `runtime = 0x00410000 + file_offset` for that stock-image coordinate system. Under that mapping, raw `0x040000` corresponds to `0x00450000`, inside the same executable continuum rather than at its start.

`neo-re` also distinguishes whole-package investigation coordinates from updater segment descriptors. Those coordinate systems must not be conflated when inferring installed segment boundaries from raw package offsets.

References:

- `ioma8/neo-re`, `docs/2026-04-20-alpha-emu-memory-map.md`
- `ioma8/neo-re`, `alpha-emu/src/firmware.rs`
- `ioma8/neo-re`, `layout-patcher/src/layout_patcher/firmware.py`

## NEO 2013 active-to-unused-ROM frontier

A dedicated source-first check now provides a real internal physical boundary after raw `0x040000`.

`neo-re` deliberately places its keyboard-layout hook at file offset `0x42E8E` / runtime `0x00452E8E` and describes that destination as unused ROM space. Direct verification of the canonical NEO 2013 image independently matches that claim:

- `[0x040000, 0x042E8E)` contains active non-fill bytes and must be treated as a mixed code/data region until its internal subobjects are separated.
- The active tail ends with a 68k `RTS` (`4E75`) at raw `0x042E8C..0x042E8D`.
- `[0x042E8E, 0x05FF80)` is one continuous `0xFF` span of length `0x1D0F2`.
- The repeated `0x80`-byte trailer begins exactly at raw `0x05FF80`.

Therefore `[0x042E8E, 0x05FF80)` is structurally **closed as unused/erased ROM fill**, not code and not ABI. The former shorthand suggesting executable code continued all the way to `0x05FF80` was too broad and is withdrawn. The preceding `[0x040000, 0x042E8E)` remains `PARCIAL_CERRADO`: its active extent is confirmed, but its internal code/data object boundaries still require source/xref/firmware decomposition.

The canonical host file also has an `0x800`-byte tail after the main `0x60000` area:

- `[0x060000, 0x060014)` is 20 non-zero bytes beginning with the System 3 Neo identifier;
- `[0x060014, 0x060780)` is zero-filled;
- `[0x060780, 0x060800)` is the final homologous trailer with revision `03 04`.

The updater segment table independently declares `0x00410000/0x60000`, `0x00406000/0x14`, and `0x005FFC00/0x400`. The length correlations are strong, but this note intentionally does not assign exact host-file payload placement to the two secondary descriptors until the updater writer path is reconstructed.

Dedicated private canonical-image regression for this frontier: **16/16 checks passed**. It verifies the canonical hash, segment descriptors, active-to-`0xFF` transition, final `RTS`, exact erased-fill extent, repeated trailer boundary, and host-tail geometry.

## `0x040000` is not a common semantic continuation

Although all three images reach the physical coordinate `0x040000`, what follows differs:

- AS3000 enters resource/string material.
- NEO 2005 has only `0x18` bytes remaining in the file.
- NEO 2013 remains inside its main OS package, with an active region through raw `0x042E8D` followed by erased/unused `0xFF` fill through `0x05FF7F`.

Therefore `0x040000` remains useful as a **cross-generation physical comparison coordinate**, but it is not evidence of one common object across generations and must not be promoted to an internal NEO 2013 bank/segment boundary. Subsequent classification must branch by generation and follow source-supported object boundaries.

## ABI consequence

No Axxx syscall is promoted from any of the regions described here. The evidence is structural and explicitly refutes interpreting zero-fill, erased-fill, or trailer metadata as dispatcher entries merely because an arithmetic A-line coordinate overlaps them.

Private canonical regressions are now **EJECUTADAS**: the earlier post-common/trailer suite passed **60/60**, and the dedicated NEO 2013 post-`0x040000` frontier suite passed **16/16**.
