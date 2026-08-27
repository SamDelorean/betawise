# System 3 / OS3K applet-selection context — A22C–A234

This note closes the mechanical ABI of the three traps immediately before the
named Applet lookup/message block (`AppletFindByName`, `AppletFindById`,
`AppletGetName`, `AppletSendMessage`). The compared implementations are the
November 2005 AlphaSmart 3000 ROM, November 2005 NEO ROM, and July 2013
NEO/System 3.15 ROM.

Reliable original public names for A22C, A230 and A234 have not been recovered.
The SDK therefore keeps `SYS_A22C`, `SYS_A230` and `SYS_A234` rather than
publishing descriptive guesses. Mechanically, the block manages one global
**selected SmartApplet runtime index**, rejects applets carrying any selection-
blocking state, clears the selection to `-1`, and maps the selected runtime index
back to the applet header's 16-bit ID.

## 1. Runtime structures used by the block

Three structures are sufficient to explain all three handlers.

### Selected runtime-index global

A22C writes and A230 clears one 32-bit global. A234 reads the same global.

| ROM | selected-index global |
| --- | ---: |
| AS3000 2005 | `0x00003560` |
| NEO 2005 | `0x000035B8` |
| NEO 2013 | `0x000035F0` |

The cleared sentinel is exactly `0xFFFFFFFF` (`-1`).

### Per-applet selection-block mask table

A22C calls a tiny helper that computes `base + runtime_index * 4` and returns the
32-bit value stored there.

| ROM | block-mask table base | getter helper |
| --- | ---: | ---: |
| AS3000 2005 | `0x0000344E` | `0x4DF664` |
| NEO 2005 | `0x000034A6` | `0x5E1FC4` |
| NEO 2013 | `0x000034DE` | `0x4383C2` |

The surrounding System 3 code initializes this as 32 longwords and the normal
Applet routines iterate runtime indices `0..31`.

The neutral term **selection-block mask** is justified directly by internal
behavior:

- A22C accepts an index only when the corresponding longword is zero;
- the NEO 2013 helper at `0x4383D4` sets bit 0 of one entry;
- the helper at `0x4383EA` clears an entire entry;
- System 3 UI paths that locate applet IDs `0xA005` and `0xA002` use those
  operations as disable/enable behavior;
- neighboring helpers OR and AND-NOT additional dynamically selected bits into
  the same longwords, so the field is wider than a single Boolean disabled flag.

The complete private bit taxonomy is not recovered, so the SDK does not publish
a guessed enum.

### Runtime applet-header pointer table

A234 maps the selected runtime index through the same 32-entry pointer table used
by the named Applet lookup functions. The table base is generation-dependent:

| ROM | applet-header pointer table |
| --- | ---: |
| AS3000 2005 | `0x00000D82` |
| NEO 2005 | `0x00000DDA` |
| NEO 2013 | `0x00000E0A` |

Each non-NULL entry points to an `AppletHeader_t`. A234 reads the word at header
`+0x14`, independently matching the established 16-bit SmartApplet ID field and
the neighboring `AppletFindById` implementation.

## 2. A22C — install a selected runtime applet index

```c
uint32_t SYS_A22C(uint32_t applet_index);
```

The handler consumes one full 32-bit stack argument. Its operation is exactly:

```text
if (selection_block_mask[applet_index] != 0)
    return 0;

selected_applet_index = applet_index;
return selected_applet_index;
```

Important details:

- rejection leaves the previous selection unchanged;
- there is no negative status result;
- success returns the complete 32-bit runtime index that was stored;
- runtime index `0` is therefore intrinsically ambiguous in the return value:
  an accepted index 0 and a rejected request both return 0;
- the handler performs **no explicit range check** before indexing the mask
  table. The surrounding applet machinery establishes the normal valid runtime
  range as `0..31`; callers must not treat A22C itself as a validator for
  arbitrary integers.

A direct System 3.15 internal caller pushes one 32-bit runtime index, calls A22C,
and removes one longword argument, independently confirming the one-long ABI.

A22C does not switch execution to the target applet, send a message, or read the
applet header. It only installs this global selection context after the block-
mask test.

## 3. A230 — clear the selected applet context

```c
int32_t SYS_A230(void);
```

The complete handler is:

```text
D0 = -1
selected_applet_index = D0
return D0
```

Therefore A230:

- consumes no arguments;
- writes sentinel `0xFFFFFFFF` to the selected-index global;
- deterministically leaves `D0 == -1` on return.

The reconstructed prototype exposes that machine result rather than declaring a
`void` function. Whether the original public API documented the result is not
known.

## 4. A234 — return the selected applet's 16-bit ID

```c
uint16_t SYS_A234(void);
```

A234 consumes no arguments.

```text
if (selected_applet_index == -1)
    return 0;                 /* low word */

header = applet_header_table[selected_applet_index]
return header->id;            /* word at +0x14 */
```

Important details:

- the `-1` sentinel maps to ID `0`;
- the handler does not re-check the selection-block mask;
- it does not validate the selected index or pointer before dereferencing;
- it reads exactly the 16-bit ID field at `AppletHeader_t + 0x14`;
- both the successful `MOVE.W` and sentinel `CLR.W` define only the low word of
  D0. The upper 16 bits are not explicitly normalized, so `uint16_t` is the
  portable reconstructed return contract.

This is a runtime-index-to-public-ID bridge, not the same operation as
`AppletFindById`, which performs the inverse search and returns a runtime index.

## 5. Cross-ROM handler map

The three handlers are instruction-for-instruction identical across the compared
ROMs apart from relocated helper/global/table addresses.

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 | Length |
| --- | ---: | ---: | ---: | ---: |
| A22C | `0x4E0DA2` | `0x5E36FE` | `0x439B00` | `0x26` |
| A230 | `0x4E0DC8` | `0x5E3724` | `0x439B26` | `0x0A` |
| A234 | `0x4E0DD2` | `0x5E372E` | `0x439B30` | `0x28` |

The accompanying mask getter is likewise the same 18-byte operation in all
three ROMs: load the long argument, multiply by four, add the table base, and
return the indexed longword.

## 6. Naming boundary

Descriptive names such as `AppletSelect`, `AppletClearSelection`, or
`AppletGetSelectedId` would fit the observed mechanics, but no reliable original
System 3 public symbols have been recovered for these trap slots. The global is
also demonstrably a selection context rather than proof that the selected applet
is currently executing or owns focus.

For that reason the SDK intentionally retains:

```c
uint32_t SYS_A22C(uint32_t applet_index);
int32_t  SYS_A230(void);
uint16_t SYS_A234(void);
```

The neutral phrase **selected applet context** should be used in documentation
until stronger historical naming evidence appears.

## 7. Safety and regression matrix

These operations modify dispatcher/global applet-selection state but do not
modify applet binaries or file contents. Emulator-first regression should still
restore/clear the selection after each case.

Minimum regression matrix:

1. choose a valid enabled runtime index and verify A22C stores/returns it;
2. preselect applet A, attempt blocked applet B, and verify A22C returns 0 while
   preserving A;
3. verify A230 stores `-1` and returns `-1`;
4. verify A234 returns 0 after A230;
5. for a selected valid applet, verify A234 equals the word at header `+0x14`;
6. verify A234 does not itself clear or revalidate the selection;
7. exercise runtime index 0 separately because successful selection and failure
   both yield A22C return value 0;
8. do **not** probe out-of-range A22C indices on irreplaceable hardware: the
   handler does not bounds-check before reading the mask table.

No emulator or hardware regression is claimed as already executed by this note.
