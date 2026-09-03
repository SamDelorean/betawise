# A1C0 / FileSetFolder — source-first closure

Status: mechanically and semantically closed against the three canonical System 3 firmware generations.

## Contract

```c
int32_t FileSetFolder(uint32_t applet_index, uint32_t *applet_flags_out);
```

`applet_index` is the runtime SmartApplet index in the range 0..31. It is not an `AppletHeader.id` and not a File API `file_id`. `applet_flags_out` is optional. When non-NULL, the handler clears `*applet_flags_out` to zero before validation and, on success, stores the selected `AppletHeader_t.flags` longword.

## Confirmed behavior

- `applet_index >= 32` returns `-64` without changing the current folder.
- An in-range runtime slot whose companion word is `0xFFFF` also returns `-64` without changing the current folder.
- After resolving the selected applet header, bit `0x40` of the low flags byte controls an additional empty-group validation. If that bit is clear and the selected runtime group has `file_count == 0`, the call returns `-7` without changing the current folder.
- Only after all validation succeeds does the handler invoke A1CC, close/release the previously active File API context, save the previous `current_folder`, install `applet_index` as the new folder, optionally copy the complete flags longword from the applet header, and return the previous folder index in `D0.L`.

The A1CC branch target was checked directly in all three canonical images: AS3000 `0x004E1C48`, NEO 2005 `0x005E45A4`, NEO 2013 `0x0043A9A6`.

## Evidence summary

The public A1C0 handler is exactly `0xA4` bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). There are seven direct absolute internal JSR callers in each firmware image (7/7/7). The handler structure is equivalent across generations; differences are relocations of tables/globals/helpers plus diagnostic metadata, with no observed semantic divergence.

The AlphaQuiz and ControlPanel callers remain useful source-first anchors: AlphaQuiz uses the returned previous-folder value to restore context, while ControlPanel supplies the flags output and tests bit `0x40`. These callers corroborate rather than define the firmware-derived contract.

## Refutation

The firmware refutes the interpretations that A1C0 is a `void` setter, that it consumes an applet ID/file token instead of a runtime index, or that the second argument is mandatory. It also confirms that the `-64` and `-7` paths return before A1CC and therefore do not close the active context.

## Confidence

- **CONFIRMED:** ABI; runtime-index domain; optional flags output; `-64` validation paths; `-7` empty-group path; flags bit `0x40` test; A1CC invocation only on success; previous/current folder handling; returned previous folder; equivalent semantics across all three canonical firmware generations.
- **STRONG INFERENCE:** continuity of the historical/vendor-facing name `FileSetFolder`.
- **UNKNOWN:** original vendor symbolic names for errors `-64`/`-7` and flag bit `0x40`.

Static regression against the three canonical images: **70/70 PASS**. Dynamic/emulator verification is specified separately and is not claimed as executed here.
