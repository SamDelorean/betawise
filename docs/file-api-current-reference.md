# System 3 / OS3K File API — current developer reference

This document is the **current operational reference** for the reconstructed
System 3 file subsystem.  It is intentionally different from
`file-api-reconstruction.md`, which is a chronological research log and still
contains provisional statements that were later superseded.

Use this document to understand how the file API is meant to be called.  Use
the linked closure notes when the evidence trail, historical genealogy, ROM
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

The analyzed 2005 descriptor stride is `0x48` bytes.  The fields below are the
ones required to understand the reconstructed API:

| Offset | Current interpretation | Confidence |
| --- | --- | --- |
| `+0x00` | storage/base pointer | A |
| `+0x04` | current logical size | A |
| `+0x08` | previous/recoverable size / high-water state | A |
| `+0x0C` | maximum allocated capacity | A |
| `+0x10` | accounting/reservation threshold; original public name open | A mechanically |
| `+0x14` | current cursor/file position | A |
| `+0x18` | NUL-terminated file password, maximum 5 characters + NUL | A |
| `+0x20` | per-file state flags | A mechanically; bit names open |
| `+0x24` | caller address bound as live storage-pointer mirror | A |
| `+0x28` | caller address bound as live current-size mirror | A |
| `+0x2C` | caller address bound as live max-size mirror | A |
| `+0x30` | caller address bound as live cursor mirror | A |
| `+0x44` | canonical 16-bit file token | A |

Do not expose the complete descriptor layout as a stable public C structure yet.
The table is a reconstructed implementation model, not a promise that every
unknown field has the same meaning across all firmware generations.

## 3. Token rules

### Ordinary file token

An ordinary non-special token is resolved in the file namespace selected by
`FileSetFolder`.

### Token `0`

Inside the shared resolver, token `0` means **the current active descriptor**.
This is why many internal file operations do not require a file ID.

Application code should not confuse this resolver convention with a normal user
file number.  It is primarily an internal/context selector.

### Token `0x00CB`

`0x00CB` is the reserved **clipboard descriptor** token.  The identification is
closed by the dedicated resolver path and the `ClipboardSet`, `ClipboardGet`
and `ClipboardClear` routines operating on the same descriptor.

## 4. Namespace selection — `FileSetFolder` / A1C0

```c
int32_t FileSetFolder(uint32_t applet_index, uint32_t *applet_flags_out);
```

`applet_index` is a **runtime SmartApplet index** in the range `0..31`, not the
16-bit applet ID stored in the header.  `AppletFindById()` is the normal way to
obtain such an index when starting from an applet ID.

On success:

- the currently active descriptor is detached through the FileClose path;
- the requested applet namespace becomes current;
- `*applet_flags_out`, when non-NULL, receives the complete 32-bit
  `AppletHeader_t.flags` value;
- the return value is the **previous folder/runtime applet index**, which can be
  passed back to `FileSetFolder` to restore the previous namespace.

Known raw failures:

- `-64`: out-of-range runtime index or uninstalled runtime slot (`0xFFFF`);
- `-7`: selected applet has no eligible runtime file group through the ordinary
  path and the low-byte `0x40` header flag is clear.

Both failures occur before the active descriptor is detached, so a failed
folder switch preserves the prior file context.  The original symbolic names of
`-64`, `-7`, and flag `0x40` have not been recovered.

Detailed evidence: [`filesetfolder-closure.md`](filesetfolder-closure.md).

## 5. Opening and closing a file context

### `FileOpen` / A1C8

```c
uint16_t FileOpen(uint16_t file_id, uint8_t reset_position);
```

Behavior:

- resolve `file_id` in the current namespace;
- resolution failure returns `0` **without destroying the previous active
  descriptor**;
- success detaches the previous active descriptor and installs the resolved one;
- `reset_position` is consumed as a byte; **only the exact value `1`** sets the
  new descriptor cursor to zero;
- any other value preserves the descriptor's existing cursor;
- success returns the descriptor's canonical 16-bit token.

The 25 official SmartApplet callers examined use only `0` and `1` for
`reset_position`.

### `FileClose` / A1CC

```c
void FileClose(void);
```

`FileClose` only sets the global active-descriptor pointer to NULL. It does
**not**:

- erase storage;
- change current or previous size;
- reset the cursor;
- flush or commit data;
- clear `FileGetFileInfo` live-mirror addresses stored in the descriptor.

That last point matters: closing the active context is not a substitute for
unbinding live mirrors.

Detailed evidence: [`fileopen-fileclose-closure.md`](fileopen-fileclose-closure.md).

## 6. Inspecting file state — `FileGetFileInfo` / A1C4

```c
uint8_t *FileGetFileInfo(
    uint16_t file_id,
    uint8_t **storage_out,
    uint32_t *current_size_out,
    uint32_t *max_size_out,
    uint32_t *cursor_out);
```

Resolution failure returns `NULL`.  Success returns the file's **storage/base
pointer**, not the internal descriptor.

The four output arguments are unusual: they are **persistent live bindings**.
System 3 stores the caller addresses in descriptor offsets `+0x24..+0x30`,
updates them immediately, and later file operations may update them again.
They are not ordinary one-shot output parameters.

The mirror mapping is:

```text
storage_out      <- descriptor +0x00
current_size_out <- descriptor +0x04
max_size_out     <- descriptor +0x0C
cursor_out       <- descriptor +0x14
```

### Mandatory lifetime rule

If any mirror points to an automatic/local variable, unregister it before that
variable leaves scope:

```c
FileGetFileInfo(file_id, NULL, NULL, NULL, NULL);
```

System 3 itself uses this explicit unbind pattern around temporary stack
variables.  Failing to unbind can leave a descriptor containing stale caller
addresses that a later file operation may write through.

`applets/FileGetFileInfoProbe/` is a read-only executable regression
specification for the initial binding/unbinding behavior.  It has not been
claimed as already executed in the current emulator/hardware state.

## 7. Current-file query — `FileGetCurrentFile` / A1A8

```c
uint16_t FileGetCurrentFile(void);
```

Returns the canonical token of the active descriptor, or `0` if no active
descriptor resolves.  It does not modify file state.

Official callers zero-extend the returned word and reuse it as a file token,
which closes the interpretation.  The older BetaWise comment that A1A8 might
be a file-count query is obsolete.

Detailed evidence: [`file-core-operations.md`](file-core-operations.md).

## 8. Destructive erase — `FileSmashFile` / A1A0

```c
uint16_t FileSmashFile(void);
```

Operates on the active descriptor.  On success it:

1. zero-fills the entire backing storage through `max_size`;
2. clears `current_size`;
3. clears `previous/recoverable_size`;
4. clears the cursor;
5. synchronizes any live mirrors;
6. returns the active file's canonical token.

No active descriptor -> `0`.

This is an **irreversible data-destruction primitive**.  It is substantially
stronger than reducing the logical file size.  The behavior is a strong modern
continuation of the historical AS3000 `FileSmashFile` operation.

Detailed evidence: [`file-core-operations.md`](file-core-operations.md).

## 9. Deleting data at the cursor — `FileDeleteChars` / A1A4

```c
uint32_t FileDeleteChars(uint32_t delete_count);
```

Equivalent behavior:

```text
available = current_size - cursor
actual = min(delete_count, available)
if actual != 0:
    memmove(storage + cursor,
            storage + cursor + actual,
            available - actual)
    current_size -= actual
    synchronize_live_mirrors()
return actual
```

Important invariants:

- cursor does not move;
- deletion is clamped at EOF;
- previous/recoverable size is not reduced;
- storage beyond the new logical EOF is not explicitly zeroed;
- the return value is the number actually deleted.

A System 3 internal cut path copies the same range to the clipboard before
calling A1A4, providing independent semantic evidence for the deletion role.

Detailed evidence: [`filedeletechars-closure.md`](filedeletechars-closure.md).

## 10. Per-file and global size/accounting primitives

The following mechanics are closed but their original public System 3 names are
not.  They therefore remain `SYS_Axxx`.

### `SYS_A1B0(file_id)` — unused capacity in one descriptor

```c
uint32_t SYS_A1B0(uint16_t file_id);
```

Successful resolution returns:

```text
max_size - current_size
```

Failure returns zero.

### `SYS_A1BC(file_id)` — maximum descriptor capacity

```c
uint32_t SYS_A1BC(uint16_t file_id);
```

Successful resolution returns `max_size`; failure returns zero.

For an ordinary valid token, a useful emulator invariant is:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

because ordinary A1B4 returns `current_size`.

### `SYS_A1AC()` — global allocatable/reclaimable metric

```c
uint32_t SYS_A1AC(void);
```

A1AC is not equivalent to `A1B0` summed naively. It operates in 0x200-byte/
character accounting blocks, includes the global unallocated storage tail, and
computes reclaimable descriptor slack relative to the greater of recoverable
size and the `+0x10` accounting threshold.  Clipboard slack is accounted for
separately.  Small rounded regions at or below one block are excluded by the
observed path.

This is a global allocator/accounting metric, not a single-file free-space
query.  Do not rename it `FileGetFreeSpace` without additional name evidence.

Detailed evidence: [`file-space-accounting.md`](file-space-accounting.md).

## 11. `SYS_A1B4` — size/accounting query family

For an ordinary file token:

```text
SYS_A1B4(file_id) -> current_size
```

Special selector behavior:

| Selector | Mechanical result |
| ---: | --- |
| `0xFC` | active descriptor `previous/recoverable_size` |
| `0xFD` | sum of `current_size` over the current file group |
| `0xFE` | sum of `min(current_size, +0x10 threshold)` over the current group |
| `0xFF` | same `min(...)` sum over the full descriptor table |

These selectors share a token/selector namespace; do not treat the argument as
a simple sequential file number.  Original public names for `0xFD..0xFF` remain
open.

## 12. `SYS_A1B8` — active-file resize/state family

A1B8 operates on the active descriptor.  For an ordinary non-negative
requested size:

```text
new_size = min(requested_size, max_size)
current_size = new_size
if cursor > new_size:
    cursor = new_size
if new_size > previous_size:
    previous_size = new_size
synchronize_live_mirrors()
return new_size
```

Known command values:

| Value | Mechanical behavior |
| ---: | --- |
| `-1` | no dedicated branch; unsigned clamp effectively selects `max_size` |
| `-2` | request `previous/recoverable_size`, then common resize path |
| `-3` | snapshot `current_size` into previous/recoverable, then common path at current size |
| `-4` | global snapshot path when no active descriptor: copy each current size into previous/recoverable; returns 0 |
| `-5` | set `+0x10 = max(current_size, 0x200)` and keep current size |
| `-6` | request `max(previous_size, current_size + 40)`, then clamp/common path |

`-2` has strong historical continuity with the old `FileRestoreOldFile`
concept.  The other command names remain intentionally unpublished until their
original semantics/names are recovered.

## 13. File state flags — A1D0 / A1D4

Published in `os3k/file_password_state.h` under their trap names:

```c
int32_t SYS_A1D0(uint16_t file_id, uint32_t state_mask, uint8_t enable);
int32_t SYS_A1D4(uint16_t file_id, uint32_t state_mask);
```

Both operate on descriptor `+0x20`.

Supported low-byte mask bits:

- AS3000 2005 and NEO 2005: `0x01 | 0x04 | 0x08` (`0x0D`);
- NEO 2013: adds `0x10` (`0x1D`).

If the mask contains no supported low-byte bit, both paths return raw `-14`.
A1D0 sets the requested bits only when `enable == 1`; any other value clears
them.  Successful A1D0 returns the canonical token.  A1D4 returns `1` when any
requested bit is currently set, otherwise `0`.

The individual bit meanings are not sufficiently closed for global
`FILE_FLAG_*` names.

## 14. File passwords — A1DC / A1D8

### `SYS_A1DC` — per-file get/set

```c
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);
```

Descriptor `+0x18` contains a NUL-terminated password.

- `read_back == 0`: set. `strlen(password) >= 6` returns raw `-8`; shorter
  strings are copied into the descriptor.
- `read_back != 0`: get. The stored string is copied into caller-provided
  writable memory.

The normal maximum is **five characters plus NUL**.  A six-byte destination is
therefore sufficient for a valid stored password.  No safe NULL-pointer branch
is visible in the handler.

### `SYS_A1D8` — reset every password in the current group

```c
uint32_t SYS_A1D8(void);
```

This is an interactive, destructive credential operation.  It requests the
master password, verifies it through the A24C helper, and on success sets every
file password in the current group to the factory string:

```text
write
```

It returns the number of file passwords reset; cancellation or failed master
verification returns zero.

The original 1998–2000 PasswordModule independently confirms both the five
character password limit and the factory value `write`.

**Testing rule:** A1D8 is emulator-first with disposable state.  Do not use it
as a hardware discovery probe.

Detailed evidence: [`file-password-state-closure.md`](file-password-state-closure.md).

## 15. Recommended application sequence

A safe explicit sequence for operating in another applet's file namespace is:

```c
uint32_t flags = 0;
int32_t previous_folder = FileSetFolder(target_applet_index, &flags);
if (previous_folder < 0) {
    /* namespace switch failed; previous active context is preserved */
    return;
}

uint16_t token = FileOpen(file_id, 1); /* reset cursor */
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

/* perform file operations while mirrors are valid */

/* Critical: remove pointers to local variables before leaving this scope. */
FileGetFileInfo(token, NULL, NULL, NULL, NULL);
FileClose();
FileSetFolder((uint32_t)previous_folder, NULL);
```

This is a lifecycle example, not a claim that every application must bind live
mirrors.  If state mirrors are not required, simply do not register them.

## 16. Common mistakes to avoid

- Treating `FileClose` as a commit/flush routine.
- Assuming a failed `FileOpen` clears the old context; it preserves it.
- Passing pointers to local variables to `FileGetFileInfo` and letting them go
  out of scope without unbinding.
- Treating token `0` or `0x00CB` as ordinary user-file numbers.
- Treating `FileSmashFile` as a harmless logical clear; it zero-fills storage.
- Assuming `FileDeleteChars` moves the cursor; it does not.
- Giving A1B4/A1B8 special values invented symbolic names before their original
  nomenclature is recovered.
- Assuming state bit `0x10` exists on the 2005 generation merely because it is
  accepted by NEO 2013.
- Running the A1D8 password reset as a discovery probe on real hardware.

## 17. Validation status

The contracts above are reconstructed from firmware, official callers,
System 3 internal callers/strings, and original source where applicable.  This
is enough to close the machine-level behavior described here.

Emulator/hardware execution is a separate validation layer.  Existing probe
programs are executable specifications unless their documentation explicitly
states that a given run has been completed.  No unrecorded test success should
be inferred from the existence of a probe.
