# SYS_A204 / A204 — clipboard cut primitive

Status: mechanically and semantically closed from the three canonical System 3 generations. The original modern vendor symbol was **not recovered**, so the neutral `SYS_A204` name is retained.

## Contract

```c
int32_t SYS_A204(uint32_t count, uint8_t clipboard_tag);
```

A204 copies a range beginning at the active-file cursor into the dedicated System 3 clipboard and then deletes from the source exactly the number of bytes that were successfully copied.

Validation is shared with the neighboring A200 copy primitive:

- no active descriptor: `-3`;
- `count > 0x7fffffff`: `-128`;
- `count > active.current_size - active.cursor`: `-128`;
- `clipboard_tag == 0`: `-129`.

For a valid request, System 3 prepares/reclaims clipboard allocation, computes an actual transfer limited by the resulting clipboard capacity, saves the shared non-zero clipboard tag, copies `actual` bytes to the clipboard, then invokes `FileDeleteChars(actual)` on the active source. The return value is `actual`.

The important invariant is therefore:

```text
actual = min(requested_count, clipboard_capacity_after_allocation)
copy source[cursor .. cursor+actual) -> clipboard
FileDeleteChars(actual)
return actual
```

A clipped clipboard allocation never causes uncopied source bytes to be deleted.

## Source-first naming pass

BetaWise still labels index 129 as `SYS_A204`. Historical File-module material recovered for this subsystem includes `FilePasteClipboard(void)`, but no primary historical or modern symbol has been recovered for the cut operation itself. Exact searches for plausible cut/copy names produced no reliable AlphaSmart primary source. Descriptive labels such as `FileCutClipboard` are therefore not promoted to vendor names.

## Firmware evidence

Canonical public handlers:

| ROM | entry | length |
| --- | ---: | ---: |
| AlphaSmart 3000 System 3 (2005) | `0x004E0A40` | `0xE4` |
| NEO System 3 (2005) | `0x005E339C` | `0xE4` |
| NEO System 3 (2013) | `0x0043979A` | `0xE4` |

Direct ROM revalidation confirms the shared A200-style range/tag validation and clipboard preparation. More importantly, each handler contains a direct absolute call at handler offset `+0xBC` to the independently reconstructed A1A4 `FileDeleteChars` vector:

- AS3000 -> `0x004E1632`
- NEO 2005 -> `0x005E3F8E`
- NEO 2013 -> `0x0043A390`

That call follows the copy/actual-count calculation and confirms the copy-then-delete behavior using the clipped actual count rather than the original request.

No direct absolute JSR to A204 itself was found in the three complete firmware images (`0/0/0`). This negative result is preserved; it does not contradict use through A-line SmartApplet/system dispatch.

## Refutation

- A204 is not an alias of A200: it contains the additional direct `FileDeleteChars` call and therefore mutates the source.
- It does not blindly delete the requested `count`; the deletion uses the already capacity-limited actual count.
- A zero clipboard tag is not accepted; it follows the `-129` path.
- The historical paste symbol does not identify this cut primitive.

## Confidence

- **CONFIRMED:** ABI; validation/status values; copy to clipboard; capacity clipping; direct A1A4 deletion using actual count; actual-count return; equivalent semantics across all three canonical generations.
- **STRONG INFERENCE:** descriptive role “cut active-file range to clipboard.”
- **UNKNOWN:** original modern vendor function name and exact original type/name of `clipboard_tag`.

Static source-first revalidation: **33/33 PASS**. Dynamic/emulator regression remains **SPECIFIED / NOT EXECUTED**, including a deliberately capacity-clipped case to prove that only `actual` bytes are removed.
