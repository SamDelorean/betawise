# Dialog API — A0F0 through A110

This document tracks the System 3 / OS3K dialog syscall family as a coherent subsystem rather than as isolated A-line traps.

## Current mapping

| Trap | BetaWise name | Current confidence | Historical relationship |
| --- | --- | --- | --- |
| `A0F0` | `DialogInit` | A/B | `DialogMenuInit` |
| `A0F4` | `DialogAddItem` | A for six-argument OS3K call shape; partial semantics | `DialogMenuAddItem` |
| `A0F8` | `DialogAddExitKey` | B | `DialogMenuAddExitICode` |
| `A0FC` | `DialogSetChoice` | B/A- | `DialogMenuSetCursorItemNumber` |
| `A100` | `DialogDraw` | B/A- | `DialogMenuDisplay` |
| `A104` | `DialogRun` | B/A- | `DialogMenuGetInput` |
| `A108` | `DialogGetChoice` | B/A- | `DialogMenuGetCursorItemNumber` |
| `A10C` | `DialogGetChoiceId` | C | no historical counterpart established yet |
| `A110` | `DialogGetItemId` | B | OS3K metadata getter; official use observed |

The names are retained because the family is internally coherent and is used by existing BetaWise applets. Confidence refers to the known contract, not merely the presence of a name in `syscall.c`.

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

This resolves earlier decompiler output that showed only two visible arguments: that decompiler signature was incomplete and must not be treated as the actual firmware ABI.

### Parameters

`text` and `text_len`
: Strongly established. They descend directly from the historical three-argument interface.

`marker`
: Historically a literal prefix glyph drawn immediately before the item text. A blank space is normal. Later OS3K applets also use glyph values such as `0xD7` and `0x10`; any additional later semantics remain experimental.

`id`
: Caller-provided metadata independent of the insertion-order choice index. Existing DebugTool code uses `DialogGetItemId(choice)` to inspect this field.

`shortcut_key`
: A real fifth OS3K argument. Existing applets pass keys such as `KEY_A`, `KEY_B`, `KEY_G`, etc. Its role as a per-item shortcut is strongly supported, but exact rendering and all key-handling rules remain to be validated.

`file_size`
: A real sixth OS3K argument. Official callers frequently pass `(size_t)-1`. No stable SDK-level semantic description is yet justified.

### Return value

The historical predecessor returns `0` on success and `-1` when its 25-item capacity is exhausted. BetaWise currently declares the OS3K function as returning `int`. Whether later OS3K preserves the exact historical failure conditions is still to be tested.

## Control and execution functions

### A0F0 — DialogInit

```c
void DialogInit(bool single, uint8_t row_first, uint8_t row_last, uint8_t col);
```

Existing official/BetaWise usage establishes the four-argument call shape. The historical predecessor initializes menu geometry and single/multi-item behavior. Exact meaning of every later OS3K geometry edge case has not yet been exhaustively tested.

### A0F8 — DialogAddExitKey

```c
int DialogAddExitKey(Key_e key);
```

Adds a key that terminates `DialogRun`. Existing code commonly adds `KEY_ENTER` and `KEY_ESC`. The historical predecessor stores explicit exit input codes. Exact return/error behavior remains to be characterized.

### A0FC — DialogSetChoice

```c
void DialogSetChoice(uint8_t index);
```

Sets the current insertion-order choice before drawing/running the dialog. Existing usage is 1-based, matching the historical cursor-item model.

### A100 — DialogDraw

```c
void DialogDraw(void);
```

Renders the current dialog state. It corresponds conceptually to historical `DialogMenuDisplay`.

### A104 — DialogRun

```c
short DialogRun(void);
```

Processes dialog input until an exit condition is reached. Existing callers treat its return as a key value (`KeyMod_e`-compatible) and mask modifier bits such as Caps Lock. Exact full return contract is still classified as strong rather than fully confirmed.

### A108 — DialogGetChoice

```c
char DialogGetChoice(void);
```

Returns the current/selected insertion-order choice. Existing code compares it against item counts and then passes it to `DialogGetItemId`. The observed model is 1-based.

### A10C — DialogGetChoiceId

```c
int DialogGetChoiceId(void);
```

This is the least-characterized member of the family. Its name and placement suggest returning the caller-defined `id` associated with the current choice, but this must not be promoted to a confirmed contract until execution or a decisive firmware caller/handler analysis establishes it.

### A110 — DialogGetItemId

```c
int DialogGetItemId(uint8_t index);
```

Returns caller-defined item metadata for an insertion-order index. Existing DebugTool usage provides strong evidence for this interpretation. Official OS3K code also uses `A110`, making it better supported than `A10C`.

## Working lifecycle

The best-supported normal usage pattern is:

```c
DialogInit(...);
DialogAddExitKey(KEY_ENTER);
DialogAddExitKey(KEY_ESC);

DialogAddItem(...);
DialogAddItem(...);

DialogSetChoice(1);
DialogDraw();
KeyMod_e exit_key = DialogRun();
char choice = DialogGetChoice();
int item_id = DialogGetItemId(choice);
```

`DialogGetChoiceId()` is intentionally not required for the minimal lifecycle because its contract is less certain.

## Emulator feedback targets

When the emulator can execute SmartApplets, dialog tracing should record at minimum:

- A-line opcode and caller PC;
- stack pointer on entry;
- decoded arguments for `A0F0`–`A110` where known;
- `DialogAddItem` return value;
- selected index before/after `DialogRun`;
- `DialogRun` return value;
- `A108`, `A10C`, and `A110` results;
- differences between AS3000 and NEO firmware behavior.

These traces should feed back into this document and `os3k.h` rather than being treated as emulator-only information.

## Remaining questions

The current blocking questions for SDK promotion are:

1. exact later-OS3K return/error behavior of `DialogAddItem` and `DialogAddExitKey`;
2. exact shortcut-key rendering and matching rules;
3. exact `file_size` semantics and cases where it differs from `-1`;
4. decisive contract for `DialogGetChoiceId` (`A10C`);
5. any AS3000/NEO behavioral differences in geometry, markers or navigation;
6. whether the historical 25-item limit remains unchanged in later OS3K.

Until these are resolved, the existing function names/signatures remain usable research interfaces, but unresolved details must stay explicitly marked as such.
