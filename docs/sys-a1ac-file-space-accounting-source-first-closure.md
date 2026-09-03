# A1AC / SYS_A1AC file-space accounting — source-first re-audit

Status: **closed A / source-first**.

```c
uint32_t SYS_A1AC(void);
```

No reliable modern vendor symbol was recovered, so the neutral `SYS_A1AC` name is retained.

## Source correlation

The early AS3000 `FileModule.c/h` establishes the descriptor/storage model but does not expose a public equivalent for this modern global free/reclaimable-space query. This negative source result is important: names such as `FileGetFreeSpace`, `FileGetAvailableSpace`, or `FileGetMaxFileSize` would be inventions and are not promoted.

The previously reconstructed System 3 descriptor model supplies the field meanings used by A1AC. In particular, later A1E8 evidence corrected descriptor `+0x10` to **minimum allocation / min_size**. Therefore the A1AC accounting watermark is interpreted with the corrected model `max(recoverable_size, min_size)`, not the older provisional label “accounting threshold”.

AlphaWord Plus callers previously correlate A1AC with UI states containing “Characters used”, “Characters available”, “Pages used”, and “Pages available”, supporting a global storage-accounting role without recovering a symbol name.

## Canonical firmware revalidation

Canonical ROM hashes were rechecked before extracting A1AC.

| ROM | Runtime | File offset | Length |
| --- | ---: | ---: | ---: |
| AS3000 2005 | `0x004E16D8` | `0x216D8` | `0xC8` |
| NEO 2005 | `0x005E4034` | `0x24034` | `0xC8` |
| NEO 2013 | `0x0043A436` | `0x2A436` | `0xC8` |

193 of 200 handler bytes are identical across all three images. AS3000↔NEO2005 differs by six bytes, NEO2005↔NEO2013 by six, and AS3000↔NEO2013 by seven; the divergent bytes are relocation/global/trace operands rather than control-flow changes.

Direct byte anchors present identically 3/3 include:

- mask `0xFFFFFE00` for 0x200-block floor alignment;
- three `0x200` thresholds;
- descriptor stride `0x48`;
- repeated descriptor references to offsets `+0x08` and `+0x10`;
- identical prologue/epilogue structure.

## Firmware-confirmed behavior

A1AC takes no syscall arguments and computes a global allocatable/reclaimable-storage metric in 0x200-byte units.

The reconstructed flow:

1. account separately for reclaimable slack associated with the clipboard descriptor;
2. include the still-unassigned global storage tail;
3. include reclaimable slack for file descriptors using the corrected watermark `max(recoverable_size, min_size)`;
4. floor candidate slack using `& 0xFFFFFE00`;
5. do not add a rounded region when it is `<= 0x200`;
6. when an active descriptor exists, evaluate the active-descriptor path; otherwise iterate the descriptor table with stride `0x48`, excluding clipboard space already accounted separately;
7. return the resulting unsigned 32-bit metric.

The function is a query over allocator/file metadata; it does not resize a file or modify caller-visible cursor/content state.

## Xrefs

Each canonical ROM contains seven absolute occurrences of the A1AC handler address: one vector-table entry and six direct `JSR abs.l` callers inside System 3. The 6/6/6 direct-call count is stable across generations.

Previously traced AlphaWord Plus A-line callers independently place the result in storage-usage/availability presentation paths.

## Refutation

- **Per-file unused capacity** is rejected: that contract belongs to A1B0 and requires a file token.
- **Maximum size getter** is rejected: A1BC returns descriptor max_size for a resolved file.
- **Current size getter** is rejected: A1B4 covers ordinary current_size plus special aggregate selectors.
- **Simple total-free counter** is incomplete: A1AC explicitly includes reclaimable descriptor/clipboard slack and block rounding.
- A modern public name is not invented because the historical source has no matching exported service.

## Classification

- **CONFIRMADO:** `uint32_t(void)` ABI; global/reclaimable file-space accounting; 0x200 block floor; descriptor stride 0x48; use of recoverable/min_size watermark; 3/3 structural equivalence; six direct System 3 callers per ROM.
- **INFERENCIA FUERTE:** role as the system-level “available/reclaimable characters/storage” metric shown by AlphaWord Plus UI paths.
- **DESCONOCIDO:** modern vendor symbol name and precise user-facing unit terminology beyond the firmware’s byte/character storage accounting.

## Regression

Static regression **EXECUTED: 43/43 PASS** against all three canonical ROMs. It checks hashes, handler bounds, no-argument shape, constants/mask/stride/descriptor-field anchors, vector/xref counts, epilogue and cross-ROM byte equivalence.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**. It should validate monotonic changes under controlled allocation/reclaim scenarios, clipboard contribution, active/no-active descriptor paths, 0x200 rounding boundaries and consistency with A1B0/A1B4/A1BC invariants.
