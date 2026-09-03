# SYS_A200 / A200 — clipboard copy primitive

Status: mechanically and semantically closed from the three canonical System 3 generations. The original modern vendor symbol was **not recovered**, so the neutral `SYS_A200` name is retained.

## Contract

```c
int32_t SYS_A200(uint32_t count, uint8_t clipboard_tag);
```

The operation copies a range beginning at the active file cursor into the dedicated System 3 clipboard descriptor without modifying the source file or its cursor.

Validation observed in firmware:

- no active descriptor: `-3`;
- `count > 0x7fffffff`: `-128`;
- `count > active.current_size - active.cursor`: `-128`;
- `clipboard_tag == 0`: `-129`.

On success, the clipboard allocation is prepared, the transfer is clipped to the resulting clipboard capacity, the non-zero tag is stored in the same shared metadata state used by the named clipboard API, and `actual` bytes are copied from `active.storage + active.cursor` to clipboard storage. Clipboard current/recoverable size is set to `actual`, and `actual` is returned.

The source cursor and source contents are unchanged.

## Source-first naming pass

BetaWise still labels index 128 as `SYS_A200`. Existing historical File-module material provides a genuine historical symbol `FilePasteClipboard(void)` for the paste lineage, but no primary historical or modern symbol was recovered for this copy operation. Because that historical paste name does not identify A200, descriptive guesses such as `FileCopyClipboard` are not promoted to vendor names.

The neighboring named functions `ClipboardSet`, `ClipboardGet`, and `ClipboardClear`, plus the dedicated clipboard descriptor and shared metadata byte, confirm the subsystem relationship but do not reveal the original A200 symbol or original enum name for `clipboard_tag`.

## Firmware evidence

Canonical entry points and public handler length:

| ROM | entry | length |
| --- | ---: | ---: |
| AlphaSmart 3000 System 3 (2005) | `0x004E096C` | `0xD4` |
| NEO System 3 (2005) | `0x005E32C8` | `0xD4` |
| NEO System 3 (2013) | `0x004396C6` | `0xD4` |

Direct revalidation confirms the canonical ROM hashes, vector boundaries, count/range tests, raw error constants, clipboard-state bit clear, stable prologue/epilogue, and the same core structure in all three generations. Same-position byte identity is 92.9% for AS3000 vs NEO 2005 and 92.5% for AS3000 vs NEO 2013; differences are relocation/platform operands rather than a semantic split.

No direct absolute JSR to the A200 handler exists inside the three complete System 3 ROMs (`0/0/0`), consistent with its use through A-line stubs by SmartApplets. Prior official-caller correlation in AlphaQuiz fixes argument order as `count` first and tag second, with literal tag `1` in the observed caller.

## Refutation

The firmware and adjacent API rule out several alternatives:

- A200 is not the cut primitive: it does not delete source data; A204 is the neighboring cut operation.
- an overlong source request is not silently clipped to source remaining length; it returns `-128`.
- the tag is not optional; zero returns `-129`.
- the historical `FilePasteClipboard` symbol cannot be assigned to A200 merely because both belong to the clipboard subsystem.

## Confidence

- **CONFIRMED:** ABI; validation/status values; copy from active cursor; clipboard-capacity clipping; tag storage; returned actual count; no source mutation; equivalent semantics across all three canonical generations.
- **STRONG INFERENCE:** descriptive role “copy active-file range to clipboard.”
- **UNKNOWN:** original modern vendor function name and exact original name/type of `clipboard_tag`.

Static revalidation performed during this source-first pass: **30/30 PASS**. Dynamic/emulator regression remains **SPECIFIED / NOT EXECUTED** and should use disposable file/clipboard state.
