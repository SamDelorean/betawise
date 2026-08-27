BetaWise
========
Several custom applets for the AlphaSmart 3000 / NEO / NEO2, and tools to create them.

Documentation
-------------
Active System 3 / OS3K ABI reconstruction notes are kept under `docs/`.

Start here when using or auditing the reconstructed ABI:

* [ABI reconstruction index](docs/abi-reconstruction-index.md) — canonical map of reconstructed blocks, confidence/naming policy, evidence layers, current-vs-historical documentation, and traceability rules.
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
