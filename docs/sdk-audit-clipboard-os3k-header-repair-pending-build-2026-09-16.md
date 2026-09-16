# SDK audit: Clipboard public header repair staged

## Status

`PENDING_M68K_BUILD`

The A20C/A210/A214 source graph is now closed sufficiently to stage the public-header repair without changing syscall classification:

- `syscall.c` exports direct veneers named `ClipboardSet`, `ClipboardGet`, and `ClipboardClear`.
- `os3k.c` has no Clipboard wrapper/adaptation layer.
- `file_clipboard_buffer.h` records the mechanically closed signatures.
- historical BetaWise research already carried these three names; they are not newly invented neutral names.
- `os3k.h` is the historical public SDK declaration surface and currently omits all three declarations.

Therefore the architecture-correct repair is a direct declaration in `os3k.h`, adjacent to the File API surface. Do **not** include `file_clipboard_buffer.h` from `os3k.h`.

## Exact pending header delta

```diff
 void FileClose(void);
+
+// Replace clipboard contents from a caller buffer. Returns the actual byte
+// count on success or a negative System 3 status.
+int32_t ClipboardSet(const uint8_t* buffer,
+    uint32_t count, uint8_t clipboard_tag);
+
+// Copy clipboard contents to a caller buffer. read_variant 3 selects the
+// control-code-aware path; other values select raw copying.
+int32_t ClipboardGet(uint8_t* buffer,
+    uint32_t count, uint8_t read_variant);
+
+// Clear clipboard logical/allocation state. This is not a secure erase.
+void ClipboardClear(void);
 
 // Interactive file-password verification.
```

The declarations intentionally match `file_clipboard_buffer.h` exactly in return type, pointer constness, argument widths and argument order.

## Build gate

Because this changes the public header, it must not be called `BUILD_VALIDATED` until an actual m68k SDK build is run. Required gate:

```sh
make -C os3k clean all
```

followed by a clean build of at least one representative applet against the updated SDK.

Until that build can be executed, the repair remains staged as `PENDING_M68K_BUILD` rather than being silently treated as validated.

No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted and no closed syscall is reopened by this staging note.