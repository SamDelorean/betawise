# FileSetFolderProbe

Executable regression probe for System 3 syscall A1C0 / `FileSetFolder`.

This is not a discovery probe.  The ABI has already been reconstructed from the
AS3000 and NEO firmware handlers and official SmartApplet callers.  The purpose
of this applet is to turn that recovered contract into a repeatable regression.

## Run policy

Run the invalid-input cases in the firmware-first emulator before physical
hardware.  The recovered handler validates both error cases before changing the
folder or clearing the active descriptor, but the emulator remains the preferred
first execution environment.

Open the applet and press **Cmd+F** to run the matrix.

The stock AlphaWord Plus, AlphaQuiz and Calculator applets are required.  The
probe aborts instead of substituting another index if one of them is absent.

## Confirmed contract exercised

| Case | Setup | Expected result | Expected output flags | State invariant |
| --- | --- | ---: | --- | --- |
| AlphaWord special namespace | `AppletFindById(0xA000)` | success; return previous folder | bit `0x40` set | restoring returned folder returns AlphaWord index |
| Normal file-owning namespace | `AppletFindById(0xA001)` | success; return previous folder | bit `0x40` clear | restoring returned folder returns AlphaQuiz index |
| No eligible runtime file group | Calculator `0xA002` | `-7` | output remains `0` | failed call does not change current folder |
| Out-of-range applet index | literal `32` | `-64` | output remains `0` | failed call does not change current folder |

The firmware also returns `-64` for an in-range runtime applet slot whose
companion ID word is `0xFFFF`.  That branch is mechanically confirmed but is not
forced by this applet because there is no public API for selecting a stable,
uninstalled runtime slot across arbitrary device configurations.

## Why the raw values and `0x40` are not named

The original AS3000 source tree from 2000 uses a different error-number family:
`STATUS_FILE_BASE` is `-700`, with file errors occupying `-701..-710`.  Therefore
the later System 3 returns `-7` and `-64` cannot safely inherit those historical
symbol names.

Likewise, the low-byte SmartApplet flag `0x40` has a confirmed mechanical role in
A1C0: it bypasses the ordinary requirement that the selected runtime file group
have a non-zero file count.  AlphaWord Plus 3.4 is the decisive stock example:
its header is `flags=0xFF0000CE`, `fileCount=0`, while AlphaQuiz declares 15 files
without bit `0x40` and KeyWords declares 8 without it.

Independent SmartApplet tooling does not provide a recovered original name for
this bit.  AlphaSync documents the known flag patterns but names only `0x01` as
`hidden`; the older `neotools` constants preserve the same limitation, and
modern `neo-re` also keeps unresolved header bits descriptive rather than
inventing semantic names.

For that reason the probe uses deliberately local research labels
`FILESETFOLDER_RAW_*` and `FILESETFOLDER_FLAG_0X40`.  They are **not** proposed as
original OS3K symbols and are not added to the public `os3k.h` API.

## Closure criterion

`FileSetFolder` is considered closed at the ABI/SDK level when this matrix runs
unchanged under the target firmware and the emulator reproduces the same
results.  Recovery of the original private source-code names for `-7`, `-64`, or
flag `0x40` would be useful historical information, but is no longer required to
use the function correctly and must not block the SDK with guessed constants.
