# FileGetFileInfoProbe

Regression probe for System 3 syscall A1C4, now exposed by BetaWise as `FileGetFileInfo`.

The probe uses reserved file token `0x00CB` (clipboard) because that descriptor can be resolved without changing the current SmartApplet file folder. It is intentionally read-only: it does not clear, resize, write, or otherwise modify clipboard/file contents.

## Contract under test

```c
uint8_t *FileGetFileInfo(
    uint16_t file_id,
    uint8_t **storage_out,
    uint32_t *current_size_out,
    uint32_t *max_size_out,
    uint32_t *cursor_out);
```

The firmware-backed expectations are:

1. A successful call returns the descriptor's storage/base pointer.
2. `storage_out`, when non-NULL, immediately receives that same storage pointer.
3. The other non-NULL mirrors receive current size, maximum size, and cursor.
4. Mirror addresses persist in the descriptor and can be updated by later file operations.
5. Calling again with all four mirror pointers `NULL` unregisters the bindings while still returning the same storage pointer.

The applet checks the directly observable portions of this contract. For emulator execution, instrument A1C4 or the resolved clipboard descriptor and additionally assert after the unbind call:

```text
descriptor+0x24 == 0
descriptor+0x28 == 0
descriptor+0x2C == 0
descriptor+0x30 == 0
```

## Running

Open the applet and press Cmd+I. The display reports the number of failed checks, returned/mirrored storage pointers, size/capacity, cursor, and the return from the explicit unbind call.

This probe is an executable regression specification. It has not yet been claimed as passed on hardware or emulator merely by being present in the repository.
