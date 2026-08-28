# System 3 / OS3K ABI reconstruction index

This is the **canonical navigation and traceability index** for reconstructed
System 3 / OS3K interfaces in this repository. It answers two questions that
must remain separate:

1. **What is the current contract that a developer may rely on?**
2. **Where is the evidence and research history that produced that contract?**

The older `file-api-reconstruction.md` is intentionally retained as a
**chronological research log** and contains provisional statements later
superseded by stronger evidence. When a closure/current-reference document
exists, use it for the current SDK contract.

## Evidence and naming policy

- **Mechanical confidence A**: argument width/order, return behavior, state
  mutation or control flow is established directly from firmware or equivalent
  primary evidence.
- Historical/name confidence is tracked separately from mechanical confidence.
- A recovered historical AS3000 name is reused only when later System 3 clearly
  implements the same conceptual API member; ABI evolution is documented.
- When behavior is closed but the original modern name is not, the interface
  remains `SYS_Axxx` rather than receiving a descriptive guess.
- A regression specification is not evidence that the test has been run.

Each closed block documents purpose, prototype, arguments/returns, state effects,
errors/sentinels, cross-ROM differences, caller/source evidence, safety,
validation status and concrete audit references.

## Current reconstructed blocks

| Trap/range | Current identity | Status | Canonical documentation |
| --- | --- | --- | --- |
| A000 | raw `_OS3K_ClearScreen(void)` / public `ClearScreen(void)` | mechanical A; clears display, hides OS cursor and restores OS cursor to 1,1; BetaWise wrapper additionally resets its private scroll/start-line state | [`clear-screen-closure.md`](clear-screen-closure.md), `os3k/os3k.h` |
| A0F0–A110 | Dialog API (`DialogInit` through `DialogGetItemId`) | mechanically closed for normal public behavior; internal event codes remain unnamed | [`dialog-api-closure.md`](dialog-api-closure.md) |
| A138 | `ShowBatteryPercentage(uint8_t time_seconds)` | ABI/parameter semantics closed | [`battery-api-closure.md`](battery-api-closure.md) |
| A1A0 | `FileSmashFile` | closed; destructive | [`file-core-operations.md`](file-core-operations.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1A4 | `FileDeleteChars` | closed | [`filedeletechars-closure.md`](filedeletechars-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1A8 | `FileGetCurrentFile` | mechanical A; SDK consolidated/audited; read-only canonical-token query; regression pending | [`filegetcurrentfile-closure.md`](filegetcurrentfile-closure.md), [`file-core-operations.md`](file-core-operations.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1AC | `SYS_A1AC` global allocator/reclaimable metric | mechanical contract closed; original name open; `+0x10` now identified as `min_size` | [`file-space-accounting.md`](file-space-accounting.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1B0 | `SYS_A1B0(file_id)` unused descriptor capacity | mechanical contract closed; original name open | [`file-space-accounting.md`](file-space-accounting.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1B4 | `SYS_A1B4(token_or_selector)` size/accounting family | mechanical paths closed; selector names open; `0xFE/0xFF` account against `min_size` | [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1B8 | `SYS_A1B8(size_or_command)` resize/state family | mechanical paths closed; command names partly historical only; `-5` updates `min_size` | [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1BC | `SYS_A1BC(file_id)` descriptor maximum capacity | mechanical contract closed; original name open | [`file-space-accounting.md`](file-space-accounting.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1C0 | `FileSetFolder` | closed | [`filesetfolder-closure.md`](filesetfolder-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1C4 | `FileGetFileInfo` | closed; live-mirror lifetime is contractual | [`file-api-current-reference.md`](file-api-current-reference.md), `applets/FileGetFileInfoProbe/` |
| A1C8/A1CC | `FileOpen` / `FileClose` | closed | [`fileopen-fileclose-closure.md`](fileopen-fileclose-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1D0/A1D4 | per-file state-bit mutate/query | mechanically closed as `SYS_A1D0/A1D4`; bit names open | [`file-password-state-closure.md`](file-password-state-closure.md), `os3k/file_password_state.h` |
| A1D8 | reset group passwords to factory `write` after master verification | mechanically closed; destructive; original name open | [`file-password-state-closure.md`](file-password-state-closure.md), `os3k/file_password_state.h` |
| A1DC | bidirectional per-file password get/set | mechanically closed; original modern name open | [`file-password-state-closure.md`](file-password-state-closure.md), `os3k/file_password_state.h` |
| A1E0 | `PasswordVerifyFileDialog` | mechanical A; historical/name continuity B+; modern six-argument ABI documented | [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1E4 | `PasswordChangeFileDialog` | mechanical A; historical/name continuity B+; return is token/status, not changed boolean | [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1E8 | dynamic descriptor/file creation | mechanically closed as `SYS_A1E8`; original name open; establishes descriptor `+0x10 = min_size` | [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A1EC | dynamic descriptor/file removal + allocator/table compaction | mechanically closed as `SYS_A1EC`; destructive; original name open | [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A1F0 | filename get/set (`+0x34`) | mechanically closed as `SYS_A1F0`; original name open | [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A1F4/A1F8 | local index (`+0x46`) get/set and reverse lookup | mechanically closed as `SYS_A1F4/A1F8`; original names open | [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A1FC | construct/validate 16-bit token | mechanically closed as `SYS_A1FC`; original name open | [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A200 | copy active-file range to clipboard | mechanically closed as `SYS_A200`; original name/tag enum open | [`clipboard-edit-closure.md`](clipboard-edit-closure.md), `os3k/file_clipboard_edit.h` |
| A204 | cut active-file range to clipboard | mechanically closed as `SYS_A204`; deletes only bytes actually copied | [`clipboard-edit-closure.md`](clipboard-edit-closure.md), `os3k/file_clipboard_edit.h` |
| A208 | paste clipboard through A198 write modes | mechanically closed as `SYS_A208`; write modes known, read variant 3 is control-code-aware/filtered; original names open | [`clipboard-edit-closure.md`](clipboard-edit-closure.md), `os3k/file_clipboard_edit.h` |
| A20C | `ClipboardSet` | mechanically closed; raw errors, capacity limiting and shared tag state identified | [`clipboard-buffer-closure.md`](clipboard-buffer-closure.md), `os3k/file_clipboard_buffer.h` |
| A210 | `ClipboardGet` | raw path closed and portable in compared ROMs; filtered selector 3 closed mechanically with documented 2005-vs-2013 firmware difference | [`clipboard-buffer-closure.md`](clipboard-buffer-closure.md), `os3k/file_clipboard_buffer.h` |
| A214 | `ClipboardClear` | mechanically closed; logical/allocation-state clear, not secure erase | [`clipboard-buffer-closure.md`](clipboard-buffer-closure.md), `os3k/file_clipboard_buffer.h` |
| A218 | shared clipboard-tag byte getter | mechanically closed as `SYS_A218`; low-byte return only; original enum/name open | [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A21C | raw/filtered clipboard logical-length query | mechanically closed as `SYS_A21C`; selector 3 computes filtered length | [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A220 | clipboard storage/base pointer query | mechanically closed as `SYS_A220`; original name open | [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A224 | clipboard maximum allocated-capacity query | mechanically closed as `SYS_A224`; original name open | [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A228 | allocator-backed clipboard logical-size set/resize | mechanically closed as `SYS_A228`; returns effective clamped size | [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A22C | selected SmartApplet runtime-index install after block-mask check | mechanically closed as `SYS_A22C`; original name open | [`applet-selection-closure.md`](applet-selection-closure.md), `os3k/applet_selection.h` |
| A230 | clear selected SmartApplet runtime index to `-1` | mechanically closed as `SYS_A230`; deterministic `-1` result | [`applet-selection-closure.md`](applet-selection-closure.md), `os3k/applet_selection.h` |
| A234 | selected runtime index to 16-bit SmartApplet ID | mechanically closed as `SYS_A234`; low-word contract | [`applet-selection-closure.md`](applet-selection-closure.md), `os3k/applet_selection.h` |
| A238 | `AppletFindByName` | mechanical A; SDK consolidated/audited; prefix-name lookup; exclusive 32-bit start index; full 32-bit runtime-index result; const input; old 8-bit index/return superseded; regression pending | [`appletfindbyname-closure.md`](appletfindbyname-closure.md), [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md), `os3k/os3k.h` |
| A23C | `AppletFindById` | mechanical A; SDK consolidated/audited; exact 16-bit header-ID lookup; full 32-bit runtime-index result; old 8-bit return superseded; regression pending | [`appletfindbyid-closure.md`](appletfindbyid-closure.md), [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md), `os3k/os3k.h` |
| A240 | `AppletGetName` | mechanically closed; 36-byte header-name copy plus NUL, low-byte 1/0 result | [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md) |
| A244 | `AppletSendMessage` | mechanically closed; target validation, synthetic/private message normalization, A5/current-applet context switch and restore | [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md) |
| A248 | `SYS_A248` file-password-protection state getter | mechanically closed; raw low-byte state, original name open | [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/password_runtime.h` |
| A24C | `SYS_A24C(password)` master-password comparison | mechanically closed; non-interactive low-byte Boolean result, original name open | [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/password_runtime.h` |
| A250 | `SYS_A250(reserved,prompt)` interactive master-password gate | mechanically closed; first slot unused by compared handlers, native callers use 2 | [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/password_runtime.h` |
| A254 | `SYS_A254(token_group,name_out)` File API token-group/high-byte selector | mechanically closed; selector 0=current, explicit 1..4, optional 30-byte group-name copy | [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/file_token_group.h` |
| A258 | `SYS_A258(protection_state)` file-password-protection state setter | mechanically closed; raw byte store, no return contract or built-in authorization | [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/password_runtime.h` |
| A25C | `SYS_A25C(action_mask,key)` global-service dispatcher | mechanically closed; 32-bit mask, independent bits 0–3, signed returns 0/8/-9 | [`system-service-dispatch-closure.md`](system-service-dispatch-closure.md), `os3k/os3k.h` |
| A260–A270 | `SYS_A260`/`A264`/`A268`/`A26C`/`A270` selection-block masks | mechanically closed; 32-entry longword table, bit-0 and dynamic-reason mutations | [`applet-selection-mask-closure.md`](applet-selection-mask-closure.md), `os3k/applet_selection_mask.h` |
| A274 | `SYS_A274(void)` printer-selection dialog | mechanically closed; no return contract; AS3000 has 7 printer records and StyleWriter validation, NEO has 5 | [`printer-selection-closure.md`](printer-selection-closure.md), `os3k/printer_selection.h` |
| A27C | `SYS_A27C(cursor,field_0,field_2,payload_size,payload)` packed-record cursor | mechanically closed; five pointer slots, three 16-bit header outputs, payload pointer and conditional even-byte cursor advance | [`packed-record-cursor-closure.md`](packed-record-cursor-closure.md), `os3k/record_cursor.h` |
| A280 | `SYS_A280(cursor,field_0,field_2,payload_size,payload)` packed-record writer | mechanically closed; five slots, three 16-bit header inputs, exact payload copy and even-byte cursor advance | [`packed-record-writer-closure.md`](packed-record-writer-closure.md), `os3k/record_writer.h` |
| A284 | `SYS_A284(cursor,field_0,field_2,payload_size,payload)` packed-record search | mechanically closed; first exact pair, zero first-field sentinel, optional outputs, signed 0/-1 return | [`packed-record-search-closure.md`](packed-record-search-closure.md), `os3k/record_search.h` |

## Source layers used for reconstruction

Evidence is normally assembled from several independent layers:

1. **Firmware handlers** from AS3000 System 3 November 2005, NEO System 3
   November 2005, and NEO/System 3.15 July 2013.
2. **Official SmartApplet callers** such as AlphaWord Plus, AlphaQuiz, KeyWords,
   Calculator and ControlPanel.
3. **System 3 internal callers and strings**, especially when an operation is
   encapsulated rather than directly invoked by SmartApplets.
4. **Original AS3000 source/object material (1998–2000)** for genealogy and
   names, without assuming ABI identity.
5. **Independent reverse engineering** such as BetaWise/Ghidra/neo-re as
   comparative evidence rather than authority over direct firmware behavior.
6. **Emulator/hardware regression** as an observable verification layer after
   the machine contract is known.

## Platform and generation rule

Claims shared by AlphaSmart 3000 and NEO require direct comparison of the
relevant handler/data flow. Known generation differences remain explicit. For
example, A1D0/A1D4 accept mask `0x0D` in the 2005 ROMs and add `0x10` in NEO
2013. A210 also demonstrates why this rule matters: the 2005 filtered-read path
passes an uninitialized local as the filter helper's initial output limit,
whereas NEO 2013 explicitly initializes the output limit from caller `count`.

A000 preserves one public display/cursor contract across AS3000 and NEO while
using generation-specific hardware paths. AS3000 issues clear-display commands
to its two character-LCD halves. NEO explicitly clears both graphical LCD
controllers and its character shadow. Both hide the OS cursor and restore
logical position 1,1; raw A000 does not promise to normalize the NEO LCD
start-line, which remains a BetaWise-wrapper responsibility in extended-font
mode.

A22C–A234 are stable across the compared ROMs apart from relocated runtime
globals, mask-table helper and applet-header table addresses. A22C relies on the
normal runtime-index domain established by the surrounding applet machinery
rather than performing its own explicit bounds test.

A238–A244 are likewise structurally stable across all three compared ROMs.
A238/A23C/A240 differ only in relocated table/library addresses, while A244 adds
relocated current-applet state, selection-block helper and per-applet A5-context
locations. The message-normalization and callback sequence are unchanged.

A248–A258 are also stable across the three compared ROMs. A248/A258 remain a
byte getter/setter pair over the relocated file-password-protection global;
A24C and A250 use the relocated master-password buffer but preserve comparison
and UI control flow; A254 preserves the four 30-byte token-group name slots and
returns the group byte used as the high byte by A1FC.

A25C preserves the same 32-bit mask/key-slot ABI and 0/8/-9 return contract in
the compared ROMs. NEO/System 3.15 adds one stage to the bit-0 service path
without changing the public contract.

A260–A270 retain the same 32-entry longword table model, per-service mutations
and validation boundaries in all three compared ROMs.

A274 is a deliberate example of a platform-visible difference that must not be
normalized away: the AS3000 2005 handler enumerates seven printer records,
including ImageWriter and StyleWriter, and contains an additional StyleWriter
validation/warning path; the two compared NEO handlers enumerate five printer
records and use the compact commit path.

A27C is byte-identical across all three compared ROMs. Each image also contains
three structurally equivalent internal callers that pass five pointer slots and
ignore the return register. The two leading 16-bit record fields remain neutral;
the third word is the payload byte count used for even-byte cursor progression.

A280 preserves the same mechanical writer contract in all three ROMs. The only
handler-byte difference is the relocated address of an identical forward-copy
helper. Two equivalent internal callers per ROM pass five slots, allocate six
plus the even-rounded payload size, and ignore the residual return register.

A284 is byte-identical across all three compared ROMs. One equivalent internal
caller per ROM passes five slots and tests the full 32-bit 0/-1 return. Its
alternate path invokes A27C on the same record sequence, independently
correlating the shared layout and cursor semantics.

## Current File API usage path

For A000 display clearing and the distinction between the raw System 3 trap and
the BetaWise scrolling wrapper, use
[`clear-screen-closure.md`](clear-screen-closure.md).

For application development, start with
[`file-api-current-reference.md`](file-api-current-reference.md) for the core
namespace/token/descriptor lifecycle, `min_size`, live mirrors, passwords,
dynamic descriptors and A200–A208 clipboard editing. The A1A8 SDK consolidation
is recorded separately in
[`filegetcurrentfile-closure.md`](filegetcurrentfile-closure.md), while the deeper
A1A0/A1A8 research evidence remains in
[`file-core-operations.md`](file-core-operations.md). Use the focused clipboard
closure documents for A20C–A228 until those later members are folded into the
single current-reference narrative.

The A22C–A234 selected-applet context is documented separately in
[`applet-selection-closure.md`](applet-selection-closure.md), because it belongs
to dispatcher/SmartApplet state rather than the File or clipboard API.

For cross-applet discovery and messaging, use
[`applet-runtime-api-closure.md`](applet-runtime-api-closure.md). A238's
SDK-facing consolidation, prefix-search semantics, const input and 32-bit
start-index/return correction are recorded separately in
[`appletfindbyname-closure.md`](appletfindbyname-closure.md). A23C's SDK-facing
consolidation and the supersession of the old 8-bit return are recorded in
[`appletfindbyid-closure.md`](appletfindbyid-closure.md). A244 owns A5/current-
applet context switching around the target `ProcessMessage` callback.

For the immediately following runtime services, use
[`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md).
It separates the A248/A24C/A250/A258 master/file-password runtime state from the
A254 File API token-group helper instead of treating numeric adjacency as one
subsystem.

The A25C global-service dispatcher is documented in
[`system-service-dispatch-closure.md`](system-service-dispatch-closure.md).
Its neutral name and unresolved bit-0–2 service labels are intentional.

The A260–A270 selection-block mask services are documented in
[`applet-selection-mask-closure.md`](applet-selection-mask-closure.md). The
32-bit aggregate mask and the distinct clear-all/clear-one operations are part
of the contract.

The A274 printer-selection UI is documented in
[`printer-selection-closure.md`](printer-selection-closure.md). Its AS3000 and
NEO record sets are intentionally documented separately because they differ in
both available printer families and validation flow.

The A27C packed-record cursor is documented in
[`packed-record-cursor-closure.md`](packed-record-cursor-closure.md). Its
five-pointer contract is closed, while the original symbol and the semantic
names of the first two record fields intentionally remain unresolved.

The complementary A280 packed-record writer is documented in
[`packed-record-writer-closure.md`](packed-record-writer-closure.md). It shares
the neutral field labels, writes exact payload bytes, leaves odd padding
untouched, and advances the cursor with 16-bit even rounding.

The A284 packed-record search is documented in
[`packed-record-search-closure.md`](packed-record-search-closure.md). It searches
for the first exact neutral-field pair, exposes optional size/payload outputs,
and documents the zero sentinel, cursor-on-failure, and 16-bit wrap behavior.

Use the focused closure notes when exact evidence, historical genealogy, ROM
addresses, raw errors, generation-specific quirks, or safety edge cases matter.

## Traceability rule for future work

When a new block is closed, update all applicable layers in the same milestone:

1. implementation/stub/header if ready for use;
2. closure/current-reference document with the complete contract;
3. this index;
4. README;
5. project ABI map and master bitacora in Google Drive.

If later evidence corrects an earlier conclusion, preserve the research history
where useful but make the canonical reference state the correction explicitly.