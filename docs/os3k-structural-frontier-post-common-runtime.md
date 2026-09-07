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

This strongly supports a bank/container/boundary interpretation, but the precise meaning of each copy remains unresolved pending source/build/updater correlation.

## `0x040000` is not a common semantic continuation

Although all three images reach the physical coordinate `0x040000`, what follows differs:

- AS3000 enters resource/string material.
- NEO 2005 has only `0x18` bytes remaining in the file.
- NEO 2013 enters executable 68k code and continues to a later repeated trailer.

Therefore `0x040000` is a coincident **physical boundary**, not evidence of one common object across generations. Subsequent classification must branch by generation.

## ABI consequence

No Axxx syscall is promoted from any of the regions described here. The evidence is structural and explicitly refutes interpreting zero-fill or trailer metadata as dispatcher entries merely because an arithmetic A-line coordinate overlaps them.

Private regression against the canonical images: **60/60 checks passed**. The regression validates hashes, generation-specific zero-fill geometry, trailer layout, metadata fields, post-`0x040000` divergence, and the repeated NEO 2013 trailer pattern.
