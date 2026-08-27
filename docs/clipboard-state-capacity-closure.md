# System 3 / OS3K clipboard state and capacity — A218–A228

This note closes the mechanical ABI of the five traps immediately following
`ClipboardClear`: A218, A21C, A220, A224 and A228. Their behavior is stable at
the machine level in the November 2005 AlphaSmart 3000 ROM, November 2005 NEO
ROM, and July 2013 NEO/System 3.15 ROM.

Reliable original public System 3 names have not been recovered, so the SDK
keeps `SYS_Axxx` names rather than publishing descriptive guesses. The functional
roles are nevertheless sufficiently precise for implementation and regression.

These traps all operate on the same dedicated clipboard descriptor used by
A200–A214 and resolved elsewhere through reserved token `0x00CB`.

## 1. A218 — clipboard metadata/tag byte

```c
uint8_t SYS_A218(void);
```

The complete handler is only eight bytes in every compared ROM:

```text
move.b clipboard_tag_global, D0
rts
```

Therefore A218 returns the low byte of the shared clipboard metadata/tag field
written by A200/A204/ClipboardSet and cleared by ClipboardClear.

Important ABI detail: the handler performs a byte move into D0 and does not
explicitly clear the upper 24 bits. The contractual value is therefore the low
byte, which is why the reconstructed prototype uses `uint8_t`.

The tag taxonomy remains unnamed. Official clipboard writers commonly store
literal `1`, while a System 3.15 internal caller invokes A218 and compares the
returned byte with literal `4` before conditionally clearing the clipboard. This
confirms that the field is not merely a Boolean and that inventing a
`CLIPBOARD_TAG_TEXT = 1` constant would be premature.

Handler addresses:

- AS3000 2005: `0x4E0CEE`;
- NEO 2005: `0x5E364A`;
- NEO 2013: `0x439A4C`.

Length: `0x08` in all three.

## 2. A21C — raw or filtered clipboard logical length

```c
uint32_t SYS_A21C(uint8_t read_variant);
```

A21C returns a length/count for the current clipboard contents. It uses the same
selector convention already established for `ClipboardGet` and A208:

- `read_variant == 3` -> calculate the control-code-aware/filtered output
  length;
- every other value -> return raw `clipboard.current_size`.

The handler allocates a four-byte local and initializes it from descriptor
`+0x04` (`current_size`). On the raw path it simply returns that local.

On selector `3`, it invokes the same filter-helper family used by
`ClipboardGet`, but with a NULL destination and with the initialized local as
the in/out output limit/count:

```text
limit = clipboard.current_size
filter(clipboard.storage,
       NULL,
       clipboard.current_size,
       &limit)
return limit
```

This makes A21C the companion size-query operation required to determine how
many bytes a filtered read would emit without first providing an output buffer.
Unlike the 2005 `ClipboardGet(..., 3)` handler, A21C **does initialize** its
in/out limit local in both 2005 ROMs, so the uninitialized-local quirk documented
for A210 does not apply here.

Caller evidence:

- AlphaWord Plus 2012 contains three A21C calls; the filtered formatting path
  uses literal selector `3`;
- Calculator 2012 calls A21C with selector `1` after obtaining clipboard storage;
- AlphaWord Plus November 2005 reproduces the same raw/filtered call patterns;
- System 3.15 contains numerous internal A21C callers.

Handler addresses:

- AS3000 2005: `0x4E0CF6`;
- NEO 2005: `0x5E3652`;
- NEO 2013: `0x439A54`.

Length: `0x4A` in all three.

## 3. A220 — clipboard storage pointer

```c
uint8_t *SYS_A220(void);
```

A220 loads the dedicated clipboard descriptor and returns its `+0x00`
storage/base pointer directly. It receives no arguments and does not mutate
clipboard state.

Calculator 2012 contains a particularly clear caller: it obtains the A220
pointer, calls A21C with raw selector `1`, then adds the returned length to the
pointer to form the end of clipboard data. This independently corroborates both
A220 as storage base and A21C as logical length.

System 3.15 contains many additional internal A220 callers.

Handler addresses:

- AS3000 2005: `0x4E0D40`;
- NEO 2005: `0x5E369C`;
- NEO 2013: `0x439A9E`.

Length: `0x0A` in all three.

## 4. A224 — clipboard maximum allocated capacity

```c
uint32_t SYS_A224(void);
```

A224 loads the clipboard descriptor and returns descriptor `+0x0C`, the
confirmed `max_size`/allocated-capacity field. It receives no arguments and does
not mutate state.

AlphaWord Plus contains direct A224 callers in both the 2005 and 2012 images.
The value is used alongside current/filtered clipboard length in clipboard-related
logic, consistent with a capacity query.

Handler addresses:

- AS3000 2005: `0x4E0D4A`;
- NEO 2005: `0x5E36A6`;
- NEO 2013: `0x439AA8`.

Length: `0x0C` in all three.

## 5. A228 — resize/set clipboard logical length

```c
uint32_t SYS_A228(uint32_t requested_size);
```

A228 receives one 32-bit requested size. It invokes the shared allocator with
the dedicated clipboard descriptor, then clamps the requested size against the
resulting clipboard `max_size`.

Its effective operation is:

```text
allocator_adjust(clipboard_descriptor, requested_size)
actual = min(requested_size, clipboard.max_size)
clipboard.recoverable_size = actual
clipboard.current_size = actual
return actual
```

The handler does not copy data, does not change the clipboard tag, and does not
explicitly move cursor `+0x14`. Expanding the logical size therefore must not be
interpreted as initializing the newly exposed bytes.

No direct A228 caller was found in the five scanned 2012 SmartApplets used for
this reconstruction. System 3.15 contains internal callers, including paths that
compare the returned actual size with the requested size and react when the
allocator cannot provide the full request. That pattern independently confirms
that the return is the effective size after allocation/clamping rather than a
Boolean status.

Handler addresses:

- AS3000 2005: `0x4E0D56`;
- NEO 2005: `0x5E36B2`;
- NEO 2013: `0x439AB4`.

Length: `0x4C` in all three.

## 6. Compact reconstructed interface

```c
uint8_t  SYS_A218(void);                       /* clipboard tag byte */
uint32_t SYS_A21C(uint8_t read_variant);        /* raw/filtered length */
uint8_t *SYS_A220(void);                        /* storage pointer */
uint32_t SYS_A224(void);                        /* max allocated size */
uint32_t SYS_A228(uint32_t requested_size);     /* set/resize logical size */
```

These names deliberately remain generic. Candidate descriptive names such as
`ClipboardGetTag`, `ClipboardGetSize`, `ClipboardGetBuffer`,
`ClipboardGetCapacity` and `ClipboardSetSize` describe the observed mechanics,
but publishing them without a recovered System 3 symbol would violate the
project naming policy.

## 7. Regression matrix

The emulator-first regression should verify:

1. after `ClipboardSet`, A218 returns the low-byte tag used by the setter;
2. A21C with a non-3 selector equals raw `current_size`;
3. A21C with selector 3 equals the count produced by a sufficiently large
   filtered `ClipboardGet` on the same firmware generation;
4. A220 equals the storage pointer associated with reserved descriptor token
   `0x00CB`;
5. A224 equals descriptor `max_size`;
6. A228 shrink/grow requests return the effective clamped size and set both
   current/recoverable size without changing tag or cursor;
7. allocator-limited A228 requests are checked explicitly;
8. the filtered-length mapping is compared separately between 2005 and 2013,
   since the control-code helper contains generation-dependent values.

No emulator or hardware regression is claimed as already executed.

## 8. Boundary of this block

A218–A228 remain clipboard-specific. A22C begins a different state-selection
family: it validates/stores a 32-bit global selection value, A230 resets that
value to `-1`, and A234 maps a valid selection through a runtime pointer table to
a 16-bit header field. Those traps should therefore be reconstructed as the next
separate block rather than being forced into clipboard nomenclature.
