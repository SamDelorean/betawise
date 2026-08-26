BetaWise
========
Several custom applets for the AlphaSmart 3000 / NEO / NEO2, and tools to create them.

Documentation
-------------
Active System 3 / OS3K ABI reconstruction notes are kept under `docs/`.

* [File API reconstruction](docs/file-api-reconstruction.md) — firmware- and caller-backed work on the A198–A1CC file subsystem and descriptor model.
* [FileSetFolder ABI closure](docs/filesetfolder-closure.md) — closed A1C0 contract, raw error behavior, the unresolved `0x40` flag name, and the emulator-first regression matrix in `applets/FileSetFolderProbe/`.
* [Core file operations](docs/file-core-operations.md) — closes A1A0 as `FileSmashFile` and A1A8 as `FileGetCurrentFile`, including modern return/token behavior and historical lineage.
* [FileDeleteChars ABI closure](docs/filedeletechars-closure.md) — closes A1A4 as the active-file deletion primitive, including EOF clamping, return semantics, clipboard caller evidence and historical lineage.
* `FileGetFileInfo` / A1C4 is now exposed in the SDK with persistent live-mirror semantics; `applets/FileGetFileInfoProbe/` provides a read-only regression specification and explicit unbind test.

Interfaces described as provisional in the research notes are intentionally not promoted into `os3k.h` until their contracts are sufficiently closed.

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
