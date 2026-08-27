# System 3 / OS3K clipboard buffer API — A20C–A214

This note closes the mechanical ABI of the named System 3 clipboard buffer
routines A20C `ClipboardSet`, A210 `ClipboardGet`, and A214 `ClipboardClear`.
The analysis was compared directly across the November 2005 AlphaSmart 3000
ROM, the November 2005 NEO ROM, and the July 2013 NEO/System 3.15 ROM, and was
cross-checked against official SmartApplet callers and the already reconstructed
A200/A204/A208 clipboard editing block.

The names `ClipboardSet`, `ClipboardGet`, and `ClipboardClear` predate this
reconstruction in BetaWise. Their functional roles now match the firmware
mechanics directly. Exact private enum names for the clipboard metadata byte and
read selector have not been recovered, so this document deliberately uses the
neutral terms `clipboard_tag` and `read_variant`.

## 1. Shared clipboard state

System 3 owns a dedicated clipboard descriptor. The shared File API resolver
recognizes reserved token `0x00CB` as this descriptor. The descriptor uses the
same core field layout as ordinary files, including:

- `+0x00`: storage/base pointer;
- `+0x04`: current logical size;
- `+0x08`: previous/recoverable size;
- `+0x0C`: maximum allocated capacity;
- `+0x14`: cursor.

A separate byte-sized global accompanies the clipboard contents. A200, A204 and
A20C write it; A214 clears it; neighboring A218 returns it directly. This note
calls that byte `clipboard_tag` because no reliable original public enum name
has been recovered.

Observed evidence prevents reducing the field to a guessed Boolean or a guessed
"text" flag. Official A20C callers commonly store literal `1`, while a System 3
internal caller reads the A218 result and compares it with literal `4`. The
numeric values are therefore real metadata values, but their complete taxonomy
remains open.

## 2. A20C — `ClipboardSet`

```c
int32_t ClipboardSet(const uint8_t *buffer,
                     uint32_t count,
                     uint8_t clipboard_tag);
```

The three ABI slots are a 32-bit source pointer, a 32-bit count, and a third
slot whose low byte is consumed as `clipboard_tag`.

Validation is mechanically closed:

- `buffer == NULL` -> raw `-6`;
- `count > 0x7FFFFFFF` -> raw `-128`;
- `clipboard_tag == 0` -> raw `-129`.

A valid call performs the following operation:

1. loads the dedicated clipboard descriptor;
2. clears state bit 2 at descriptor byte `+0x23`;
3. adjusts/reclaims clipboard allocation through the shared File allocator;
4. limits the actual transfer against the resulting `max_size`;
5. stores `clipboard_tag` in the shared clipboard-tag global;
6. copies `actual` bytes from `buffer` to clipboard storage;
7. writes `actual` to both clipboard `current_size` (`+0x04`) and
   `previous/recoverable_size` (`+0x08`);
8. returns `actual`.

Conceptually:

```text
actual = min(count, clipboard.max_size_after_allocation)
clipboard_tag_global = clipboard_tag
memcpy(clipboard.storage, buffer, actual)
clipboard.current_size = actual
clipboard.recoverable_size = actual
return actual
```

`count == 0` is accepted when the pointer is non-NULL and the tag is non-zero.
The handler does not itself reset clipboard cursor `+0x14`; `ClipboardClear`
performs that operation.

### Caller evidence

The official 2012 Calculator, ControlPanel and AlphaWord Plus images contain
A20C callers. The AlphaWord Plus image contains four such calls. The analyzed
callers pass literal tag `1` and clean three 32-bit argument slots after the
trap. The November 2005 AlphaWord Plus image contains the same call shape, so
this is not a late ABI addition.

System 3.15 also contains internal direct A20C callers that use literal tag `1`.

## 3. A210 — `ClipboardGet`

```c
int32_t ClipboardGet(uint8_t *buffer,
                     uint32_t count,
                     uint8_t read_variant);
```

The three ABI slots are a 32-bit destination pointer, a 32-bit count, and a
third slot whose low byte selects the transfer path.

Common validation:

- `buffer == NULL` -> raw `-6`;
- `count > 0x7FFFFFFF` -> raw `-128`.

The selector test is exact:

- `read_variant == 3` -> control-code-aware/filtered path;
- every other value -> ordinary/raw path.

### Raw path

The raw path is stable across all three compared ROMs:

```text
actual = min(count, clipboard.current_size)
memcpy(buffer, clipboard.storage, actual)
return actual
```

A System 3.15 internal caller uses literal `1`, confirming at least one observed
non-3 value on the raw path.

### Filtered path

The alternate helper is not a plain `memcpy`. It scans source bytes through an
internal control-code table/switch, can consume or omit control sequences, and
can emit visible equivalents such as hyphen, tab or space. AlphaWord Plus uses
literal `3` in both the 2005 and 2012 images.

The helper's machine contract is important: argument 3 specifies source length;
argument 4 points to an in/out output limit/count. It stops when either source
is exhausted or emitted output reaches the initial pointed-to limit, then writes
the emitted count back through that pointer.

### 2005 filtered-path firmware quirk

The November 2005 AS3000 and NEO A210 handlers are 0x7E bytes and are logically
identical. In their `read_variant == 3` path they allocate a four-byte local but
do **not initialize that local before passing its address as the filter helper's
in/out output limit**. They pass caller `count` as the helper's source length.

In other words, the 2005 path is mechanically equivalent to:

```text
uint32_t output_limit;              /* uninitialized firmware local */
filter(clipboard.storage,
       buffer,
       count,                       /* source length */
       &output_limit);              /* in/out output limit/count */
return output_limit;
```

This is a genuine implementation quirk of the shipped 2005 handlers, not a
portable SDK guarantee that should be normalized away in documentation.

### NEO 2013 correction

The July 2013 A210 handler grows to 0x82 bytes and explicitly initializes the
local with caller `count`. It also passes `clipboard.current_size` as the source
length. Its filtered behavior is therefore coherent:

```text
uint32_t output_limit = count;
filter(clipboard.storage,
       buffer,
       clipboard.current_size,
       &output_limit);
return output_limit;
```

The filter helper itself also contains generation-dependent control-code values,
so exact formatted output should not be assumed byte-identical between 2005 and
2013 merely because selector `3` exists in both.

**SDK consequence:** the raw path has a portable deterministic size contract in
the compared ROMs. Selector `3` is mechanically identified as the filtered path,
but its exact truncation/output behavior is generation-sensitive and should be
regressed explicitly before an application depends on identical cross-generation
results.

## 4. A214 — `ClipboardClear`

```c
void ClipboardClear(void);
```

A214 consumes no caller arguments. In all three compared ROMs it performs the
same logical sequence:

1. loads the dedicated clipboard descriptor;
2. clears `previous/recoverable_size` (`+0x08`);
3. clears `current_size` (`+0x04`);
4. clears cursor (`+0x14`);
5. invokes shared allocator/reclaim maintenance;
6. clears the shared `clipboard_tag` byte;
7. returns.

It does **not** explicitly zero-fill the clipboard storage bytes. Therefore
`ClipboardClear` is a logical/allocation-state clear, not a secure erase
primitive.

The handler does not establish a meaningful public result in D0, so the public
prototype remains `void`.

Official Calculator, ControlPanel and AlphaQuiz images contain no-argument A214
calls whose return value is ignored. System 3.15 contains additional internal
no-argument callers.

## 5. Cross-ROM handler map

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 | Length |
| --- | ---: | ---: | ---: | ---: |
| A20C | `0x4E0B8A` | `0x5E34E6` | `0x4398E4` | `0xAE` |
| A210 | `0x4E0C38` | `0x5E3594` | `0x439992` | `0x7E` / `0x7E` / `0x82` |
| A214 | `0x4E0CB6` | `0x5E3612` | `0x439A14` | `0x38` |

A20C and A214 retain equivalent state transitions across the three generations.
A210 retains the same public selector split, but the filtered path contains the
explicit 2005-vs-2013 difference documented above.

## 6. Relation to A200/A204/A208

This block closes the buffer-oriented companion operations to the previously
reconstructed clipboard editing primitives:

- A200 copies from the active file into clipboard storage;
- A204 cuts by copying and then deleting exactly the copied count;
- A208 pastes clipboard contents through A198 write modes;
- A20C sets clipboard contents from an arbitrary caller buffer;
- A210 gets clipboard contents into an arbitrary caller buffer;
- A214 clears clipboard logical/allocation state.

A200/A204 and A20C share the same non-zero clipboard-tag global. A208 and A210
share the same `read_variant == 3` filtered helper family. This cross-correlation
is independent of the inherited BetaWise names.

## 7. Regression and safety policy

A minimum emulator-first regression matrix is:

1. `ClipboardSet` with a small buffer and tag `1`: verify returned count,
   current/recoverable size and stored bytes.
2. `ClipboardSet` errors: NULL source, count above signed-positive range, and
   zero tag.
3. `ClipboardGet` raw path with a non-3 selector: verify clamping to current
   clipboard size and exact byte copy.
4. `ClipboardClear`: verify current/recoverable size, cursor and tag become zero,
   while not asserting that backing bytes were securely wiped.
5. `ClipboardGet(..., 3)` on NEO 2013: verify output-capacity semantics and the
   actual filtered byte mapping.
6. Run the equivalent selector-3 case separately on AS3000/NEO 2005 and capture
   the shipped uninitialized-local behavior rather than silently emulating the
   2013 fix.
7. Correlate A200/A204/A208 with A20C/A210/A214 against the same clipboard
   descriptor and tag state.

The clipboard routines mutate state, but the regression can be performed with
disposable emulator snapshots. No emulator or hardware regression is claimed as
executed by this document.

## 8. SDK decision

`ClipboardSet`, `ClipboardGet`, and `ClipboardClear` are now mechanically closed
and suitable for publication as reconstructed SDK interfaces. The names already
existed in BetaWise and are retained because direct firmware behavior now
matches them.

No `CLIPBOARD_TAG_*` or `CLIPBOARD_READ_*` enum is introduced yet. Numeric
behavior is documented without inventing symbolic names. The neighboring A218
trap is already mechanically recognizable as a byte-sized getter for the shared
clipboard tag and is the natural next member to close in the sequence.
