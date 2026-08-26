# System 3 / OS3K File API reconstruction

This document tracks reverse-engineered behavior of the System 3 file ABI used by AlphaSmart 3000 and NEO/NEO2. It records only behavior supported by firmware analysis and official SmartApplet callers; provisional names remain explicitly marked.

## Evidence status

Confidence **A** means the behavior is established directly by firmware analysis or equivalent primary evidence. Historical BetaWise names are retained only when they match the observed behavior.

## Historical-source audit

The original AS3000 `FileModule.c` from 2000 was rechecked directly while auditing continuity between the `betawise` and `Betawise 2` research threads. It contains `FileGetCurrentFile`, `FileSetCurrentFile`, `FileRestoreOldFile`, `FileClearFile`, `FileSmashFile`, and `FileGetFileInfo`. It does **not** contain functions literally named `FileOpen`, `FileClose`, or `FileSetFolder`.

An earlier working-note statement that the historical file contained `FileOpen`/`FileClose` wrappers was therefore incorrect and is intentionally not carried forward. The later System 3 names must continue to be established from firmware and real callers rather than projected backward onto the 2000 source.

## Descriptor fields identified so far

| Offset | Current interpretation | Confidence |
| --- | --- | --- |
| `+0x00` | storage/base pointer | A |
| `+0x04` | current file size | A |
| `+0x08` | previous/recoverable size | A |
| `+0x0C` | maximum size/capacity | A |
| `+0x10` | accounting/reservation threshold; exact public meaning still open | A for mechanical use |
| `+0x14` | current cursor/file position | A |
| `+0x24` | stored pointer to caller's storage-pointer mirror | A |
| `+0x28` | stored pointer to caller's current-size mirror | A |
| `+0x2C` | stored pointer to caller's maximum-size mirror | A |
| `+0x30` | stored pointer to caller's cursor-position mirror | A |
| `+0x44` | canonical 16-bit descriptor/file token | A |

The historical AS3000 `FileModule.c` independently describes the same conceptual model for maximum size, current size, previous size, cursor position and backing storage. Modern OS3K adds fields and accounting behavior that should not be projected back into the 2000 structure without evidence.

## A1C8 — provisional `FileOpen`

Current research prototype:

```c
uint16_t SYS_A1C8(uint16_t file_id, uint8_t reset_position);
```

This prototype is **not yet promoted into `os3k.h`**.

Observed behavior on AS3000 System 3 (Nov 2005), NEO System 3 (Nov 2005), and NEO System 3 (Jul 2013):

1. Resolve the 16-bit token to a runtime descriptor.
2. Clear the previously active descriptor through A1CC.
3. Install the resolved descriptor as the active file/context.
4. Perform descriptor preparation.
5. If the second argument is exactly `1`, set `descriptor+0x14` (current position) to zero.
6. Return the zero-extended 16-bit value from `descriptor+0x44`.
7. Resolution failure returns `0`.

The second argument is only compared against `1`; no other use of it was found in the handler. Official callers examined so far use only `0` and `1`.

### Official callers examined

Twenty-five real A1C8 callers were traced in official 2012 SmartApplets:

- AlphaWord Plus 3.4: 4
- Calculator 3.0: 1
- ControlPanel 1.0: 1
- KeyWords 3.6: 9
- AlphaQuiz 1.0: 10

Observed second-argument distribution: **17 calls with `1`, 8 calls with `0`, no other values**.

The 2005 Calculator and ControlPanel binaries also contain the exact call pattern:

```c
A1C8(0x00CB, 1);
```

## Reserved token `0x00CB`

`0x00CB` is a reserved token for the **clipboard descriptor**.

The A1C8/A1C4 resolver compares the requested token explicitly against `0x00CB` and, on match, takes a dedicated path to a global descriptor instead of performing the ordinary lookup. The already-identified `ClipboardSet`, `ClipboardGet`, and `ClipboardClear` syscalls operate on this same descriptor, closing the identification.

## Token `0`

Token `0` resolves to the **currently active descriptor**. This explains why several neighboring file syscalls can operate without an explicit file ID: they consume the context selected by A1C8.

## A1CC — provisional `FileClose`

A1CC consumes no arguments and clears the global active-descriptor pointer:

```c
active_descriptor = NULL;
```

The historical BetaWise name `FileClose` is therefore strongly supported, but public SDK promotion is being kept together with A1C8 until the neighboring API family is closed.

## A1A0 — inherited comment is wrong

Upstream BetaWise currently comments A1A0 as:

```c
SYS_A1A0 // get current file number?
```

Firmware analysis contradicts that interpretation. A1A0 operates on the active descriptor, clears backing storage and resets multiple descriptor fields. It is much closer to a destructive reset/smash operation than a getter. The historical AS3000 source contains a `FileSmashFile()` concept with matching core semantics, but the final public OS3K name remains provisional pending full caller correlation.

## A1B4 — size/accounting query family

For an ordinary 16-bit file token, A1B4 resolves the descriptor and returns `descriptor+0x04`, the current file size. Resolution failure returns zero.

The handler also has four special selectors. Their **mechanical behavior is confirmed**, while the public accounting names for `0xFD`–`0xFF` remain intentionally unset:

| Selector | Observed operation |
| --- | --- |
| `0xFC` | resolve token `0` (active descriptor) and return `+0x08` previous/recoverable size |
| `0xFD` | sum `+0x04` current sizes across the current applet/file group |
| `0xFE` | sum `min(+0x04 current_size, +0x10)` across the current applet/file group |
| `0xFF` | sum `min(+0x04 current_size, +0x10)` across the full descriptor table |

The descriptor stride used by these loops is `0x48` bytes in the analyzed 2005 firmware.

Official AlphaWord Plus callers include direct uses of `0xFC` and `0xFF`, and several applets query `0x00CB` through A1B4 to obtain clipboard size. This confirms that the argument is a shared token/selector namespace, not merely a sequential user-file number.

## A1B8 — active-file resize/state operation

A1B8 resolves token `0`, so it operates on the active descriptor selected by A1C8. Its argument is a signed/command-sized 32-bit value, but ordinary non-negative values are interpreted as a requested file size.

### Ordinary resize

```text
new_size = min(requested_size, max_size)
current_size = new_size
if cursor > new_size:
    cursor = new_size
if new_size > previous_size:
    previous_size = new_size
```

For this path, the returned value is the effective size after clamping.

The same mechanics are present in the November 2005 AS3000 and NEO handlers.

### Special command values

The handler recognizes several negative values before the common resize path:

| Value | Mechanical effect |
| ---: | --- |
| `-2` | use `+0x08` previous/recoverable size as the requested size, then run the common resize path |
| `-3` | copy current size `+0x04` into previous/recoverable size `+0x08`, keep the effective requested size equal to current size, then run the common path |
| `-4` | global snapshot path: when the active-descriptor global is clear, iterate the full descriptor table and copy each `+0x04` current size into `+0x08`; this path returns zero |
| `-5` | set `+0x10 = max(current_size, 0x200)` and keep the effective requested size equal to current size |
| `-6` | choose `max(previous_size, current_size + 40)` as the requested size, then run the common clamp/update path |

`-1` has no dedicated branch. Under the ordinary unsigned maximum-size clamp it becomes `max_size`; this is mechanically equivalent to exposing the entire allocated file capacity, but it is not yet published as a named SDK command.

The `-2` behavior has a strong historical relationship to `FileRestoreOldFile()`. The destructive A1A0 path similarly resembles historical `FileSmashFile()`. These genealogical matches are useful evidence, but modern OS3K cursor/accounting details differ enough that the old function names should not be assigned solely by analogy.

### Official caller evidence

Real A1B8 callers were found in AlphaWord Plus, AlphaQuiz and KeyWords. They include:

- a direct `-2` call in AlphaWord Plus;
- direct ordinary sizes such as `0`, `0x74`, and `0x290`;
- dynamic values supplied from registers or descriptor/app state.

No direct SmartApplet constant call to `-3`, `-4`, `-5` or `-6` has yet been established. Their mechanics are nevertheless explicit in the firmware handler and therefore can be documented without assigning public semantic names.

## A1C0 — `FileSetFolder`

The inherited `FileSetFolder` name is now supported strongly enough to publish. Its mechanical contract is confidence **A** in the analyzed AS3000/NEO 2005 firmware and was independently rechecked against NEO System 3 from July 2013.

Public SDK prototype:

```c
int32_t FileSetFolder(uint32_t applet_index, uint32_t *applet_flags_out);
```

The second pointer is optional.

### What `folder` means

The first argument is a **runtime SmartApplet index**, not an applet ID and not a user-file number. The handler reads the complete 32-bit argument and accepts only the unsigned range `0..31`.

Official callers prove the mapping directly. AlphaQuiz and ControlPanel call `AppletFindById(0xA000)`, retain the returned runtime index for AlphaWord Plus, and pass that value to A1C0. `AppletFindById` walks the same 32-entry applet-header pointer table and compares `AppletHeader_t.id` at header offset `+0x14`.

A1C0 therefore selects the file namespace/folder owned by the selected runtime SmartApplet.

### Exact validation path

The NEO 2013 handler makes the previously generic validation language precise:

1. If `applet_flags_out != NULL`, initialize `*applet_flags_out = 0`.
2. Reject an `applet_index >= 32` with return `-64`.
3. Consult the 32-word runtime applet-ID table. If the slot contains `0xFFFF`, reject it with return `-64`.
4. Resolve the corresponding `AppletHeader_t` from the 32-entry header-pointer table.
5. Test bit `0x40` of the low byte of `AppletHeader_t.flags` (`header+0x13`, bit 6).
6. If that bit is clear, inspect the current runtime file-group record for the selected applet. Its byte at `group+0x04` is the group's file count; A1B4 independently uses the same byte as its enumeration loop bound. If this count is zero, reject the selection with return `-7`.
7. If validation succeeds, call A1CC so that no file descriptor from the old folder remains active.
8. Save the previous global folder index.
9. Store `applet_index` as the new current folder.
10. If `applet_flags_out != NULL`, copy the complete 32-bit `AppletHeader_t.flags` field (`header+0x10`) to the caller.
11. Return the previous folder index.

The low-byte flag `0x40` is therefore a confirmed bypass of the normal non-zero runtime-file-count requirement. Its final public symbolic name is intentionally **not** guessed yet. AlphaWord Plus provides an important real example: its 2012 header has flags `0xFF0000CE`, so bit `0x40` is set even though its header `fileCount` byte is zero.

The raw negative returns are also documented without invented symbolic names: `-64` covers an out-of-range/uninstalled runtime applet slot, while `-7` covers a selected applet with no eligible runtime file group when the `0x40` bypass flag is absent.

### Official caller behavior

Six A1C0 callers were traced in the 2012 AlphaQuiz binary. They all pass `NULL` for `applet_flags_out`. A representative sequence is especially useful for the contract:

```text
index = AppletFindById(0xA000)
previous = FileSetFolder(index, NULL)
... operate in AlphaWord Plus file namespace ...
FileSetFolder(previous, NULL)
```

The first call's D0 value is explicitly saved and later supplied as the first argument to another A1C0 call, proving that the success return is the **previous folder index**, not merely a success status.

ControlPanel contains a non-NULL second-argument caller. After `FileSetFolder`, it directly tests bit 6 of the low byte of the returned 32-bit flags value. This independently confirms that the second argument is an `AppletHeader_t.flags` output rather than an opaque status structure.

### Public SDK decision

`FileSetFolder` is the first member of the reconstructed modern File API considered sufficiently closed for an `os3k.h` prototype. The implementation stub was already present as syscall A1C0; publishing the prototype does not rename an unknown trap.

No public constants are added yet for `-64`, `-7`, or flag `0x40`, because their mechanics are established but their original symbolic names have not been recovered.

## A1C4 — live file-info binding

A1C4 uses the same file-token resolver as A1C8 and consumes **five argument slots**: a 16-bit file token plus four pointer values.

Mechanical research prototype:

```c
void *SYS_A1C4(
    uint16_t file_id,
    uint8_t **storage_out,
    uint32_t *current_size_out,
    uint32_t *max_size_out,
    uint32_t *cursor_out);
```

The public semantic name remains provisional, but the ABI and the meaning of all four pointer arguments are confidence A.

On successful resolution A1C4 stores the caller pointers in descriptor offsets:

```text
+0x24 = storage_out
+0x28 = current_size_out
+0x2C = max_size_out
+0x30 = cursor_out
```

It then immediately invokes a synchronization helper. That helper performs:

```text
if storage_out      != NULL: *storage_out      = descriptor->storage
if current_size_out != NULL: *current_size_out = descriptor->current_size
if max_size_out     != NULL: *max_size_out     = descriptor->max_size
if cursor_out       != NULL: *cursor_out       = descriptor->cursor
```

The helper is byte-for-byte logically equivalent in AS3000 2005 and NEO 2005.

### Persistent binding, not one-shot output

These four addresses remain stored in the descriptor. The same synchronization helper is called by multiple neighboring file operations, including write/read/state-changing paths and A1B8. Consequently the arguments are **live mirror bindings**: while installed, System 3 keeps the caller variables synchronized with changes to the backing file descriptor.

This also explains why many official SmartApplets call A1C4 with four NULL values: they do not need live mirrors. Internal System 3 callers can provide real storage/size/capacity/cursor destinations when they need continuously synchronized state.

### Historical relationship

The AS3000 source from 2000 contains `FileGetFileInfo()`, which returns the storage pointer and outputs current size, cursor and maximum size. A1C4 is clearly related to that information family, but the modern ABI adds an explicit file token and persistent pointer binding. Therefore the historical function name is useful genealogy but is not yet assigned automatically to A1C4.

## Current naming decision

`FileSetFolder` is now SDK-ready and can be exposed in `os3k.h`. A1C4's ABI is closed but its final public name is still held back. The observed A1C8/A1CC behavior strongly supports the inherited `FileOpen` / `FileClose` names and conflicts with treating A1C8 as a simple scalar-property query.

The remaining work before publishing the rest of the File API as a coherent SDK family is mainly semantic naming of A1A0/A1B4/A1B8/A1C4 and validation of how the storage accounting selectors are presented by AlphaWord.

## Next work

1. Add the confirmed `FileSetFolder` prototype to `os3k.h` without inventing error/flag constants.
2. Correlate A1B4 `0xFD`–`0xFF` with the AlphaWord storage/status UI before assigning accounting names.
3. Find real non-NULL A1C4 callers and determine whether an original System 3 name can be recovered.
4. Revisit A1A0 against official destructive-delete/recovery callers.
5. Define emulator regressions for A1C8 selection/reset, A1C0 folder switching, A1C4 live mirrors and A1B8 size/recovery commands.
