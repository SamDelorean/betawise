# SDK audit — closed subheaders eligible for umbrella integration

Date: 2026-09-14
Branch: `sdk/abi-automation`

## Purpose

Follow-up to `sdk-audit-umbrella-header-file-subheaders-gap-2026-09-14.md`.
This pass inventories the dedicated `os3k/` headers that contain mechanically
closed contracts but are not visible through the current `os3k/os3k.h` umbrella.
No ABI status is promoted by this document.

## Safe File API umbrella candidates

The following dedicated headers contain only contracts already treated as
closed by the current reconstruction documentation and are therefore safe
*candidates* for umbrella exposure:

- `file_space_accounting.h` — A1AC, A1B0, A1B4, A1B8, A1BC
- `file_password_state.h` — A1D0, A1D4, A1D8, A1DC
- `file_dynamic.h` — A1E8, A1EC, A1F0, A1F4, A1F8, A1FC
- `file_clipboard_edit.h` — A200, A204, A208
- `file_clipboard_buffer.h` — A20C, A210, A214 (`ClipboardSet`, `ClipboardGet`, `ClipboardClear`)
- `file_clipboard_state.h` — A218, A21C, A220, A224, A228

This expands the previously recorded umbrella gap: it is not limited to the
three A1D0–A208 headers. The closed accounting block A1AC–A1BC and clipboard
buffer/state block A20C–A228 are also isolated behind reconstruction-specific
subheader names.

## Integration rule

Prefer `#include` of these dedicated headers from `os3k.h` rather than copying
prototypes into the umbrella. This preserves one declaration source for each
contract and avoids prototype drift.

Do not infer that every dedicated header under `os3k/` belongs in the umbrella.
Applet, token-group, IRDA, password-runtime and other later headers require their
own publication-status check. In particular, no entry classified
`CERRADA_CON_LÍMITE_DE_EVIDENCIA` is authorized for public exposure by this
inventory.

## Build boundary

This inventory itself is documentation-only. The eventual umbrella include
change alters compile visibility and must be tested with the actual m68k SDK
build before being labelled `BUILD_VALIDATED`.

No m68k build was executed in this audit pass and no `BUILD_VALIDATED` claim is
made.
