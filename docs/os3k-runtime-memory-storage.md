# OS3K runtime memory and file/storage map

Canonical base-analysis consolidation, 2026-09-12. This map organizes existing
project evidence for OS3K, emulator and application work; it adds no ABI contract
or SDK declaration. Scope is AS3000 System 3 November 2005, NEO November 2005
(“early NEO” below), and NEO/System 3.15 July 2013. Earlier 1998–2000 AS3000
source is a separate historical layer; no AS2000 behavior is established here.

## Evidence and authority

The [ABI index](abi-reconstruction-index.md) and
[2026-09-11 consolidation](os3k-abi-consolidation-2026-09-11.md) remain authoritative.
Their inventory remains 208 `CERRADA`, 56 `CERRADA_CON_LÍMITE_DE_EVIDENCIA`,
21 `NO_SYSCALL/NO_CALLABLE`, and zero `PENDIENTE_REAL`. Unresolved RAM ownership
or physical persistence does not reopen that closed syscall inventory.

Labels used below describe individual claims, independently of inventory status:

- **M — mechanical/direct:** firmware/source observations reported by existing
  closures. This iteration reviewed those documents, not fresh firmware traces.
- **H — historical/continuity:** strong genealogy or cross-version correlation;
  not proof of identical structures, symbols or physical hardware.
- **I — implementation inference:** proposed consequence for consumers, not an
  observed firmware guarantee.
- **U — unresolved:** the reviewed evidence does not establish the claim.

A mechanically understood routine can retain a source-level evidence limit.
Neutral `SYS_Axxx` names, unknown flag names and unknown return meanings stay
unchanged. Nothing here promotes A468/A46C or another limited-evidence entry.
“Persistent binding” in File API notes means retained across calls; it does not
mean retained across loss of power. Static regression results cited by closures
are prior results, not tests rerun in this iteration; their pending dynamic
regressions remain pending.

The [current File API reference](file-api-current-reference.md) takes precedence
over the chronological [reconstruction log](file-api-reconstruction.md).
In particular, `+0x10` is now `min_size`, A1C4 returns storage rather than a
descriptor, and A1C4 outputs are retained bindings rather than one-shot outputs.
The source-first [A1B4 closure](sys-a1b4-size-accounting-source-first-closure.md)
also preserves resolver-derived failure results; the old log's blanket zero
failure claim is not carried into this map.

## 1. Located runtime state and RAM boundaries

These are CPU runtime addresses, not offsets in a ROM package. A pointer-table
address locates the table, not necessarily the memory class of its pointees.
The map is deliberately partial: known objects do not establish ownership of
all intervening bytes.

| Object / anchor | AS3000 2005 | NEO 2005 | NEO 2013 | Evidence |
| --- | --- | --- | --- | --- |
| Initial stack pointer | `0x0003FFF0` | `0x0007FFF0` | `0x0007FFF0` | M, [structural frontier](os3k-structural-frontier-post-common-runtime.md) |
| 32 applet-header pointers | `0x00000D82` | `0x00000DDA` | `0x00000E0A` | M, [applet runtime](applet-runtime-api-closure.md) |
| Active file descriptor pointer global | `0x00003442` | `0x0000349A` | `0x000034D2` | M, [open/close](fileopen-fileclose-closure.md) |
| Currently executing applet index | `0x00003552` | `0x000035AA` | `0x000035E2` | M, [A278](applet-current-settings-closure.md) |
| Four token-group name slots, 30 bytes each | `0x00003D7A` | `0x00003DD2` | `0x00003E0A` | M, [A254](password-token-group-runtime-closure.md) |
| Group-state byte / current-group flags byte | `0x3DF2 / 0x3DF3` | `0x3E4A / 0x3E4B` | `0x3E82 / 0x3E83` | M, A254 |
| Password protection byte / master-password buffer | `0x4472 / 0x4473` | `0x58A4 / 0x58A5` | `0x118A2 / 0x118A3` | M, [password runtime](password-token-group-runtime-closure.md) |

**H:** historical AS3000 initialization places RAM at zero and Flash at
`0x00400000`. **M:** the stack anchors above occur in the compared entry
scaffolds. **U:** these facts alone do not close installed RAM size, mirrored
banks, stack lower limits, heap/file-arena boundaries, retained-power domains,
or the exact cold/warm initialization map for each generation. The NEO stack
value is not a file-capacity measurement.

Other closed runtime-owned structures include the 32-entry longword
[selection-block mask table](applet-selection-mask-closure.md), the separate
[selected-applet context](applet-selection-closure.md), and per-applet A5
contexts. A244 saves/restores A5 and the executing-app index around dispatch
(**M**); preserving OS ownership of A5 is an application constraint.

The [selection/display backing store](sys-a03c-selection-backing-store-closure.md)
has row stride 40 on AS3000 and 132 on both NEO generations (**M**). A03C writes
one backing cell without display I/O or cursor advance. This is a display object,
not evidence of file storage. A278 computes a settings pointer from header
`settingsOffset` (`+0x0C`), with a special System-base rule (**M**); a settings
pointer does not establish writable RAM, retention or a settings commit API.

## 2. File descriptor layout, ownership and lifetime

**M**, from the [current reference](file-api-current-reference.md),
[identity/dynamic closure](file-identity-dynamic-closure.md), and
[A1C4 closure](filegetfileinfo-a1c4-source-first-closure.md): the runtime table
uses `0x48`-byte descriptors, with the following identified fields.

| Offset | Field | Width / established use |
| --- | --- | --- |
| `+0x00` | storage/base pointer | 32-bit pointer to file bytes |
| `+0x04` | current logical size | 32-bit byte count |
| `+0x08` | previous/recoverable size | 32-bit extent/high-water state |
| `+0x0C` | maximum allocated capacity | 32-bit `max_size` |
| `+0x10` | minimum allocation | 32-bit `min_size` |
| `+0x14` | current cursor | 32-bit byte position |
| `+0x18` | password | at most five characters plus NUL |
| `+0x20` | per-file state flags | masks known; semantic bit names open |
| `+0x24` | storage-pointer mirror binding | caller address |
| `+0x28` | current-size mirror binding | caller address |
| `+0x2C` | maximum-size mirror binding | caller address |
| `+0x30` | cursor mirror binding | caller address |
| `+0x34` | filename | at most 15 characters plus NUL |
| `+0x44` | canonical token | 16 bits |
| `+0x46` | local index | 16 bits |

Unlisted gaps are not assigned a meaning. This is a private implementation map,
not a stable public C structure or an on-disk descriptor format.

A1E8 rejects a table count at least 127, inserts a descriptor, initializes local
index to `0x00FF`, allocates storage and returns a canonical token. Failure to
allocate undoes provisional creation. A1EC removes a descriptor and its
allocation, closes it if active, compacts the table, repairs group pointers and
renumbers affected tokens. Closing a context does not destroy its descriptor.
These are **M** lifetime/topology facts, not a proven startup/shutdown lifetime.

**I:** consumers must not cache internal descriptor/name addresses or treat
tokens as immutable identities across creation/removal. **U:** exact table
base/end initialization, relocation rules for every allocation path, and mirror
cleanup on all teardown paths still need a dedicated ownership map.

## 3. Storage pointers and live bindings

**M:** A1C4 returns descriptor `+0x00` and registers four optional caller
addresses in `+0x24..+0x30`; synchronization writes current values through them
immediately and on later File API mutations. Passing four NULL outputs
unregisters those bindings. Failure leaves existing bindings untouched.
`FileClose` neither unregisters nor synchronizes them.

File data is accessed through storage plus cursor/size offsets by the closed
read/write/delete operations. Capacity growth and allocator maintenance are
part of this runtime model. **I:** refresh storage references after operations
that can change allocation; keep registered mirror objects alive and unbind
before their scope ends. A one-time raw pointer is not a lifetime guarantee.

**U:** a RAM-addressed storage pointer does not identify the physical persistent
medium, a separate durable copy, or a commit mechanism. Neither pointer return
nor direct byte copy proves battery-backed retention, flash write-through,
filesystem sectors, or survival of reset/power loss.

## 4. Namespace, active descriptor and applet context

**M**, [A1C0](filesetfolder-a1c0-source-first-closure.md),
[open/close](fileopen-fileclose-closure.md), [A254](password-token-group-runtime-closure.md):

- `FileSetFolder` selects a runtime applet index `0..31`, not an applet ID.
  Success detaches the active descriptor and returns the previous folder.
  Failures `-64` and `-7` preserve folder and active descriptor; optional flags
  output is first cleared. Header flag `0x40` bypasses the empty-group check.
- Ordinary 16-bit tokens resolve in that namespace. Token zero denotes the
  active descriptor; `0x00CB` denotes the dedicated clipboard descriptor.
- A1FC constructs/validates `(high << 8) | low`. A254 queries current/explicit
  token group and optional name; selector zero uses current flags masked by
  `0x7F`, explicit slots are 1..4. This does not equate its selector domain with
  A1E8's creation validation (group zero, conditionally 1..3).
- A1F4 gets/sets local index `+0x46`; A1F8 searches the current context and returns
  a canonical token. Local index, token, applet ID, runtime folder, selected
  applet and currently executing applet are distinct state/identity concepts.

## 5. What FileOpen and FileClose do

**M**, [lifecycle closure](fileopen-fileclose-closure.md): resolution failure in
`FileOpen` returns zero before detaching the prior active descriptor. Success
runs descriptor/open-state preparation, installs the resolved descriptor,
rewinds only when `reset_position == 1`, and returns its canonical token.
The byte argument is not an access/create mode. The preparation path must not
be expanded into a guessed load-from-disk or commit protocol.

`FileClose` only stores NULL in the active-descriptor global and returns. It has
no meaningful return value, flush, erase, cursor reset or mirror unbinding.
**H:** the 2000 source has current-file/restore/smash/info concepts but no literal
`FileOpen`, `FileClose` or `FileSetFolder` functions; modern ABI identity is not
projected backward.

## 6. Immediate effects of file operations

“Immediate” here means the documented handler path mutates runtime storage or
state before returning; it makes no physical durability or atomicity claim.
All mechanical rows below are **M**.

| Operation | Data effect | Runtime/state effect and boundary |
| --- | --- | --- |
| [A198 FileWriteBuffer](filewritebuffer-a198-source-first-closure.md) | mode 1 appends; 2 shifts suffix and inserts; 3 overwrites | Partial-write count; growth attempted when needed for active descriptor; capacity clamp; size/high-water and mirrors updated; modes 2/3 advance cursor; invalid mode `-5` |
| [A19C FileReadBuffer](filereadbuffer-a19c-source-first-closure.md) | Copies bytes to caller, no NUL insertion | Clamps at EOF, advances cursor and synchronizes mirrors; does not change file bytes |
| [A1A4 FileDeleteChars](filedeletechars-closure.md) | Moves suffix over deleted bytes | Clamps to remaining bytes, reduces size, preserves cursor/high-water; leaves trailing bytes |
| [A1A0 FileSmashFile](file-core-operations.md) | Zero-fills entire `max_size` allocation | Clears size/high-water/cursor and syncs mirrors; descriptor survives |
| [A1B8](sys-a1b8-active-file-resize-state-source-first-closure.md) | No payload copy/initialization established by logical resize | Sets clamped size, clamps cursor, raises high-water on growth, syncs mirrors; not a save/flush |
| [A1E8 / A1EC](file-identity-dynamic-closure.md) | Allocate/reclaim storage | Create/remove descriptor and repair table/token topology |
| A1F0 / A1F4 / A1DC / A1D0 | Mutate name/index/password/flags metadata | No durable metadata commit established; [reference](file-api-current-reference.md) |
| A1A8 / A1B0 / A1B4 / A1BC / A1AC | No data mutation | Query token, size/capacity or accounting; special selectors retain distinct meanings |
| A1C0 / A1C8 / A1CC / A1C4 | No user-payload write established by context/binding contract | Namespace/open/close/binding effects described above; allocation preparation is not a durability guarantee |

A1B8 numeric commands remain neutral: `-2` requests previous size; `-3`
snapshots current into previous; `-4` snapshots all descriptors only with no
active descriptor; `-5` sets `min_size=max(current_size,0x200)`; `-6` requests
`max(previous_size,current_size+40)` before clamping. There is no special `-1`
branch: unsigned clamping makes it select maximum size. The active-descriptor
`-4` path skips the global snapshot and retains an auxiliary resolver result.
None of these size snapshots copies old file contents; **I:** high-water state
must not be presented as transactional undo or a durable backup.

## 7. Allocation units, minimum, capacity and reclaimable space

**M**, [space accounting](file-space-accounting.md): allocation accounting uses
`0x200` (512-byte) units. A1E8 requests
`max(requested_max_size,min_size,0x200)`; the supplied `min_size` is stored
separately. This does not prove that every arbitrary requested capacity is
returned unchanged or that 512 bytes is a physical sector size.

For an ordinary resolvable token, A1B0 returns `max_size-current_size`, A1BC
returns `max_size`, and A1B4 returns `current_size`. Thus
`A1B4(token)+A1B0(token)==A1BC(token)` for this ordinary path. `max_size` is the
current per-descriptor allocation, not a universal file-size/device maximum.

A1AC combines three components:

1. Clipboard slack: round `max_size-previous_size` down to a multiple of 512;
   count it only if the rounded amount is greater than 512.
2. The raw unallocated tail between the last allocation end and global
   end-of-file-storage pointer.
3. Relevant ordinary descriptor slack: round
   `max_size-max(previous_size,min_size)` down to 512; again count only amounts
   greater than 512. With an active descriptor use that descriptor's path;
   without one walk the descriptor table, excluding the separately counted
   clipboard.

Consequently the metric depends on active context and is not a plain sum of
unused capacities or necessarily itself a multiple of 512. A1B4 selectors
`0xFC/FD/FE/FF` respectively expose active previous size, current-group size sum,
current-group `min(current_size,min_size)` sum and table-wide equivalent sum.
**U:** a total file-storage arena size, all fragmentation/growth policies, and a
physical capacity/retention map per device are not closed by these formulas.

## 8. Clipboard relationship to file storage

**M**, [buffer closure](clipboard-buffer-closure.md),
[state/capacity closure](clipboard-state-capacity-closure.md), and
[edit closure](clipboard-edit-closure.md): clipboard is a dedicated descriptor
with the common storage/size/capacity/cursor fields, shared allocator and token
`0x00CB`. Its tag is a separate byte, not a proven Boolean or named text enum.
A220 returns storage, A224 capacity, A21C raw or filtered length, A218 tag.

A200 copies an active-file range into clipboard without changing source cursor
or bytes. A204 then deletes only the actual copied count. A208 pastes via the
A198 write-mode family. ClipboardSet copies caller bytes, limits to resulting
capacity and sets tag plus current/previous size. ClipboardGet raw mode copies
at most current size. ClipboardClear clears current/previous size, cursor and
tag and performs allocator maintenance without explicitly zeroing data.
A228 adjusts allocation and sets clamped current/previous size without copying
payload, changing tag or explicitly moving cursor; expansion is not initialization.

Selector 3 is the control-code-aware filtered path; other read variants use
raw data. In AS3000/NEO 2005, A210 passes an uninitialized local as output limit
and caller count as source length. NEO 2013 initializes the output limit from
count and uses clipboard current size as source length. Filter mappings also
vary by generation. A21C initializes its limit in all three versions.
**U:** clipboard survival across reset/power loss is not established by this
shared allocator relationship.

## 9. Record-oriented structures

**M**, [A27C cursor](packed-record-cursor-closure.md),
[A280 writer](packed-record-writer-closure.md), [A284 search](packed-record-search-closure.md):
records contain 16-bit neutral `field_0`, `field_2`, and payload count at offsets
0, 2, 4; payload begins at 6. These helpers touch caller memory and cursors,
not OS globals. Their existence does not identify an OS file format, directory
record, spreadsheet cell format or persistence layer.

Let `R=((payload_size+1)&0xFFFE)` in **16-bit arithmetic**. A27C always returns
its four outputs and updates the cursor to `record+6+R` only if R is nonzero;
size zero and `0xFFFF` leave the original cursor unchanged. A280 writes the
three header words and exact payload, leaves odd padding untouched and always
sets the cursor to `record+6+R`; size `0xFFFF` copies 65535 bytes but advances
only six. A284 uses first-field zero as terminator, finds the first matching
pair, advances past a match, and returns 0; failure returns -1 with cursor at
the terminator. Its skip rule always includes six, including wrapped sizes.

No helper provides a bounded parser/serializer or alignment/overlap validation.
A280's payload copy is forward-only. **I:** application serialization must
validate sizes and bounds independently, and must not blindly iterate A27C on
zero-sized records. A280 does not establish automatic terminator creation.

## 10. Runtime lifetime versus physical persistence

| State | Established lifetime/effect | Retention boundary |
| --- | --- | --- |
| Active descriptor / current namespace | M: explicit context changes; close clears active pointer | U: cold/warm boot initialization and retention policy |
| Mirror addresses / cursor / size / high-water | M: survive ordinary calls, with defined mutations; close leaves them | U: reconstruction or retention across restart/power loss |
| File bytes and descriptor metadata | M: directly updated through storage/descriptor operations | U: physical medium, flush timing, power-failure consistency |
| Clipboard bytes / tag | M: shared descriptor plus separate tag; logical clear differs from erase | U: persistence policy |
| A5 / executing-app index | M: temporarily switched and restored by A244 | No durable-storage contract follows |
| Master password / protection byte | M: runtime access and documented cold/default initialization | U: complete restore/save path and physical retention |

“Runtime state” does not by itself prove physically volatile memory: even
runtime-owned RAM may have a retention mechanism. Conversely, state retained
across calls is not evidence of power-cycle durability. The reviewed file
closures establish neither an ordinary file commit primitive nor proof that
one is unnecessary at another layer.

There is separate evidence of programming transactions in
[Manager applet installation](os3k-manager-applet-installation-transaction.md):
principal-OS installation uses staged block transfer/program/finalize, and NEO
Small ROM OS update uses destination-map/erase setup and a related block engine.
The note distinguishes direct firmware findings from comparative host-source
packing and still-open allocator/rollback policy. These are installation/update
contexts; their `0x0B` program and `0x07` finalize operations are **not evidence
that ordinary FileWriteBuffer data is committed through those commands**.
External neo-re implementations remain comparative references.

Still unresolved: retained RAM versus copied backing storage; power domains;
file-arena initialization and reconstruction; dirty tracking; reset/sleep/off
transitions; write ordering, atomicity, torn-write recovery and checksums;
which file/clipboard/metadata state survives each transition. No physical medium
or storage size is guessed to fill these gaps.

## 11. Cross-generation boundaries

| Topic | AS3000 2005 | Early NEO 2005 | NEO 2013 |
| --- | --- | --- | --- |
| Core descriptor/file lifecycle | Shared compared mechanics | Shared compared mechanics | Shared compared mechanics, relocated globals |
| A1D0/A1D4 supported mask | `0x0D` | `0x0D` | `0x1D`; extra bit meaning unnamed |
| A210 filtered read | Uninitialized output-limit local | Same defect | Initialized limit and corrected source length |
| Display backing stride | 40 | 132 | 132 |
| Initial stack anchor | `0x3FFF0` | `0x7FFF0` | `0x7FFF0` |
| Packed-record helpers | Shared mechanics/edge cases | Shared mechanics/edge cases | Shared mechanics/edge cases |
| Manager installation finalizer extent | `0x284` | `0x284` | `0x290`; added purpose unresolved |

These are **M** comparisons supported by the linked closures, not a claim that
all RAM layouts or persistence hardware match. The early AS3000 2000 source
uses fixed per-file capacities and a simpler byte file-number model (**H**);
it cannot supply modern folder/token/allocator contracts. AS2000 and unexamined
NEO revisions require their own evidence before any transfer of these findings.

## 12. Cross-project handoffs (implementation inference)

These recommendations are **I**, with no code changes or completed runtime tests
claimed in this iteration.

**Emulator:** preserve firmware-specific RAM coordinates, descriptor topology,
active context, cursor/high-water distinctions, live guest-pointer writes,
clipboard sharing and partial-write behavior. Regress failed-open preservation,
close-only detach, table compaction, allocator thresholds and record edge cases
with synthetic disposable state. Test 2005 filtering separately from 2013.
A host save-state can preserve emulated RAM for testing, but is not evidence of
the device's physical persistence. Do not attach a fabricated disk flush to
A1CC or substitute installation programming for ordinary file writes.

**BetaWise:** use existing interfaces and keep A5 reserved. Select the intended
runtime namespace, check open success, check actual read/write counts, explicitly
unbind mirrors and restore context. Reopening is required if restoring an
active file is intended: restoring a folder alone detaches rather than restores
that active descriptor. Avoid retained raw pointers/tokens across topology
changes. SDK header work belongs to its separate publication front.

**Spreadsheet / BetaCalc / NeoSheet:** define a versioned application byte format
with explicit lengths and bounds, independent of unknown OS record-field names.
Separate the in-memory workbook from its serialized representation. Estimate
space using per-file capacity and contextual allocator metrics; handle partial
writes and allocation failure. Stage/validate serialization before destructive
replacement when resources permit, and read back the written bytes to verify
runtime content. Readback and close do not prove durability. Atomic replacement,
power-loss recovery and a user-visible durable “saved” guarantee remain design
questions until the backing-store lifecycle is established. Do not use clipboard
or previous-size state as a proven durable backup.

## 13. Recommended next bounded frontier

Trace **initialization and ownership of the file-storage arena**, starting from
the already identified storage-end pointer and descriptor-table roots used by
A1AC/A1E8/A1EC. Recover their initialization writers and exact ranges for one
canonical generation first (AS3000 2005), distinguishing cold/default and
warm/resume paths only where directly demonstrated. Record a small address,
writer, lifetime and evidence table; compare NEO only after that first unit is
closed. Do not reopen the public syscall contracts.

Success would locate descriptor/storage boundaries and identify the next actual
retention/restore dependency. It would not, by itself, settle physical medium
or power-loss durability. Subsequent hardware/emulator handoffs should target
that concrete dependency with synthetic disposable data, publishing only
independently derived documentation and test scaffolding.
