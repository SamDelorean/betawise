# System 3 / OS3K SmartApplet runtime API closure — A238–A244

This note closes the mechanical ABI of the four named SmartApplet runtime traps
immediately following the selected-applet context block A22C–A234:

- `A238` — `AppletFindByName`
- `A23C` — `AppletFindById`
- `A240` — `AppletGetName`
- `A244` — `AppletSendMessage`

The compared implementations are the November 2005 AlphaSmart 3000 ROM, the
November 2005 NEO ROM, and the July 2013 NEO/System 3.15 ROM. The function names
already existed in BetaWise and are consistent with the handlers and official
callers. Mechanical confidence is A. Name continuity is strong, although no
matching public source-level implementation was recovered from the earlier
1998–2000 monolithic AS3000 source tree.

## Shared runtime structures

All four functions use the 32-entry runtime SmartApplet table. Each installed
entry points to an `AppletHeader_t`.

Relevant header fields are:

- `+0x14`: 16-bit SmartApplet ID;
- `+0x18..+0x3b`: 36-byte name field;
- `+0x84`: applet entry-point offset used by the runtime dispatcher.

Runtime table bases differ only by generation:

| ROM | applet-header pointer table |
| --- | ---: |
| AS3000 2005 | `0x00000D82` |
| NEO 2005 | `0x00000DDA` |
| NEO 2013 | `0x00000E0A` |

The normal runtime index domain is `0..31`. Index 0 is the System applet and is
also used by lookup routines as their not-found sentinel, so callers must account
for that ambiguity.

## A238 — AppletFindByName

Reconstructed public form:

```c
uint32_t AppletFindByName(const char *name, uint32_t start_index);
```

The handler consumes two full 32-bit stack slots. `name` is read-only and must
point to a valid NUL-terminated string; the handler calls `strlen(name)` before
performing any NULL check, so NULL is not a safe input.

The search begins at `start_index + 1`, making `start_index` exclusive. It then
scans runtime indices while the resulting index is below 32. NULL table entries
are skipped.

For every installed candidate it compares:

```text
strncmp(candidate_header->name, name, strlen(name))
```

Therefore A238 is mechanically a **prefix search**, not an exact-name equality
function. Consequences include:

- `"Alpha"` can match `"AlphaWord Plus"`;
- an empty string matches the first installed candidate after `start_index`;
- `start_index == 31` performs no candidate comparison and returns 0;
- the return is the full 32-bit runtime index, although valid matches remain in
  the range 0..31;
- no match returns full `D0 = 0`.

The implementation uses the same `strlen` and `strncmp` library helpers in all
three compared ROMs. System 3 internal callers observed in NEO 2013 pass a zero
start value to search the ordinary SmartApplet slots beginning at index 1.

The older BetaWise declaration used `uint8_t` for both `start_index` and return.
Those widths were sufficient for common callers but do not describe the machine
contract: the handler loads, increments, compares and returns the index as a
longword.

## A23C — AppletFindById

Reconstructed public form:

```c
uint32_t AppletFindById(uint16_t id);
```

A23C scans installed runtime entries from index 0 through 31. For each non-NULL
header pointer it compares the requested low word with the word at
`AppletHeader_t + 0x14`.

On match it returns the complete 32-bit runtime index. If no match exists it
returns full `D0 = 0`.

This means index 0 is intrinsically ambiguous: a successful lookup of the System
applet's ID can return 0, and not-found also returns 0. Ordinary third-party
callers normally use nonzero applet IDs and treat a positive index as found.

The existing `uint16_t id` argument is correct. The old `uint8_t` return is
widened to `uint32_t` to reflect the explicit longword result produced by the
handler.

## A240 — AppletGetName

Reconstructed public form:

```c
uint8_t AppletGetName(uint32_t index, char *name_out);
```

A240 consumes a full 32-bit runtime index and an output pointer.

Validation is explicit:

- index 0 is rejected;
- index >= 32 is rejected;
- an uninstalled/NULL runtime table entry is rejected.

On success, the function copies bytes from header `+0x18` and NUL-terminates the
caller buffer. The source-side limit is header `+0x3c`, so at most the complete
36-byte SmartApplet name field is copied. A caller that wants to support the
maximum non-NUL-filled header name safely should provide at least 37 writable
bytes.

The function also carries a 40-byte destination-side guard inherited by the
implementation, but the 36-byte header-field boundary is reached first for a
normal `AppletHeader_t` name.

Return behavior is important: success executes `MOVEQ #1,D0`, but failure clears
only the low byte of D0. Therefore only an 8-bit Boolean-like return is
contractual. The old BetaWise `int` declaration could expose stale upper D0 bits
on a failed call and is corrected to `uint8_t`.

The handler does not provide a useful NULL-output path for a valid installed
index; `name_out` must reference writable memory.

## A244 — AppletSendMessage

Reconstructed public form:

```c
uint8_t AppletSendMessage(
    uint32_t index,
    Message_e message,
    uint32_t param,
    uint32_t *status);
```

The handler consumes four 32-bit argument slots. The runtime index and message
are both handled internally as longwords. `Message_e` remains the public SDK type
for source compatibility with `ProcessMessage`, but the underlying A244 message
slot is 32 bits.

### Target validation

For a nonzero target index A244 requires:

- index < 32;
- a non-NULL applet-header table entry;
- a zero per-applet selection-block mask, using the same mask helper documented
  for A22C.

Failure clears only the low byte of D0 and returns without changing the active
runtime context. Index 0 is deliberately special: the handler bypasses the
ordinary range/table/block-mask validation path and proceeds through the System
applet dispatch path.

As with A240, the byte-only failure write makes `uint8_t` the portable return
contract. Successful dispatch returns full `D0 = 1`.

### Message normalization

Two raw message values, `0x1C` and `0x1D`, are passed through unchanged. Their
original symbolic names have not been recovered and are intentionally not added
to `Message_e` yet.

Every other message is transformed before delivery as:

```text
normalized = (message & 0x00FFFFFF) | 0x01000000
```

This directly explains the existing BetaWise `MSG_MOD_SYNTHETIC = 0x01000000`
namespace and confirms that A244 is the normal mechanism for private/synthetic
cross-applet messages. System 3 internal callers pass small selectors such as 2,
3, 4 and 5; A244 converts them to `0x01000002`, `0x01000003`, etc. Independent
BetaWise/neo-re usage likewise finds the font applet and sends private message
`0x01000002` through this function.

### Entry point and A5 context switching

A244 obtains the target header from the runtime table, reads the dword at
header `+0x84`, and combines it with the header base to obtain the runtime entry
address.

Before invoking the target, it installs the target's per-applet A5 context. The
helper used for this operation is only three instructions in each compared ROM:

```text
MOVE.L  A5,D0
MOVEA.L 4(SP),A5
RTS
```

It therefore returns the previous A5 while installing the requested A5 value.
A244 saves that previous value, calls the target applet callback, and restores A5
afterward. This is direct firmware evidence for the long-standing BetaWise build
rule `-ffixed-a5`: A5 is owned by the OS3K applet runtime and must not be treated
as a general compiler register.

A244 also saves the previous current-applet runtime index, installs the target
index for the duration of the callback, and restores the previous index before
returning.

The target callback is invoked with the normal three-argument ABI:

```c
ProcessMessage(normalized_message, param, status);
```

The caller-provided `status` pointer is forwarded unchanged. A244 itself does not
make it safe to pass NULL; whether NULL is acceptable depends on the receiving
applet callback. SDK callers should provide a valid `uint32_t` status object
unless the target applet's private contract explicitly states otherwise.

## Cross-ROM handler map

The implementations are structurally identical across the three compared ROMs.
Differences are confined to relocated helper/global/table addresses.

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 | Length |
| --- | ---: | ---: | ---: | ---: |
| A238 | `0x4E0DFA` | `0x5E3756` | `0x439B58` | `0x64` |
| A23C | `0x4E0E5E` | `0x5E37BA` | `0x439BBC` | `0x38` |
| A240 | `0x4E0E96` | `0x5E37F2` | `0x439BF4` | `0x6C` |
| A244 | `0x4E0F02` | `0x5E385E` | `0x439C60` | `0xAA` |

The only byte differences inside corresponding handlers are the expected
relocations for table bases, global runtime state, block-mask helper and A5
context-switch helper.

## Naming and historical boundary

The four names predate this reconstruction in BetaWise and match both behavior
and independent modern usage. They are retained as the SDK names.

The original 1998–2000 AS3000 source tree represents the earlier monolithic
applet architecture and does not provide a source-level one-to-one implementation
of these later OS3K runtime services. That absence is not treated as evidence
against the names; it simply means the name genealogy is not claimed to be a
direct ABI continuity from the early source generation.

## Safety and regression matrix

A238, A23C and A240 are suitable for read-only regression when called with valid
pointers and normal indices. A244 executes another applet's callback and can
therefore have target-specific side effects; validate it emulator-first.

Minimum regression matrix:

1. A23C: resolve a known installed nonzero ID and compare the returned runtime
   index with the runtime table;
2. A23C: missing ID -> 0;
3. A238: exact full name and a shorter prefix resolve the same first matching
   applet after `start_index`;
4. A238: verify the exclusive start-index rule and `start_index=31 -> 0`;
5. A240: valid installed index copies the header name and returns low byte 1;
6. A240: index 0, index 32 and an uninstalled slot return low byte 0 without
   treating the upper D0 bits as contractual;
7. A244: rejected/blocked nonzero target returns low byte 0 and preserves the
   previous runtime/A5 context;
8. A244: send a known benign private message to a cooperating applet, verify the
   `0x01000000` normalization, callback arguments and low-byte success result;
9. A244: verify A5 and current-applet context are restored after callback return;
10. do not use arbitrary private message selectors on irreplaceable hardware.

No emulator or hardware regression is claimed as already executed by this note.
