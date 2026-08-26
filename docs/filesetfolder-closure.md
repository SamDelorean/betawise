# A1C0 / `FileSetFolder` — ABI closure

Status: **closed for SDK use; confidence A for the mechanical ABI**.

This note records the closure point for System 3 syscall A1C0.  The public
function name `FileSetFolder` is retained because the observed firmware behavior
and official callers agree with selecting a SmartApplet-owned file namespace.
The private source-level names of two negative return codes and one applet flag
have not been recovered; they are intentionally not guessed.

## Public prototype

```c
int32_t FileSetFolder(uint32_t applet_index, uint32_t *applet_flags_out);
```

`applet_flags_out` may be `NULL`.

The first argument is the **runtime SmartApplet index**, not the 16-bit applet ID
and not a file number.  The handler consumes the full 32-bit argument and accepts
only indices `0..31`.

## Mechanical contract

The equivalent control flow established from the AS3000/NEO firmware is:

```c
int32_t FileSetFolder(uint32_t applet_index, uint32_t *applet_flags_out)
{
    if (applet_flags_out != NULL)
        *applet_flags_out = 0;

    if (applet_index >= 32)
        return -64;

    if (runtime_applet_id[applet_index] == 0xFFFF)
        return -64;

    AppletHeader_t *header = runtime_applet_header[applet_index];

    if ((header->flags & 0x40) == 0 &&
        runtime_file_group[applet_index].file_count == 0)
        return -7;

    FileClose();                /* A1CC: clear active descriptor */

    int32_t previous = current_folder;
    current_folder = applet_index;

    if (applet_flags_out != NULL)
        *applet_flags_out = header->flags;

    return previous;
}
```

The pseudo-code names the internal structures descriptively; it does not claim
that these were the original private source identifiers.

## Return and state invariants

| Path | Return | `*applet_flags_out` when non-NULL | Folder state |
| --- | ---: | --- | --- |
| valid namespace | previous runtime folder index | full 32-bit `AppletHeader_t.flags` | changes to requested index |
| `applet_index >= 32` | `-64` | zero | unchanged |
| in-range runtime slot marked `0xFFFF` | `-64` | zero | unchanged |
| no eligible normal runtime file group and flag `0x40` clear | `-7` | zero | unchanged |

A1CC is reached only after validation succeeds, so the active descriptor is not
cleared by the confirmed error paths.

## Official caller proof

AlphaQuiz contains the sequence conceptually equivalent to:

```c
uint32_t alphaWord = AppletFindById(0xA000);
int32_t previous = FileSetFolder(alphaWord, NULL);
/* operate in AlphaWord Plus file namespace */
FileSetFolder((uint32_t)previous, NULL);
```

The value returned in D0 is therefore demonstrably the previous folder index,
not a generic success code.

ControlPanel supplies a real second pointer.  After A1C0 it tests bit 6 of the
low byte of the 32-bit flags written through that pointer.  This independently
closes the second argument as an `AppletHeader_t.flags` output.

## Flag `0x40`

The exact original symbolic name of low-byte flag `0x40` has **not** been
recovered.  Its A1C0 effect is nevertheless unambiguous: when set, it bypasses
the ordinary requirement that the selected runtime file group contain at least
one file.

Stock headers show why it must not be named simply "has files":

| SmartApplet | ID | flags | header `fileCount` | bit `0x40` |
| --- | ---: | ---: | ---: | --- |
| AlphaWord Plus 3.4 | `0xA000` | `0xFF0000CE` | 0 | set |
| AlphaQuiz | `0xA001` | `0xFF000090` | 15 | clear |
| Calculator | `0xA002` | `0xFF000000` | 0 | clear |
| KeyWords | `0xA004` | `0xFF000000` | 8 | clear |
| ControlPanel | `0xA007` | `0xFF000080` | 0 | clear |

AlphaWord Plus is therefore the decisive special case: it exposes a usable file
namespace even though its applet header declares zero ordinary files.

The same header bit is tested in additional System 3 applet-management code, so
its global meaning is broader than the one A1C0 branch.  The SDK therefore does
not publish a guessed `APPLET_FLAG_*` name for it.

Independent mature tooling reaches the same naming limit: AlphaSync and the
older `neotools` code document known applet flag patterns but only give `0x01`
the semantic name `hidden`; modern `neo-re` likewise leaves unresolved header
bits descriptive rather than inventing source-level names.

## Raw returns `-7` and `-64`

Their exact original symbolic names are also unrecovered.

The historical AS3000 source tree from 2000 cannot supply them: its file-status
namespace is a different scheme, with `STATUS_FILE_BASE = -700` and named file
errors occupying `-701..-710`.  Assigning one of those historical names to the
later System 3 values `-7` or `-64` would be false continuity.

For SDK documentation the safe contract is therefore behavioral:

- `-64`: out-of-range runtime SmartApplet index or an in-range slot whose runtime
  ID word is `0xFFFF`;
- `-7`: selected applet has no eligible ordinary runtime file group while the
  header's low-byte `0x40` exception bit is clear.

No public status constants are added until an original or independently
well-supported name is recovered.

## Regression specification

`applets/FileSetFolderProbe/` implements the closure as an executable regression.
It is deliberately **emulator first** for the invalid-input cases.

The minimum matrix is:

1. AlphaWord Plus (`0xA000`): success, output flag `0x40` set, restore via returned
   folder index.
2. AlphaQuiz (`0xA001`): success through the ordinary non-empty file-group path,
   output `0x40` clear, restore via returned folder index.
3. Calculator (`0xA002`): expected `-7`; output initialized to a non-zero sentinel
   must become zero; subsequent restore must show that the failed call did not
   change `current_folder`.
4. Literal runtime index `32`: expected `-64`; output must become zero; subsequent
   restore must show that the failed call did not change `current_folder`.

The additional `0xFFFF` in-range slot branch is mechanically confirmed but is
not forced by the applet because an arbitrary installation does not provide a
stable public API for choosing an unused runtime slot.

## Closure decision

`FileSetFolder` is **closed at the ABI and SDK level**.  Its public prototype is
appropriate for `os3k.h` and is already present there.

Recovery of the original private names for `-7`, `-64`, or `0x40` remains useful
historical research, but it is no longer an ABI blocker.  Those names must remain
unpublished rather than be fabricated.

The only remaining verification step for this function is execution of the
regression applet under the firmware-first emulator, followed later by a safe
hardware regression if desired.  Until that execution occurs, the regression is
an executable specification, not a claimed passing test.
