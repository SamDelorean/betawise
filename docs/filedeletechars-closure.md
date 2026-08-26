# A1A4 / FileDeleteChars ABI closure

Status: **closed for SDK use**.

- Trap: `A1A4`
- Public name: `FileDeleteChars`
- Mechanical ABI confidence: **A**
- Historical-name continuity confidence: **B+ / strong**

```c
uint32_t FileDeleteChars(uint32_t delete_count);
```

## Firmware behavior

A1A4 operates on the currently active file descriptor (resolver token `0`). If no active descriptor resolves, it returns `0`.

For a valid active descriptor:

```text
available = current_size - cursor
actual = min(delete_count, available)

if actual != 0:
    memmove(storage + cursor,
            storage + cursor + actual,
            available - actual)
    current_size -= actual

sync_live_file_info_mirrors()
return actual
```

Confirmed consequences:

- deletion starts at the current cursor;
- the cursor itself is not moved;
- requests extending past EOF are clamped;
- the return is the actual number of bytes/characters removed;
- `previous/recoverable_size` is not reduced, preserving the recoverable/high-water extent;
- trailing bytes beyond the new logical EOF are not zero-filled;
- zero-length deletion is a no-op returning zero;
- A1A4 itself contains no explicit read-only test in the analyzed handler.

## Cross-firmware stability

The handler is exactly `0x70` bytes long in all three firmware versions analyzed:

| Firmware | Handler | File offset |
| --- | ---: | ---: |
| AS3000 System 3, Nov 2005 | `0x4E1632` | `0x21632` |
| NEO System 3, Nov 2005 | `0x5E3F8E` | `0x23F8E` |
| NEO System 3, Jul 2013 | `0x43A390` | `0x2A390` |

Of the 112 handler bytes, **107 are identical** across all three images. The five differing byte positions are relocation/call-displacement or trace-number differences; the data-flow and descriptor mutations are unchanged.

## Direct System 3 caller evidence

No direct A1A4 caller was found in the five official 2012 SmartApplets examined (AlphaWord Plus, AlphaQuiz, KeyWords, Calculator and ControlPanel). The syscall is nevertheless used internally by System 3.

In NEO System 3 (Jul 2013), the internal `A204` handler contains the decisive caller. That routine works with the dedicated clipboard descriptor, copies bytes beginning at the active file's `storage + cursor` into clipboard storage, updates the clipboard size, and then invokes:

```text
A1A4(delete_count)
```

with the same count. Mechanically this is a cut/delete sequence: copy the selected bytes to clipboard, then remove those bytes from the active file.

This is strong independent evidence that A1A4 is a character-range deletion primitive rather than a generic resize or accounting operation.

## Historical AS3000 relationship

The original AS3000 `FileModule.c` (1998-2000) contains:

```c
void FileDeleteChars(UInt32 new_cursor_position, UInt32 delete_count)
{
    gFileActiveInfo->ulCursor = new_cursor_position;
    for (...) {
        FileRemoveChar();
    }
}
```

`FileRemoveChar()` shifts the remaining contents left and decrements the logical size, which is the same core operation implemented directly and in bulk by A1A4.

The modern System 3 ABI evolved in two visible ways:

1. cursor positioning is no longer an argument to A1A4; deletion uses the current descriptor cursor;
2. the modern syscall returns the actual number of bytes deleted, allowing EOF clamping to be observed by the caller.

Therefore the historical name `FileDeleteChars` is retained as the best-supported public name even though the ABI is not byte-for-byte identical to the 2000 source API.

## SDK decision

A1A4 is promoted from `SYS_A1A4` to:

```c
uint32_t FileDeleteChars(uint32_t delete_count);
```

The name is genealogical rather than recovered from modern symbol data, so documentation should preserve the distinction: **behavior/ABI is confidence A; exact original modern symbol name is confidence B+**.

## Emulator regression contract

A future automated regression should create an active test file containing a known byte sequence and check at least:

1. deleting `N` bytes in the middle shifts the suffix left by exactly `N` bytes;
2. cursor is unchanged;
3. current size decreases by `N`;
4. recoverable/high-water size remains unchanged;
5. deleting beyond EOF returns only the available byte count;
6. deleting zero returns zero and leaves contents unchanged;
7. live `FileGetFileInfo` mirrors receive the new current size;
8. no-active-descriptor returns zero without touching arbitrary storage.

This regression is intentionally specified before execution; no claim is made here that the probe has already run on hardware or the emulator.
