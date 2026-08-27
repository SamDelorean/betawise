# System 3 / OS3K file identity and dynamic-file block — A1E0–A1FC

This note closes the mechanically usable ABI of traps A1E0–A1FC and records the
historical genealogy of the two password-dialog entries.  Analysis used the
November 2005 AlphaSmart 3000 ROM, November 2005 NEO ROM, July 2013 NEO/System
3.15 ROM, official SmartApplet callers, and the recovered 1998–2000
`PasswordModule` / File-module sources.

Two names are promoted because the historical continuity is strong:

- A1E0 -> `PasswordVerifyFileDialog`
- A1E4 -> `PasswordChangeFileDialog`

The remaining entries are mechanically closed but retain `SYS_Axxx` names
because no reliable later System 3 public symbols have been recovered.

## 1. A1E0 — PasswordVerifyFileDialog

Modern reconstructed ABI:

```c
uint8_t PasswordVerifyFileDialog(
    uint16_t file_id,
    uint8_t try_once,
    uint8_t allow_escape,
    uint8_t allow_special_navigation,
    uint8_t allow_file_switching,
    uint16_t *selected_file_out);
```

Mechanical behavior:

- `file_id` is resolved through the shared file resolver.
- `try_once == 1` changes invalid-password handling to one failed attempt and
  exit; other values use the retry path.
- `allow_escape == 1` adds Escape to the TextBox exit-key set and enables the
  cancellation path.
- `allow_special_navigation == 1` adds the special `0xFE` exit value and the
  Applets key (`0x29`) to the interaction path.  The exact public name of the
  `0xFE` system action remains unknown; the neutral flag name intentionally
  avoids claiming more than the firmware proves.
- non-zero `allow_file_switching` enumerates eligible file indices from the
  current group, allowing the dialog to move to another descriptor and prompt
  again.
- when `selected_file_out` is non-NULL it is initialized to `0x00FF`; on an
  accepted/authorized completion it receives the canonical token of the
  descriptor that ended up selected.

The normal authorization succeeds when the entered string matches either the
master password or the selected descriptor's file password.  Escape/cancel
returns zero.  The return should therefore be treated as an accepted/authorized
boolean for normal application use, rather than as a raw `strcmp` result.

Official callers establish the six-slot ABI and the flag roles.  Representative
patterns include:

```text
AlphaWord Plus: [file_id, 1, 1, 0, 0, NULL]
AlphaWord Plus: [dynamic file_id, 0, 1, dynamic, 1, &selected]
AlphaQuiz:      [FileGetCurrentFile(), 0, 1, 1, 0, NULL]
```

Observed direct caller counts in the scanned 2012 applets:

- AlphaWord Plus: 5
- AlphaQuiz: 1
- KeyWords: 2

### Historical lineage

The recovered early AS3000 API contains:

```c
Boolean PasswordVerifyFileDialog(UInt8 iubFileNumber, Boolean iubTryOnce);
```

Its comments describe the same conceptual service: request the file password,
accept either master or file password, display invalid-password feedback, allow
F1–F8 file switching, and return TRUE/FALSE.  System 3 broadened the interface
with explicit navigation flags and an optional selected-token output.  Because
conceptual continuity is direct while ABI evolution is explicitly documented,
name confidence is B+ strong and mechanical confidence is A.

## 2. A1E4 — PasswordChangeFileDialog

Modern reconstructed ABI:

```c
int32_t PasswordChangeFileDialog(uint16_t file_id);
```

The handler resolves `file_id` and runs an interactive password-change flow.
ROM strings make the UI sequence explicit:

- `Change password for`
- `Current password:`
- `New password:`
- `Confirm new password:`
- `Current password was incorrect.`
- `New passwords do not match.`
- `A new password was not entered.`

The current file password or the master password authorizes the change.  The
new password and confirmation must match and the new password must be non-empty;
the actual per-file storage remains the five-character-plus-NUL field already
closed for A1DC.

### Important return-value rule

For a successfully resolved descriptor, the routine returns the descriptor's
canonical token on dialog exit **including cancellation**.  The return value is
therefore a token/status result, not a `password_changed` boolean.  Resolver
failure returns the resolver's raw negative status.

The historical source contains `PasswordChangeFileDialog(void)`, operating on
the implicit current file.  System 3 evolves this to an explicit token and a
status/token return.  The conceptual role remains direct, so the historical name
is promoted with the ABI difference documented.

## 3. A1E8 — create a dynamic file descriptor

```c
int32_t SYS_A1E8(
    uint8_t token_group,
    const char *name,
    uint32_t min_size,
    uint32_t requested_max_size);
```

A1E8 creates a dynamically described file in the System 3 descriptor table.
The exact original public symbol is unknown.

Mechanical contract:

- descriptor-table count >= 127 -> raw `-9`;
- `name` is normalized to at most 15 characters plus NUL;
- empty/invalid name -> raw `-13`;
- `token_group == 0` is accepted directly;
- groups 1..3 require the corresponding runtime availability state;
- group >=4 or unavailable requested group -> raw `-10`;
- duplicate name in the relevant group/class -> raw `-15`;
- a 0x48-byte descriptor is inserted in the appropriate ordered slot;
- existing descriptors/tokens are renumbered as required;
- the new descriptor's local-index field starts at `0x00FF`;
- allocation is requested with:

```text
effective_max = max(requested_max_size, min_size, 0x200)
```

- allocator maintenance/retry is attempted when necessary;
- if sufficient allocation still cannot be obtained, the provisional descriptor
  is removed and raw `-12` is returned;
- success stores `min_size` in descriptor `+0x10` and returns the canonical
  16-bit token, zero-extended.

### Descriptor +0x10 correction

A1E8 closes the meaning of descriptor offset `+0x10` as the file's
**minimum allocation / `min_size`**.  This supersedes the earlier neutral label
`accounting/reservation threshold` used while the field's origin was unknown.
The firmware diagnostic string `File %d Min Size overflow` independently
supports the same interpretation.

This correction also sharpens prior A1AC/A1B4/A1B8 accounting descriptions:
those routines compare current/recoverable allocation against `min_size`, not an
anonymous threshold.

## 4. A1EC — remove a dynamic file descriptor

```c
int32_t SYS_A1EC(uint16_t file_id);
```

This deletes the file **descriptor and its allocation**, not merely its content.

On successful resolution the handler:

1. closes the active context if the target is currently active;
2. decrements the current group's file count;
3. reclaims the target allocation into the adjacent allocator region;
4. compacts the descriptor table using 0x48-byte records;
5. moves the descriptor-table end pointer back one record;
6. fixes per-folder/group descriptor pointers;
7. renumbers affected canonical tokens;
8. performs allocator maintenance;
9. returns `0`.

Resolver failure returns the resolver status.  This operation is categorically
different from `FileDeleteChars` (range deletion) and `FileSmashFile` (erase the
content of an existing descriptor).

## 5. A1F0 — get/set the file name

```c
char *SYS_A1F0(
    uint16_t file_id,
    char *name_buffer,
    uint8_t set_name);
```

The descriptor's NUL-terminated name is stored at `+0x34` and is limited to 15
characters plus NUL.

When `set_name == 0`:

- if `name_buffer != NULL`, the descriptor name is copied into caller storage;
- success returns the internal descriptor name pointer (`descriptor+0x34`).

When `set_name != 0`:

- the supplied name is normalized/truncated to 15 characters;
- the same validation family used by A1E8 is applied;
- duplicate detection excludes the descriptor itself;
- invalid or duplicate input returns NULL and leaves the old name intact;
- success copies the normalized string to `+0x34` and returns the internal name
  pointer.

Resolver failure returns NULL.

## 6. A1F4 — get/set local file index

```c
int32_t SYS_A1F4(
    uint16_t file_id,
    uint16_t index_value,
    uint16_t *index_out);
```

The local file index is the 16-bit field at descriptor `+0x46`.

- `index_out == NULL`: set `+0x46 = index_value`;
- `index_out != NULL`: write `+0x46` to `*index_out`; `index_value` is ignored;
- success -> `0`;
- resolver failure -> resolver status.

The field is used by A1E0 when file switching is enabled and by A1F8 for reverse
lookup.

## 7. A1F8 — local index to canonical token

```c
int32_t SYS_A1F8(uint16_t local_index);
```

A1F8 searches the current file group/class for a descriptor whose `+0x46` local
index matches the argument.

- found -> canonical token from descriptor `+0x44`;
- not found -> raw `-11`.

## 8. A1FC — build and validate a token

```c
int32_t SYS_A1FC(uint8_t token_low, uint8_t token_high);
```

Equivalent mechanical operation:

```text
token = (token_high << 8) | token_low
status = resolve(token)
if resolution succeeds:
    return token
return status
```

This explicitly demonstrates that the modern file identity is a 16-bit token
with a group/class high byte, not the single-byte sequential file-number model
of the earliest AS3000 source.

## 9. Cross-ROM stability

Handler entry addresses used during reconstruction:

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 | 2013 handler length |
| --- | ---: | ---: | ---: | ---: |
| A1E0 | `0x4E2BA0` | `0x5E54FC` | `0x43B8FE` | `0x4EE` |
| A1E4 | `0x4E308E` | `0x5E59EA` | `0x43BDEC` | `0x2DE` |
| A1E8 | `0x4E1FD0` | `0x5E492C` | `0x43AD2E` | `0x40E` |
| A1EC | `0x4E250C` | `0x5E4E68` | `0x43B26A` | `0x1EE` |
| A1F0 | `0x4E26FA` | `0x5E5056` | `0x43B458` | `0x1C4` |
| A1F4 | `0x4E28BE` | `0x5E521A` | `0x43B61C` | `0x44` |
| A1F8 | `0x4E2902` | `0x5E525E` | `0x43B660` | `0x90` |
| A1FC | `0x4E2992` | `0x5E52EE` | `0x43B6F0` | `0x40` |

The handlers preserve the same logical slots and closely corresponding control
flow across all three images.  Pairwise same-position byte identity is typically
about 88–96%; the very short A1F4/A1FC handlers are essentially identical apart
from relocation/trace values.  Semantic conclusions above are based on control
flow and data use, not the percentage alone.

## 10. Safety and regression policy

- A1E0 and A1E4 are interactive and should first be regression-tested in the
  emulator with disposable password state.
- A1E8/A1EC modify descriptor-table topology and allocator state.  Test creation,
  duplicate rejection, allocation failure and deletion/renumbering in an
  emulator snapshot before physical-hardware experiments.
- A1F0 renames files and A1F4 mutates indexing metadata; use disposable files.
- A1F8/A1FC can be exercised as read/validation operations once a known descriptor
  set exists.

No emulator or hardware regression is claimed as executed by this closure.
Firmware, official callers and historical source close the ABI contracts; runtime
execution remains the next verification layer.
