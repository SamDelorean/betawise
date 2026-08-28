BetaWise
========
Several custom applets for the AlphaSmart 3000 / NEO / NEO2, and tools to create them.

Documentation
-------------
Active System 3 / OS3K ABI reconstruction notes are kept under `docs/`.

Start here when using or auditing the reconstructed ABI:

* [ABI reconstruction index](docs/abi-reconstruction-index.md) — canonical map of reconstructed blocks, confidence/naming policy, evidence layers, current-vs-historical documentation, and traceability rules.
* [ClearScreen closure](docs/clear-screen-closure.md) — closes A000 mechanically across AS3000 and NEO, including the no-argument/void raw syscall, cursor reset, generation-specific display clearing, and the additional scroll/start-line work performed by the public BetaWise wrapper.
* [Current File API reference](docs/file-api-current-reference.md) — current developer-facing contract for namespaces, tokens, descriptors, `min_size`, live mirrors, open/close lifecycle, deletion, accounting, passwords, dynamic files and clipboard editing. Prefer this over early provisional File API notes when deciding how to call the API.
* [Dialog API closure](docs/dialog-api-closure.md) — consolidated A0F0–A110 menu/dialog contract: item metadata, marker/shortcut/file-size behavior, layout, navigation, exit ordering and getter invariants.
* [Battery API closure](docs/battery-api-closure.md) — A138 / `ShowBatteryPercentage`, including zero-vs-timed modes, units, cross-ROM evidence and historical ABI evolution.

Detailed evidence and closure notes:

* [File API reconstruction log](docs/file-api-reconstruction.md) — chronological firmware/caller research log. It intentionally preserves provisional stages that were later superseded; use the current reference/closure documents for the present contract.
* [FileSetFolder ABI closure](docs/filesetfolder-closure.md) — closed A1C0 contract, raw error behavior, the unresolved `0x40` flag name, and the emulator-first regression matrix in `applets/FileSetFolderProbe/`.
* [Core file operations](docs/file-core-operations.md) — closes A1A0 as `FileSmashFile` and A1A8 as `FileGetCurrentFile`, including modern return/token behavior and historical lineage.
* [FileDeleteChars ABI closure](docs/filedeletechars-closure.md) — closes A1A4 as the active-file deletion primitive, including EOF clamping, return semantics, clipboard caller evidence and historical lineage.
* [File-space accounting](docs/file-space-accounting.md) — closes A1AC/A1B0/A1BC and now records descriptor `+0x10` as the confirmed `min_size` field used by allocation/accounting.
* [FileOpen / FileClose ABI closure](docs/fileopen-fileclose-closure.md) — closes A1C8/A1CC, including exact token/cursor behavior, failed-open state preservation, and the fact that FileClose only detaches the active descriptor rather than committing or flushing it.
* [File password/state ABI closure](docs/file-password-state-closure.md) — closes A1D0–A1DC mechanically: per-file state-bit mutation/query, bidirectional five-character file passwords, and the master-password-gated destructive reset to factory default `write`.
* [File identity / dynamic-file closure](docs/file-identity-dynamic-closure.md) — closes A1E0–A1FC: `PasswordVerifyFileDialog`, `PasswordChangeFileDialog`, dynamic descriptor creation/removal, filename metadata, local-index lookup, 16-bit token construction, and the `min_size` correction. Unnamed mechanics are exposed in `os3k/file_dynamic.h`.
* [Clipboard editing closure](docs/clipboard-edit-closure.md) — closes A200–A208 copy/cut/paste mechanics, capacity-clipped cut safety, A198 append/insert/overwrite modes, clipboard tag state, and the control-code-aware read variant. Generic prototypes are in `os3k/file_clipboard_edit.h`.
* [Clipboard buffer API closure](docs/clipboard-buffer-closure.md) — closes named A20C/A210/A214 `ClipboardSet`/`ClipboardGet`/`ClipboardClear`, raw errors and allocation behavior, and documents the 2005-vs-2013 filtered-read firmware difference. Prototypes are in `os3k/file_clipboard_buffer.h`.
* [Clipboard state/capacity closure](docs/clipboard-state-capacity-closure.md) — closes A218–A228 mechanically: shared tag getter, raw/filtered logical length, storage pointer, maximum capacity and allocator-backed logical resize. Generic prototypes are in `os3k/file_clipboard_state.h`.
* [Applet-selection context closure](docs/applet-selection-closure.md) — closes A22C–A234 mechanically: selection-block-mask validation, selected runtime-index install/clear, and conversion of the selected runtime index to the 16-bit SmartApplet ID. Generic prototypes are in `os3k/applet_selection.h`.
* [SmartApplet runtime API closure](docs/applet-runtime-api-closure.md) — closes named A238–A244 lookup/name/message services, including prefix-name lookup, exact ID lookup, name-copy bounds, synthetic-message normalization, target validation and OS-owned A5 context switching.
* [Password/token-group runtime closure](docs/password-token-group-runtime-closure.md) — closes A248–A258 mechanically: global file-password-protection getter/setter, non-interactive and interactive master-password verification, and the File API token-group/high-byte selector with optional group-name copy. Generic prototypes are in `os3k/password_runtime.h` and `os3k/file_token_group.h`.
* [Global-service dispatcher closure](docs/system-service-dispatch-closure.md) — closes A25C mechanically: 32-bit action mask, independent service bits, special-key word argument and signed returns 0/8/-9. The neutral `SYS_A25C` name is retained in `os3k/os3k.h`.
* [SmartApplet selection-block mask closure](docs/applet-selection-mask-closure.md) — closes A260–A270 mechanically over the shared 32-entry longword mask table. Generic prototypes are in `os3k/applet_selection_mask.h`.
* [Printer-selection service closure](docs/printer-selection-closure.md) — closes A274 mechanically as the no-argument System 3 printer-selection dialog, including ROM-specific printer tables, the AS3000 StyleWriter validation path and the 7-vs-5 printer-set difference. Generic prototype is in `os3k/printer_selection.h`.
* [Packed-record cursor closure](docs/packed-record-cursor-closure.md) — closes A27C mechanically as a five-pointer parser for a six-byte packed header plus even-padded payload. The neutral prototype is in `os3k/record_cursor.h`.
* [Packed-record writer closure](docs/packed-record-writer-closure.md) — closes A280 mechanically as a five-argument writer for the same six-byte packed header plus even-padded payload. The neutral prototype is in `os3k/record_writer.h`.
* [Packed-record search closure](docs/packed-record-search-closure.md) — closes A284 mechanically as a two-key search over the same packed records, with optional outputs, sentinel behavior and signed 0/-1 return. The neutral prototype is in `os3k/record_search.h`.
* `FileGetFileInfo` / A1C4 is exposed in the SDK with persistent live-mirror semantics; `applets/FileGetFileInfoProbe/` provides a read-only regression specification and explicit unbind test.

Documentation policy: a mechanically closed interface may still retain a `SYS_Axxx` name when the original modern symbol is not recoverable. Behavior, naming confidence, regression state and platform/generation differences are tracked separately. The presence of a probe or regression specification never implies that it has already been executed.

Compiling
---------
Any `m68k-elf` gcc cross compiler should work. On Windows, I have successfully
used [MinGW GCC for M68K](https://sourceforge.net/projects/mingw-gcc-68k-elf/).

Installing Custom Applets
-------------------------
Using [Neo Manager](https://support.renaissance.com/techkb/techkb/13002475e.asp):
* File → Add to Applet List... → select [AppletFile].OS3KApp
* SmartApplets tab → select [AppletName] → Add
  * Optional: checking *Delete SmartApplets that are not in the Install List…*
    saves you a keypress or two during debug (**will delete existing applets**)
* Send List tab → Send

Note: On Windows 7 x64, Neo Manager stores custom applets in
`%LocalAppData%\VirtualStore\Program Files (x86)\AlphaSmart\AlphaSmart Manager 2\SmartApplets`

Tips
----
* To start in applet chooser, hold `Left Shift` + `Tab` while turning on.
* To reset to defaults, hold `Right Shift` + `Backspace` while turning on. The
  reset password is `tommy`.

Work in Progress
----------------
Much research and work is still needed:
* Improve stdio compatibility
* Investigate different statuses from `ProcessMessage`
* Find and document timer functions and messages
* Find and document non-message keyboard functions
* Reconstruct and validate the remaining System 3 file API operations
* Eliminate need to put global data into a structure

Contributions welcome!
