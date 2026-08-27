# System 3 / OS3K clipboard editing block — A200–A208

This note closes the mechanical ABI of the System 3 copy/cut/paste primitives
A200, A204 and A208.  Their functional roles are clear, but reliable original
System 3 public symbols have not been recovered, so the SDK intentionally keeps
`SYS_A200`, `SYS_A204` and `SYS_A208` rather than promoting descriptive guesses.

The analysis was compared across the November 2005 AlphaSmart 3000 ROM,
November 2005 NEO ROM and July 2013 NEO/System 3.15 ROM, and cross-checked
against official SmartApplet callers and the already named A20C/A210/A214
clipboard routines.

## 1. Shared clipboard model

System 3 owns a dedicated clipboard descriptor.  The file resolver recognizes
reserved token `0x00CB` as this descriptor.  The clipboard also has a shared
non-zero byte stored in the same global field used by `ClipboardSet` (A20C) and
cleared by `ClipboardClear` (A214).

Because the exact public taxonomy of this byte has not been recovered, this
document calls it **`clipboard_tag`**.  This is intentionally neutral: the
firmware proves that the value is required and preserved with clipboard
contents, but not an original public enum name.

## 2. A200 — copy an active-file range to the clipboard

```c
int32_t SYS_A200(uint32_t count, uint8_t clipboard_tag);
```

A200 copies from the current cursor of the active file into the dedicated
clipboard descriptor without modifying the source file.

Validation:

- no active descriptor -> raw `-3`;
- `count > 0x7FFFFFFF` -> raw `-128`;
- `count > current_size - cursor` -> raw `-128`;
- `clipboard_tag == 0` -> raw `-129`.

On a valid request:

1. clipboard state bit 2 at descriptor byte `+0x23` is cleared;
2. System 3 adjusts/reclaims clipboard allocation as needed;
3. the actual transfer count is limited to the clipboard's resulting maximum
   capacity:

```text
actual = min(count, clipboard.max_size_after_allocation)
```

4. the non-zero `clipboard_tag` is saved in the shared clipboard-tag global;
5. `actual` bytes are copied from `active.storage + active.cursor` to clipboard
   storage;
6. clipboard current/recoverable size is set to `actual`;
7. the function returns `actual`.

The active cursor and source contents are unchanged.

### Official caller evidence

The scanned AlphaQuiz image contains a direct A200 call whose stack setup passes
its transfer count first and literal tag `1` second.  This independently fixes
argument order and width.

## 3. A204 — cut an active-file range to the clipboard

```c
int32_t SYS_A204(uint32_t count, uint8_t clipboard_tag);
```

A204 shares A200's validation and clipboard-allocation logic, then removes from
the source exactly the range successfully copied.

Validation errors are the same:

- no active descriptor -> `-3`;
- invalid/over-remaining count -> `-128`;
- zero tag -> `-129`.

The critical invariant is:

```text
actual = min(count, clipboard.max_size_after_allocation)
copy active[cursor .. cursor+actual) -> clipboard
FileDeleteChars(actual)
return actual
```

Therefore, if the clipboard allocation cannot hold the full requested range,
**only the portion actually copied is deleted from the source**.  A204 never
blindly deletes the original requested `count` after a clipped copy.

The direct call from A204 to A1A4 was also the decisive internal evidence used
to close A1A4 as `FileDeleteChars`.

No direct A204 call was found in the scanned official 2012 SmartApplets; it is
used by System 3's internal cut behavior.

## 4. A208 — paste clipboard data into the active file

```c
int32_t SYS_A208(uint8_t write_mode, uint8_t read_variant);
```

A208 writes the dedicated clipboard contents to the active file by delegating
to the A198 file-write family.  It supplies active token `0`, clipboard storage
and clipboard current size to that writer.

### `write_mode`

The first byte is exactly the A198 write-mode selector:

| Value | Mechanical behavior |
| ---: | --- |
| `1` | append at EOF; grow current size |
| `2` | insert at cursor; shift suffix right; grow size and cursor |
| `3` | overwrite at cursor; may extend size; advance cursor |
| other | underlying A198 path returns raw `-5` |

The numeric values are mechanically closed.  No reconstructed macro names are
published yet because the original public enum names have not been recovered.

### `read_variant`

The second byte chooses the source-transfer variant:

- `read_variant == 3`: use an alternate A198 entry whose byte-transfer helper is
  **control-code-aware/filtered**;
- any other value: use the ordinary/raw copy path.

The alternate helper is not a plain `memcpy`: it walks source bytes through an
internal control-code table/switch, may consume or skip control sequences, and
can emit visible characters such as hyphen, tab or space while maintaining the
actual transferred count.  This is sufficient to close the mechanical
selection rule, but not sufficient to claim a higher-level public format name.

A208 returns the result of the selected underlying writer.

### Official caller evidence

AlphaQuiz contains a direct A208 call with arguments `(3, 3)`, i.e. overwrite
mode plus the filtered/control-code-aware source path.

## 5. Relation to the named clipboard routines

The neighboring traps already carried BetaWise names:

- A20C `ClipboardSet`
- A210 `ClipboardGet`
- A214 `ClipboardClear`

They provide important cross-evidence:

- `ClipboardSet` takes external buffer data, a count and an analogous non-zero
  tag, then stores that tag in the same shared global used by A200/A204;
- `ClipboardGet` has an analogous third source/read selector and uses the same
  filtered helper when that selector equals `3`;
- `ClipboardClear` resets clipboard sizes/cursor/allocation state and clears the
  shared tag global.

This relation justifies the neutral terms `clipboard_tag` and `read_variant`
without pretending that the original symbolic enum names are known.

## 6. Historical naming boundary

The recovered early File-module headers contain a declaration for
`FilePasteClipboard(void)`, proving that a clipboard-paste concept existed in the
early AS3000 code base.  An ABI-identical implementation matching modern A208
has not been recovered, and the System 3 routine now takes two explicit selector
bytes.  Therefore A208 is **not** renamed `FilePasteClipboard` on lineage alone.

Likewise, the copy and cut mechanics of A200/A204 are unambiguous but no primary
public names have been recovered.  All three remain `SYS_Axxx`.

## 7. Cross-ROM stability

Handler entry addresses:

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 | 2013 handler length |
| --- | ---: | ---: | ---: | ---: |
| A200 | `0x4E096C` | `0x5E32C8` | `0x4396C6` | `0xD4` |
| A204 | `0x4E0A40` | `0x5E339C` | `0x43979A` | `0xE4` |
| A208 | `0x4E0B24` | `0x5E3480` | `0x43987E` | `0x66` |

Same-position byte identity is about 89–93% across the compared images.  The
control flow, error tests, clipboard descriptor use, write-mode delegation and
A204 -> FileDeleteChars relation are stable across generations.

## 8. Regression and safety policy

These operations modify file or clipboard state, so regressions should run
first in a firmware-first emulator with disposable snapshots.

Minimum regression matrix:

1. A200 normal copy leaves source cursor/data unchanged and returns actual count.
2. A200 rejects no-active, over-remaining and zero-tag requests with the raw
   statuses above.
3. A204 performs copy then deletes exactly `actual`, including a deliberately
   clipped clipboard-capacity case.
4. A208 verifies append, insert and overwrite modes independently.
5. A208 compares raw and `read_variant==3` filtered behavior using known control
   sequences.
6. ClipboardSet/Get/Clear are correlated against the same tag/variant state.

No emulator or hardware regression is claimed as executed by this document.
