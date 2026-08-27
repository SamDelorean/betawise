# System 3 / OS3K ABI reconstruction index

This is the **canonical navigation and traceability index** for reconstructed
System 3 / OS3K interfaces in this repository.  It answers two different
questions that should not be confused:

1. **What is the current contract that a developer may rely on?**
2. **Where is the evidence and research history that produced that contract?**

The older `file-api-reconstruction.md` is intentionally retained as a
**chronological research log**.  It contains provisional statements that were
correctly superseded by later evidence.  Do not use an early provisional
section from that log as the current SDK contract when a closure/current-reference
document listed here exists.

## Evidence and naming policy

- **Mechanical confidence A**: argument width/order, return behavior, state
  mutation, or control flow is established directly from firmware handlers or
  equivalent primary evidence.
- **Historical/name confidence** is tracked separately.  A behavior may be
  mechanically closed while its original public System 3 symbol remains
  unknown.
- A recovered historical AS3000 name is reused only when the later System 3
  operation is demonstrably the same conceptual API member.  ABI evolution is
  documented rather than hidden.
- When behavior is closed but the original modern name is not, the interface
  remains `SYS_Axxx`.  Descriptive guesses are not promoted to API names.
- A regression specification is **not** evidence that the test has been run.
  Documents state explicitly whether emulator or hardware execution has
  occurred.

Each closed block should document: purpose, prototype, arguments and returns,
state effects, errors/sentinels, cross-ROM differences, caller/source evidence,
safety constraints, validation status, and the commit or document that makes
the result auditable.

## Current reconstructed blocks

| Trap/range | Current identity | Status | Canonical documentation |
| --- | --- | --- | --- |
| A0F0–A110 | Dialog API (`DialogInit` through `DialogGetItemId`) | mechanically closed for normal public behavior; internal event codes 0x64–0x67 remain unnamed | [`dialog-api-closure.md`](dialog-api-closure.md) |
| A138 | `ShowBatteryPercentage(uint8_t time_seconds)` | ABI/parameter semantics closed; helper A120/A128/A130 names remain open | [`battery-api-closure.md`](battery-api-closure.md) |
| A1A0 | `FileSmashFile` | closed; destructive | [`file-core-operations.md`](file-core-operations.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1A4 | `FileDeleteChars` | closed | [`filedeletechars-closure.md`](filedeletechars-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1A8 | `FileGetCurrentFile` | closed | [`file-core-operations.md`](file-core-operations.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1AC | `SYS_A1AC` global allocator/reclaimable-space metric | mechanical contract closed; original name open | [`file-space-accounting.md`](file-space-accounting.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1B0 | `SYS_A1B0(file_id)` unused descriptor capacity | mechanical contract closed; original name open | [`file-space-accounting.md`](file-space-accounting.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1B4 | `SYS_A1B4(token_or_selector)` size/accounting query family | mechanical paths closed; selector names open | [`file-api-current-reference.md`](file-api-current-reference.md), research history in [`file-api-reconstruction.md`](file-api-reconstruction.md) |
| A1B8 | `SYS_A1B8(size_or_command)` active-file resize/state family | mechanical paths closed; command names partly historical only | [`file-api-current-reference.md`](file-api-current-reference.md), research history in [`file-api-reconstruction.md`](file-api-reconstruction.md) |
| A1BC | `SYS_A1BC(file_id)` descriptor maximum capacity | mechanical contract closed; original name open | [`file-space-accounting.md`](file-space-accounting.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1C0 | `FileSetFolder` | closed | [`filesetfolder-closure.md`](filesetfolder-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1C4 | `FileGetFileInfo` | closed; live-mirror lifetime is part of the contract | [`file-api-current-reference.md`](file-api-current-reference.md), executable specification in `applets/FileGetFileInfoProbe/` |
| A1C8/A1CC | `FileOpen` / `FileClose` | closed | [`fileopen-fileclose-closure.md`](fileopen-fileclose-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1D0/A1D4 | per-file state-bit mutate/query | mechanically closed as `SYS_A1D0` / `SYS_A1D4`; individual flag names open | [`file-password-state-closure.md`](file-password-state-closure.md), `os3k/file_password_state.h` |
| A1D8 | reset all file passwords to factory default `write` after master-password verification | mechanically closed; destructive; original name open | [`file-password-state-closure.md`](file-password-state-closure.md), `os3k/file_password_state.h` |
| A1DC | bidirectional per-file password get/set | mechanically closed; original modern name open | [`file-password-state-closure.md`](file-password-state-closure.md), `os3k/file_password_state.h` |

## Source layers used for reconstruction

Evidence is normally assembled from several independent layers:

1. **Firmware handlers** from AS3000 System 3 November 2005, NEO System 3
   November 2005, and NEO/System 3.15 July 2013.  These establish the actual
   machine-level ABI and state mutation.
2. **Official SmartApplet callers** such as AlphaWord Plus, AlphaQuiz, KeyWords,
   Calculator and ControlPanel.  These establish real argument values, return
   consumption, and expected sequencing.
3. **System 3 internal callers and strings**, useful when an operation is
   normally encapsulated rather than invoked directly by SmartApplets.
4. **Original AS3000 source and object material (1998–2000)**, used for
   genealogy and names.  It is not assumed ABI-identical to later System 3.
5. **Independent reverse engineering** such as earlier BetaWise/Ghidra/neo-re
   annotations, treated as comparative evidence rather than authority when it
   conflicts with direct firmware behavior.
6. **Emulator/hardware regression**, used to verify observable behavior after
   the machine-level contract is known.  Tests are separated from discovery
   when a call is destructive or unsafe.

## Platform and generation rule

A statement that applies to both AlphaSmart 3000 and NEO is only made when the
relevant handlers/data flow have been compared.  Known generation differences
are preserved explicitly.  For example, A1D0/A1D4 accept state-mask bits
`0x01|0x04|0x08` in the 2005 ROMs and add `0x10` in NEO 2013.

## Current File API usage path

For application development, start with
[`file-api-current-reference.md`](file-api-current-reference.md).  That document
contains the current descriptor model, token rules, active-file lifecycle,
namespace switching, live-mirror lifetime rules, destructive-operation warnings,
and a recommended call sequence.  The individual closure notes are then used
when the evidence trail or edge cases matter.

## Traceability rule for future work

When a new trap/block is closed, update all applicable layers in the same
milestone:

1. implementation/stub/header if the interface is ready for use;
2. a closure or current-reference document containing the full contract;
3. this index;
4. the repository README;
5. the project ABI map and master bitacora in Google Drive.

If later evidence corrects an earlier conclusion, preserve the historical note
where useful but make the canonical reference state the correction explicitly.
This keeps the research reproducible without forcing future developers to infer
which old statement is still current.
