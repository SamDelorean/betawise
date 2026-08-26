# Dialog API — A0F0 through A110

This document tracks the System 3 / OS3K dialog syscall family as a coherent subsystem rather than as isolated A-line traps.

## Current mapping

| Trap | BetaWise name | Current confidence | Historical relationship |
| --- | --- | --- | --- |
| `A0F0` | `DialogInit` | A | `DialogMenuInit` |
| `A0F4` | `DialogAddItem` | A for call shape, `id`, capacity and core inherited fields; partial for later metadata | `DialogMenuAddItem` |
| `A0F8` | `DialogAddExitKey` | A | `DialogMenuAddExitICode` |
| `A0FC` | `DialogSetChoice` | A | `DialogMenuSetCursorItemNumber` |
| `A100` | `DialogDraw` | B/A- | `DialogMenuDisplay` |
| `A104` | `DialogRun` | B/A- | `DialogMenuGetInput` |
| `A108` | `DialogGetChoice` | A | `DialogMenuGetCursorItemNumber` |
| `A10C` | `DialogGetChoiceId` | A | OS3K metadata getter for current choice |
| `A110` | `DialogGetItemId` | A | OS3K metadata getter by item index |

Confidence refers to the known contract, not merely the presence of a name in `syscall.c`.

## Historical AS3000 dialog model

The original AS3000 dialog implementation predates the later OS3K SmartApplet ABI. Historical compiled code and source usage show an insertion-ordered menu model with functions corresponding to initialization, adding items, adding exit keys, selecting the cursor item, drawing, waiting for input, and retrieving the selected item.

The historical `DialogMenuAddItem` accepts three arguments:

```c
int DialogMenuAddItem(char *text, uint8_t text_len, char marker);
```

Observed historical behavior:

- items are stored in insertion order;
- the historical implementation accepts at most 25 items;
- `text_len` is clipped to the available dialog width;
- `marker` is stored per item and drawn immediately before the item text;
- success returns `0`; a full menu returns `-1`;
- the selected cursor item is 1-based in original AlphaWord usage.

The old global dialog structure contains item text pointers, lengths, markers, cursor/visible-window state and exit keys. It does not contain the later OS3K `id`, `shortcut_key`, or `file_size` metadata.

## OS3K extension of DialogAddItem

Official OS3K SmartApplet machine code confirms that `A0F4` is called with six 32-bit stack slots under the compiler ABI:

```c
int DialogAddItem(char* text,
                  uint8_t text_len,
                  char marker,
                  int id,
                  Key_e shortcut_key,
                  size_t file_size);
```

Direct handler analysis in both the November 2005 AS3000 and NEO System 3 ROMs confirms that the fourth argument (`id`) is stored as a 32-bit value in a per-item ID array indexed by insertion order. The same array is read by `A10C` and `A110`.

### Parameters

`text` and `text_len`
: Strongly established and inherited directly from the historical three-argument interface.

`marker`
: Historically a literal prefix glyph drawn immediately before the item text. A blank space is normal. Later OS3K applets also use other byte values; any additional OS3K-specific semantics remain experimental.

`id`
: **Confirmed.** Caller-provided 32-bit metadata independent of the insertion-order choice index. `A0F4` stores this value in the ID array; `A10C` and `A110` retrieve it.

`shortcut_key`
: A real fifth OS3K argument. Existing applets pass key codes. Its role as a per-item shortcut is strongly supported, but exact rendering and all matching rules remain to be validated.

`file_size`
: A real sixth OS3K argument. Official callers frequently pass `(size_t)-1`. No stable SDK-level semantic description is yet justified.

### Capacity and return value

The later 2005 OS3K implementation does **not** preserve the historical 25-item capacity. Both AS3000 and NEO handlers compare the current item count against `0x40` before insertion:

- maximum later-OS3K capacity observed: **64 items**;
- when already at capacity, `DialogAddItem` returns `-1`;
- on successful insertion, it returns `0`.

This is direct firmware evidence for the analyzed 2005 System 3 images. It should not be projected onto unrelated firmware generations without verification.

## Control and execution functions

### A0F0 — DialogInit

```c
void DialogInit(bool single, uint8_t row_first, uint8_t row_last, uint8_t col);
```

**Confidence: A for argument placement/state initialization in the analyzed AS3000 and NEO ROMs.**

The handler clears item and exit-key counts, initializes the current choice to 1, stores the three geometry bytes, and normalizes the `single` argument to a boolean state. Exact visual behavior of unusual geometry remains an execution-level question, not an ABI question.

### A0F8 — DialogAddExitKey

```c
int DialogAddExitKey(Key_e key);
```

**Confidence: A.** The analyzed AS3000 and NEO handlers maintain an explicit byte array of exit keys:

- capacity: **15 exit keys**;
- success: returns `0` and appends the key;
- full array: returns `-1`.

### A0FC — DialogSetChoice

```c
void DialogSetChoice(uint8_t index);
```

**Confidence: A.** The handler writes the low byte of `index` directly into the dialog's current-choice state. No range validation is performed by this syscall itself. Normal usage is 1-based.

### A100 — DialogDraw

```c
void DialogDraw(void);
```

Renders the current dialog state. It corresponds conceptually to historical `DialogMenuDisplay`. Rendering details and AS3000/NEO differences remain best validated through emulator/hardware execution.

### A104 — DialogRun

```c
short DialogRun(void);
```

Processes dialog input until an exit condition is reached. Firmware returns a 16-bit value from the routine, consistent with the existing `short` declaration and caller treatment as a key-compatible value. The complete navigation/shortcut contract remains under execution validation.

### A108 — DialogGetChoice

```c
char DialogGetChoice(void);
```

**Confidence: A.** In both analyzed ROMs the handler reads the current-choice byte directly and returns it. The normal model is 1-based.

### A10C — DialogGetChoiceId

```c
int DialogGetChoiceId(void);
```

**Confidence: A.** Direct firmware handler analysis resolves the previously uncertain contract.

For a valid current dialog state, the implementation is equivalent to:

```c
return item_ids[current_choice - 1];
```

The AS3000 and NEO implementations are structurally identical; only their RAM addresses differ. The routine reads the same current-choice byte returned by `A108`, converts the 1-based choice to a zero-based array offset, multiplies by four, and returns the corresponding 32-bit ID stored by `A0F4`.

Importantly, `A10C` performs **no explicit bounds check**. It relies on the current dialog choice being valid.

Therefore, for a valid choice:

```c
DialogGetChoiceId() == DialogGetItemId(DialogGetChoice())
```

### A110 — DialogGetItemId

```c
int DialogGetItemId(uint8_t index);
```

**Confidence: A.** Direct firmware analysis shows that `A110` reads the same per-item ID array used by `A10C`.

Unlike `A10C`, `A110` validates its 1-based argument:

```c
if(index < 1 || index > item_count)
    return 0;
return item_ids[index - 1];
```

This also confirms that the `id` argument to `DialogAddItem` is semantically distinct from the insertion-order choice index.

## 2005 firmware state relationship

The analyzed November 2005 AS3000 and NEO ROMs use the same logical structure with different RAM locations:

```text
item_count
current_choice
exit_key_count
exit_key[]
item_id[]
```

The important ABI relation is logical rather than address-based; these RAM addresses are firmware-internal and must not be exposed as portable SDK constants.

## Working lifecycle

The now-confirmed metadata lifecycle is:

```c
DialogInit(...);
DialogAddExitKey(KEY_ENTER);
DialogAddExitKey(KEY_ESC);

DialogAddItem(..., 100, ...);
DialogAddItem(..., 200, ...);

DialogSetChoice(1);
DialogDraw();
KeyMod_e exit_key = DialogRun();
char choice = DialogGetChoice();
int current_id = DialogGetChoiceId();
int indexed_id = DialogGetItemId(choice);
```

For a valid current choice, `current_id` and `indexed_id` should be identical by construction of the firmware handlers.

## Emulator feedback targets

When the emulator can execute SmartApplets, dialog tracing should record at minimum:

- A-line opcode and caller PC;
- stack pointer on entry;
- decoded arguments for `A0F0`–`A110` where known;
- item/exit-key counts where useful;
- selected index before/after `DialogRun`;
- `DialogRun` return value;
- `A108`, `A10C`, and `A110` results;
- differences between AS3000 and NEO rendering/navigation behavior.

For `A10C`, the emulator regression expectation is now explicit: with a valid choice it must match `A110(A108())`.

## Remaining questions

The main unresolved dialog questions are now:

1. exact shortcut-key rendering and matching rules;
2. exact `file_size` semantics and cases where it differs from `-1`;
3. AS3000/NEO behavioral differences in geometry, markers and navigation;
4. exact edge-case behavior of `DialogDraw` and `DialogRun`;
5. whether capacities or metadata layout differ in System 3 firmware generations other than the analyzed 2005 images.

`A10C` is no longer a blocker: its contract is directly established by both AS3000 and NEO firmware handlers.
