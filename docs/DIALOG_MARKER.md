# Dialog marker field — DialogAddItem argument 3

This note records the evidence for the `marker` argument of `A0F4 / DialogAddItem`.

## SDK contract

```c
int DialogAddItem(char* text,
                  uint8_t text_len,
                  char marker,
                  int id,
                  Key_e shortcut_key,
                  size_t file_size);
```

**`marker` confidence: A — confirmed.**

`marker` is a caller-provided one-byte literal glyph. The System 3 dialog code stores the byte without semantic validation and `DialogDraw` later sends that same byte directly to `A010 / PutChar` while rendering the item.

It is presentation metadata only. It does not select an item, terminate `DialogRun`, alter the caller `id`, validate a shortcut, or control `file_size`.

A space (`' '`, `0x20`) is the normal blank marker. Official code also uses visible glyphs such as `'*'`, `'+'`, and `'x'`.

## Historical AS3000 lineage

The March 2000 simulator object for `DialogModule.c` exposes:

```c
int DialogMenuAddItem(char *text, uint8_t text_len, char marker);
```

Its machine code stores the low byte of the third argument directly into the per-item marker array. This establishes the original meaning of the field as a literal per-item character rather than an enum or flag set.

The historical dialog implementation therefore already had the conceptual relation:

```c
item.marker = marker;
```

The later OS3K ABI preserves that field while adding `id`, `shortcut_key`, and `file_size`.

## Direct System 3 firmware evidence

The same core path is present in all three firmware generations inspected:

- AS3000 System 3, November 2005;
- NEO System 3, November 2005;
- NEO System 3, July 2013.

### A0F4 writer

After its normal register-save prologue, each analyzed `A0F4` handler reads the low byte of the third C argument and writes it to a per-item byte array indexed by the current insertion count. There is no range check, translation table, mask, or special-case branch around the marker value.

Conceptually:

```c
markers[item_count] = (uint8_t)marker;
```

The physical RAM addresses differ between firmware builds and are intentionally not exposed by BetaWise.

### DialogDraw reader

A reference scan of each firmware's marker-array base finds the expected writer in `A0F4` and the rendering reader reached from `A100 / DialogDraw`.

In that rendering helper the firmware loads the stored marker byte, zero-extends it, pushes it as the argument, and calls the same implementation referenced by `A010 / PutChar`.

The semantic operation is therefore directly equivalent to:

```c
PutChar(markers[item_index]);
```

No marker-dependent branch was found in the dialog input/navigation path. This is why a marker should be treated as a glyph, not as a dialog-state flag.

The 2013 NEO implementation retains the same writer/reader structure as the two 2005 images, which strengthens the conclusion that this is a stable System 3 ABI property rather than a one-build accident.

## Official usage

Official callers provide independent usage evidence for the same interpretation.

The 2005 Control Panel SmartApplet commonly supplies a space marker (`0x20`). Internal System 3 code contains a call path that chooses between `'+'` (`0x2B`) and `' '` (`0x20`) for the third argument.

The 2012 `keywords.os3kapp` binary contains multiple `A0F4` callers in which the third-argument stack slot is selected from literal display characters. Observed values include:

```text
' '   0x20
'*'   0x2A
'+'   0x2B
'x'   0x78
```

Those values are passed in exactly the position consumed by the `marker` byte of `A0F4`; they are not shortcut-key values. This demonstrates real later-OS3K use of non-blank markers.

The application-specific meaning of `*`, `+`, or `x` belongs to the caller. The dialog subsystem itself does not interpret those characters.

## SDK guidance

Use a printable character when a visible status marker is desired and a space when no marker should be visible:

```c
DialogAddItem("Normal", 6, ' ', 100, KEY_N, (size_t)-1);
DialogAddItem("Marked", 6, '*', 200, KEY_M, (size_t)-1);
```

Do not introduce a marker enum unless a higher-level BetaWise wrapper intentionally defines application semantics. The raw OS3K API is correctly represented by `char marker`.

Because the firmware does not validate the byte, unusual/non-printable values are forwarded to `PutChar`; their useful visual behavior should not be assumed portable across fonts or firmware.

## Emulator regression

The emulator should eventually verify at least:

```text
marker=' '  -> blank marker position
marker='*'  -> literal '*'
marker='+'  -> literal '+'
marker='x'  -> literal 'x'
```

The trace should show the `A0F4` third argument and the corresponding `PutChar` value during `DialogDraw`. Changing the marker must not change `DialogGetChoice`, `DialogGetChoiceId`, shortcut behavior, exit-key behavior, or file-size metadata.

This is now a regression target, not a discovery experiment.
