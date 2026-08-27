# System 3 / OS3K File API — current developer reference

This document is the **current operational reference** for the reconstructed
System 3 file subsystem.  It is intentionally different from
`file-api-reconstruction.md`, which is a chronological research log and still
contains provisional statements that were later superseded.

Use this document to understand how the File API is meant to be called. Use the
linked closure notes when the evidence trail, historical genealogy, ROM
addresses, or edge cases need to be audited.

## 1. Mental model

System 3 file operations are built around three related concepts:

1. a **file namespace/folder** owned by a runtime SmartApplet;
2. a **16-bit file token** resolved inside that namespace;
3. a global **active descriptor** used by operations that do not receive an
   explicit `file_id`.

`FileSetFolder` selects the namespace. `FileOpen` resolves a token and makes the
resulting descriptor active. Operations such as `FileDeleteChars` and several
size-changing calls then act on that active descriptor. `FileClose` only
detaches the active descriptor; it is not a flush or commit operation.

## 2. Reconstructed descriptor fields

The analyzed descriptor stride is `0x48` bytes. Fields currently required to
understand the reconstructed API are:

| Offset | Current interpretation | Confidence |
| --- | --- | --- |
| `+0x00` | storage/base pointer | A |
| `+0x04` | current logical size | A |
| `+0x08` | previous/recoverable size / high-water state | A |
| `+0x0C` | maximum allocated capacity | A |
| `+0x10` | minimum allocation / `min_size` | A |
| `+0x14` | current cursor/file position | A |
| `+0x18` | NUL-terminated file password, maximum 5 chars + NUL | A |
| `+0x20` | per-file state flags | A mechanically; bit names open |
| `+0x24` | caller address bound as live storage-pointer mirror | A |
| `+0x28` | caller address bound as live current-size mirror | A |
| `+0x2C` | caller address bound as live max-size mirror | A |
| `+0x30` | caller address bound as live cursor mirror | A |
| `+0x34` | NUL-terminated filename, maximum 15 chars + NUL | A |
| `+0x44` | canonical 16-bit file token | A |
| `+0x46` | 16-bit local file index | A |

A1E8 closes `+0x10` specifically as `min_size`: creation stores its explicit
`min_size` argument there and the ROM contains the diagnostic `File %d Min Size
overflow`. Earlier project notes that call `+0x10` only an accounting/reservation
threshold are superseded.

Do not expose the complete descriptor as a stable public C structure yet. This
table is a reconstructed implementation model; unknown fields may differ across
firmware generations.

## 3. Token rules

An ordinary non-special token is resolved in the namespace selected by
`FileSetFolder`.

- token `0` means **the current active descriptor** to the shared resolver;
- token `0x00CB` is the reserved **clipboard descriptor** token;
- A1FC demonstrates explicitly that identity is a 16-bit token composed of high
  group/class byte plus low byte, rather than the earliest AS3000 single-byte
  sequential file-number model.

## 4. Namespace selection — `FileSetFolder` / A1C0

```c
int32_t FileSetFolder(uint32_t applet_index, uint32_t *applet_flags_out);
```

`applet_index` is a runtime SmartApplet index `0..31`, not the 16-bit applet ID.
On success the selected namespace becomes current, `applet_flags_out` receives
the full `AppletHeader_t.flags` when non-NULL, and the return is the previous
folder/runtime index. Known raw failures are `-64` for invalid/uninstalled
runtime index and `-7` for the no-eligible-file-group path when header flag
`0x40` is clear. Both fail before detaching the prior active descriptor.

Detailed evidence: [`filesetfolder-closure.md`](filesetfolder-closure.md).

## 5. Opening and closing a file context

```c
uint16_t FileOpen(uint16_t file_id, uint8_t reset_position);
void FileClose(void);
```

`FileOpen` failure returns 0 and preserves the previous active descriptor.
Success installs the resolved descriptor; exactly `reset_position == 1` rewinds
its cursor, while other values preserve it. The return is the canonical token.

`FileClose` only sets the global active-descriptor pointer to NULL. It does not
erase, flush, commit, rewind, change metadata, or clear `FileGetFileInfo` live
mirror addresses.

Detailed evidence: [`fileopen-fileclose-closure.md`](fileopen-fileclose-closure.md).

## 6. Inspecting state — `FileGetFileInfo` / A1C4

```c
uint8_t *FileGetFileInfo(
    uint16_t file_id,
    uint8_t **storage_out,
    uint32_t *current_size_out,
    uint32_t *max_size_out,
    uint32_t *cursor_out);
```

Success returns the storage/base pointer, not the descriptor. The four output
addresses become persistent **live mirrors** stored at descriptor `+0x24..+0x30`.
Later file operations may write through them.

```text
storage_out      <- +0x00
current_size_out <- +0x04
max_size_out     <- +0x0C
cursor_out       <- +0x14
```

If a mirror points to an automatic/local variable, explicitly unbind it before
scope exit:

```c
FileGetFileInfo(file_id, NULL, NULL, NULL, NULL);
```

`applets/FileGetFileInfoProbe/` is an executable regression specification; no
run is claimed merely because the probe exists.

## 7. Current-file query — `FileGetCurrentFile` / A1A8

```c
uint16_t FileGetCurrentFile(void);
```

Returns the canonical active token or 0, without modifying state.

Detailed evidence: [`file-core-operations.md`](file-core-operations.md).

## 8. Destructive erase — `FileSmashFile` / A1A0

```c
uint16_t FileSmashFile(void);
```

Zero-fills the entire active file allocation through `max_size`, clears current
size, recoverable/high-water size and cursor, synchronizes live mirrors, and
returns the canonical token. No active descriptor -> 0. This is destructive and
stronger than a logical truncate.

Detailed evidence: [`file-core-operations.md`](file-core-operations.md).

## 9. Delete at cursor — `FileDeleteChars` / A1A4

```c
uint32_t FileDeleteChars(uint32_t delete_count);
```

```text
available = current_size - cursor
actual = min(delete_count, available)
if actual:
    memmove(storage+cursor, storage+cursor+actual, available-actual)
    current_size -= actual
    synchronize_live_mirrors()
return actual
```

Cursor does not move; deletion clamps at EOF; recoverable/high-water size is not
reduced; trailing storage is not zeroed. A204 independently confirms the role by
copying to clipboard then invoking A1A4 with the same actual count.

Detailed evidence: [`filedeletechars-closure.md`](filedeletechars-closure.md).

## 10. Per-file/global size and accounting

Mechanically closed names remain generic where original symbols are unknown.

```c
uint32_t SYS_A1B0(uint16_t file_id); /* max_size-current_size */
uint32_t SYS_A1BC(uint16_t file_id); /* max_size */
uint32_t SYS_A1AC(void);             /* global allocatable/reclaimable metric */
```

For an ordinary token:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

A1AC uses 0x200-unit allocator accounting, includes the global unallocated tail,
and computes reclaimable descriptor slack relative to the greater of
recoverable size and **`min_size`**. Clipboard slack is handled separately.

Detailed evidence: [`file-space-accounting.md`](file-space-accounting.md).

## 11. `SYS_A1B4` — size/accounting query family

Ordinary token -> `current_size`.

| Selector | Mechanical result |
| ---: | --- |
| `0xFC` | active `previous/recoverable_size` |
| `0xFD` | sum current sizes in current file group |
| `0xFE` | sum `min(current_size, min_size)` in current group |
| `0xFF` | same `min(...)` sum across full descriptor table |

The selector namespace is not a set of ordinary sequential file numbers.
Original names for `0xFD..0xFF` remain open.

## 12. `SYS_A1B8` — active-file resize/state family

Ordinary non-negative request is clamped to `max_size`, cursor is pulled back if
past the new EOF, recoverable/high-water is raised on growth, mirrors are synced,
and the resulting size is returned.

| Value | Mechanical behavior |
| ---: | --- |
| `-1` | unsigned clamp effectively selects `max_size` |
| `-2` | request previous/recoverable size |
| `-3` | snapshot current into previous/recoverable |
| `-4` | global snapshot current -> previous when no active descriptor |
| `-5` | set **`min_size = max(current_size, 0x200)`** |
| `-6` | request `max(previous_size, current_size + 40)` then common clamp |

`-2` has strong continuity with historical `FileRestoreOldFile`; other command
names remain unpublished.

## 13. File state flags — A1D0/A1D4

Declared in `os3k/file_password_state.h`:

```c
int32_t SYS_A1D0(uint16_t file_id, uint32_t state_mask, uint8_t enable);
int32_t SYS_A1D4(uint16_t file_id, uint32_t state_mask);
```

Both use descriptor `+0x20`. Supported low-byte mask is `0x0D` in AS3000/NEO
2005 and `0x1D` in NEO 2013. A mask with no supported bit returns raw `-14`.
A1D0 sets only for `enable==1`, otherwise clears; A1D4 returns whether any
requested bit is set. Individual flag names remain open.

## 14. File passwords — A1DC/A1D8

```c
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);
uint32_t SYS_A1D8(void);
```

A1DC gets/sets descriptor `+0x18`. Set strings of length >=6 return raw `-8`;
therefore the normal password limit is five characters plus NUL.

A1D8 is an interactive, destructive master-password-gated reset that sets every
file password in the current group to factory string `write` and returns the
number reset. It is emulator-first with disposable state.

Detailed evidence: [`file-password-state-closure.md`](file-password-state-closure.md).

## 15. Password verification/change dialogs — A1E0/A1E4

These two names have B+ strong historical continuity and A mechanical ABI.

```c
uint8_t PasswordVerifyFileDialog(
    uint16_t file_id,
    uint8_t try_once,
    uint8_t allow_escape,
    uint8_t allow_special_navigation,
    uint8_t allow_file_switching,
    uint16_t *selected_file_out);

int32_t PasswordChangeFileDialog(uint16_t file_id);
```

`PasswordVerifyFileDialog` accepts master or selected file password. Exactly-1
`try_once` exits after one invalid attempt; exactly-1 `allow_escape` enables
Escape; exactly-1 `allow_special_navigation` enables the Applets/special system
exit set; non-zero `allow_file_switching` enables switching among eligible local
file indices. A non-NULL output starts at `0x00FF` and receives the final
canonical token on accepted completion.

`PasswordChangeFileDialog` resolves one token, verifies current/master password,
then requests/validates/commits a new password. **Its return is not a changed
boolean:** any resolved dialog exit, including cancellation, returns the
canonical token; resolver failure returns status.

Detailed evidence: [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md).

## 16. Dynamic descriptors, names and local identity — A1E8–A1FC

Declared under generic names in `os3k/file_dynamic.h`.

### A1E8 — create

```c
int32_t SYS_A1E8(uint8_t token_group, const char *name,
                 uint32_t min_size, uint32_t requested_max_size);
```

Creates/inserts a 0x48-byte descriptor, validates group/name/duplicates,
renumbers affected tokens, initializes local index to `0x00FF`, and allocates:

```text
effective_max = max(requested_max_size, min_size, 0x200)
```

Success stores `min_size` at `+0x10` and returns canonical token. Known raw
errors: `-9` table full, `-10` invalid/unavailable token group, `-12` allocation
failure, `-13` invalid name, `-15` duplicate name.

### A1EC — remove descriptor

```c
int32_t SYS_A1EC(uint16_t file_id);
```

Removes the descriptor itself, reclaims allocation, compacts the table, fixes
group pointers and renumbers tokens. Success ->0. This is distinct from content
erase/truncate.

### A1F0 — filename

```c
char *SYS_A1F0(uint16_t file_id, char *name_buffer, uint8_t set_name);
```

Filename is `+0x34`, max 15 chars + NUL. Get optionally copies to caller and
returns internal name pointer. Set normalizes/validates/duplicate-checks and
returns internal pointer; invalid/duplicate/resolution failure returns NULL.

### A1F4/A1F8 — local index

```c
int32_t SYS_A1F4(uint16_t file_id, uint16_t index_value, uint16_t *index_out);
int32_t SYS_A1F8(uint16_t local_index);
```

A1F4 gets/sets `+0x46`; non-NULL output means get, NULL output means set. A1F8
maps local index back to canonical token; missing index -> raw `-11`.

### A1FC — token construction/validation

```c
int32_t SYS_A1FC(uint8_t token_low, uint8_t token_high);
```

Builds `(high<<8)|low`, resolves it and returns the same token on success or the
resolver status on failure.

Detailed evidence: [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md).

## 17. Clipboard copy/cut/paste — A200–A208

Declared in `os3k/file_clipboard_edit.h`. Names remain generic because exact
System 3 public symbols/enums are not recovered.

```c
int32_t SYS_A200(uint32_t count, uint8_t clipboard_tag);
int32_t SYS_A204(uint32_t count, uint8_t clipboard_tag);
int32_t SYS_A208(uint8_t write_mode, uint8_t read_variant);
```

A200 copies from active `storage+cursor` to the clipboard. No active -> `-3`,
over-range/invalid count -> `-128`, zero tag -> `-129`. Return is actual count
after clipboard-capacity limiting; source cursor/data are unchanged.

A204 is cut: it copies `actual=min(requested,clipboard capacity)`, then invokes
`FileDeleteChars(actual)`. **Only bytes successfully copied are deleted.**

A208 pastes clipboard contents through the A198 write family:

- mode 1 append at EOF;
- mode 2 insert at cursor;
- mode 3 overwrite at cursor;
- other mode -> underlying raw `-5`.

`read_variant==3` chooses a control-code-aware/filtered transfer helper; other
values use the raw source path. The filtered helper interprets/skips control
sequences and can emit visible equivalents; no higher-level format name is
claimed.

A20C `ClipboardSet`, A210 `ClipboardGet`, and A214 `ClipboardClear` corroborate
the shared non-zero clipboard-tag state and the same filtered variant.

Detailed evidence: [`clipboard-edit-closure.md`](clipboard-edit-closure.md).

## 18. Recommended application sequence

A safe explicit sequence for operating in another applet's namespace is:

```c
uint32_t flags = 0;
int32_t previous_folder = FileSetFolder(target_applet_index, &flags);
if (previous_folder < 0)
    return;

uint16_t token = FileOpen(file_id, 1);
if (token == 0) {
    FileSetFolder((uint32_t)previous_folder, NULL);
    return;
}

uint8_t *storage = NULL;
uint32_t size = 0, max_size = 0, cursor = 0;
if (FileGetFileInfo(token, &storage, &size, &max_size, &cursor) == NULL) {
    FileClose();
    FileSetFolder((uint32_t)previous_folder, NULL);
    return;
}

/* perform file operations */

FileGetFileInfo(token, NULL, NULL, NULL, NULL); /* unbind live mirrors */
FileClose();
FileSetFolder((uint32_t)previous_folder, NULL);
```

Mirrors are optional; if they are not needed, do not register them.

## 19. Common mistakes to avoid

- Treating `FileClose` as commit/flush.
- Assuming failed `FileOpen` clears the old context.
- Leaving local `FileGetFileInfo` mirror pointers registered after scope exit.
- Treating token `0` or `0x00CB` as normal user-file numbers.
- Treating `FileSmashFile` as harmless logical clear.
- Assuming `FileDeleteChars` moves the cursor.
- Calling descriptor `+0x10` an unknown threshold after A1E8: it is `min_size`.
- Inventing symbolic names for A1B4/A1B8 special selectors or A200–A208 enums.
- Treating `PasswordChangeFileDialog` return as `password_changed`.
- Using A1EC without recognizing that it removes/compacts descriptor topology.
- Assuming A204 deletes the requested count even when clipboard capacity clips
  the copy; it deletes only `actual`.
- Running A1D8 or other destructive discovery experiments on irreplaceable
  hardware state.

## 20. Validation status

These contracts are reconstructed from firmware, official callers, System 3
internal callers/strings, and original source where applicable. This closes the
machine-level behavior described here.

Emulator/hardware execution is a separate layer. Existing probes and regression
matrices are executable specifications unless their documentation explicitly
states a run has completed. **No emulator or hardware regression for the newly
closed A1E0–A208 block is claimed as already executed.**
