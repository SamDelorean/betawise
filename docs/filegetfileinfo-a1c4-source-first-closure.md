# A1C4 / FileGetFileInfo — source-first closure

Status: mechanically and semantically closed against the three canonical System 3 firmware generations.

## Contract

```c
uint8_t *FileGetFileInfo(
    uint16_t file_id,
    uint8_t **storage_out,
    uint32_t *current_size_out,
    uint32_t *max_size_out,
    uint32_t *cursor_out);
```

The call resolves `file_id` through the common File API descriptor resolver. On success it returns `descriptor->storage`, not the internal descriptor pointer.

The four optional pointer arguments are persistent live bindings. A1C4 stores them in the descriptor and invokes the common synchronization helper immediately. Later File API mutations use the same helper to keep registered storage/current-size/max-size/cursor mirrors synchronized. Passing four `NULL` pointers therefore unregisters the mirrors rather than merely requesting no one-shot outputs.

## Confirmed descriptor relation

```text
+0x00 storage
+0x04 current_size
+0x0C max_size
+0x14 cursor
+0x24 storage mirror binding
+0x28 current_size mirror binding
+0x2C max_size mirror binding
+0x30 cursor mirror binding
```

If resolution does not produce a descriptor, the handler leaves the bindings untouched and returns the initialized NULL storage result.

## Evidence summary

The public A1C4 handler is exactly `0x5A` bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). Normalizing four relocated/diagnostic operands yields an equivalent body across all three generations.

Direct absolute internal JSR callers: **52 / 52 / 55** (AS3000 / NEO 2005 / NEO 2013). Numerous official callers explicitly pass four NULL pointers, while others register actual mirrors; the additional NEO 2013 callers do not change the ABI.

Historical `FileModule.c` strongly supports the name and central semantics: its `FileGetFileInfo` returned storage and exposed size/cursor/max outputs. The modern OS3K ABI adds `file_id` and persistent bindings.

## Refutation

- The return value is not the internal descriptor; firmware loads the returned pointer from descriptor offset `+0x00`.
- The four pointers are not mandatory.
- The pointers are not merely one-shot outputs: they are written into descriptor binding fields and synchronized by a helper shared with subsequent File API mutation paths.

## Confidence

- **CONFIRMED:** five-argument ABI; common resolver; successful return of storage; descriptor field mapping; persistent mirror registration; unregister-by-NULL behavior; equivalent semantics across the three canonical firmware generations.
- **STRONG INFERENCE:** continuity of the vendor-facing name `FileGetFileInfo` from the historical implementation.
- **UNKNOWN:** complete resolver error/status catalog; internal descriptor type remains private.

Static regression against the three canonical images: **52/52 PASS**. The existing FileGetFileInfo probe remains an executable specification; dynamic/emulator execution is not claimed here.
