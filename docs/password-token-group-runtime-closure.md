# System 3 / OS3K runtime services closure — A248–A258

This note closes the mechanical ABI of the five traps between the named
SmartApplet runtime API (`A238`–`A244`) and the still-partial special-key
dispatcher `A25C`:

- `A248` — global file-password-protection state getter;
- `A24C` — non-interactive master-password comparison;
- `A250` — interactive master-password verification UI;
- `A254` — file-token-group selector/name query;
- `A258` — global file-password-protection state setter.

The compared implementations are the November 2005 AlphaSmart 3000 ROM, the
November 2005 NEO ROM, and the July 2013 NEO/System 3.15 ROM. Mechanical
confidence is A. Original modern public symbols were not recovered, so the SDK
retains the `SYS_Axxx` names rather than inventing replacements.

The block is not one coherent subsystem: A248/A24C/A250/A258 belong to password
runtime state, while A254 belongs to the modern File API token-group machinery.
They are documented together here because they occupy one contiguous A-line
range.

## A248 — file-password-protection state getter

Reconstructed form:

```c
uint8_t SYS_A248(void);
```

The handler is only:

```text
MOVE.B  protection_state,D0
RTS
```

Only the low byte of D0 is contractual; the handler does not clear the upper 24
bits.

The state byte is the byte immediately preceding the current master-password
buffer in all three compared ROMs:

| ROM | protection byte | master-password buffer |
| --- | ---: | ---: |
| AS3000 2005 | `0x00004472` | `0x00004473` |
| NEO 2005 | `0x000058A4` | `0x000058A5` |
| NEO 2013 | `0x000118A2` | `0x000118A3` |

Cold/default initialization clears the protection byte and initializes the
master-password buffer from the firmware default `think`. This directly
establishes the relationship between the two globals.

The decisive semantic caller is `PasswordVerifyFileDialog` / A1E0. A1E0 calls
A248 and skips the file-password challenge path when the returned byte is zero;
a nonzero value enables the password-protected path. Another System 3 caller
compares the value to 1 while selecting the localized ON/OFF text used by the
status/control UI. Firmware strings in every compared generation include
`File password protection is OFF`, `File password protection is ON`, and the
master-password prompts for turning the setting on or off.

Thus A248 is mechanically the getter for the global file-password-protection
state. The original symbol name is still unknown.

## A258 — file-password-protection state setter

Reconstructed form:

```c
void SYS_A258(uint8_t protection_state);
```

The complete handler is equivalent to:

```text
protection_state_global = low_byte(first_argument)
RTS
```

It performs no validation, normalization, password check, or Boolean clamp. The
normal System 3 model uses 0/1, but the trap mechanically stores any low byte.
The master-password authorization step belongs to higher-level flows such as
A250, not to A258 itself.

A258 does not write D0 at all, so no return value is contractual. It is exposed
as `void` rather than pretending that stale D0 contents are meaningful.

A248 and A258 address exactly the same global byte in each ROM, making them a
mechanical getter/setter pair.

## A24C — non-interactive master-password comparison

Reconstructed form:

```c
uint8_t SYS_A24C(const char *password);
```

A24C receives one caller pointer and compares it against the current master
password using the same `StringCompare` service exposed as A08C. The call is
mechanically equivalent to:

```c
StringCompare(master_password, password, 0, 0)
```

A zero comparison result becomes low-byte return 1; a nonzero comparison result
becomes low-byte return 0. Because failure uses a byte clear, only the low byte
of D0 is contractual.

The pointer must reference a valid NUL-terminated string; there is no NULL-safe
path in the handler.

A1D8, the already documented destructive reset of file passwords to factory
`write`, passes the password entered by the user to A24C before carrying out the
reset. That internal caller independently confirms that A24C verifies the master
password rather than a per-file password.

The original 1998–2000 password subsystem contained a concept named
`PasswordVerifyMasterPassword`, but the modern symbol/ABI lineage is not strong
enough to rename A24C automatically. The descriptive historical relationship is
recorded without promoting the name.

## A250 — interactive master-password verification

Reconstructed form:

```c
uint8_t SYS_A250(uint32_t reserved, const char *prompt);
```

The handler consumes two 32-bit caller slots. The first slot is not read anywhere
in the compared implementations. All six direct System 3 callers in each ROM use
value 2 in that slot, so SDK code that must reproduce native calling patterns
should use 2 while treating the parameter as reserved/legacy.

The second argument is an optional string pointer. When non-NULL the handler
prints that text as part of the prompt sequence.

The verification loop is mechanically closed:

1. construct an exit-key list containing Enter, Escape and `KEY_NONE`;
2. call `TextBox` / A084 in password mode with maximum length 6;
3. Escape or any non-Enter termination returns low-byte 0;
4. Enter compares the captured text with the current master-password buffer via
   `StringCompare(..., 0, 0)`;
5. a match returns low-byte 1;
6. a mismatch displays the localized `Master password was incorrect.` message,
   waits 200 centiseconds through A0D4, clears/restores the input area and loops
   for another attempt.

Only the low return byte is contractual. The first argument remains intentionally
unnamed beyond `reserved`: the current firmware does not consume it, and its
historical purpose has not been recovered.

A250 is used internally for several protected System 3 settings, not only the
file-password-protection toggle. It is therefore a generic interactive master-
password gate rather than a file-password dialog.

## A254 — file token-group selector and optional name copy

Reconstructed form:

```c
uint8_t SYS_A254(uint8_t token_group, char *group_name_out);
```

A254 is functionally separate from the password traps around it. It operates on
the File API token-group runtime state.

Each compared ROM maintains four 30-byte group-name slots (`0x78` bytes total),
plus a group-state byte and a current-group/flags byte:

| ROM | name table | state byte | current group/flags |
| --- | ---: | ---: | ---: |
| AS3000 2005 | `0x00003D7A` | `0x00003DF2` | `0x00003DF3` |
| NEO 2005 | `0x00003DD2` | `0x00003E4A` | `0x00003E4B` |
| NEO 2013 | `0x00003E0A` | `0x00003E82` | `0x00003E83` |

Mechanical behavior:

- if the group-state byte is zero, return 0;
- explicit selectors greater than 4 return 0;
- selector 0 means use `current_group_flags & 0x7F`;
- the selected slot address is `name_table + 30 * (selector - 1)`;
- if the first byte of that slot is zero, return 0;
- if `group_name_out != NULL`, copy the NUL-terminated slot string with `strcpy`;
- return the effective selector in the low byte of D0.

A writable output buffer of at least 30 bytes is the conservative SDK contract
for the optional name copy.

The token-group interpretation is established directly by System 3 callers. A
common sequence invokes `SYS_A254(0, NULL)`, zero-extends its returned byte, and
passes that value as the second/high-byte argument to A1FC, whose closed contract
constructs and validates `(token_high << 8) | token_low`. A254 therefore resolves
the current or explicit token group used as the high byte of modern 16-bit File
API tokens; the 30-byte slot is the associated group name.

No source-level original name has been recovered, so `SYS_A254` is retained.

## Cross-ROM handler map

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 | Length |
| --- | ---: | ---: | ---: | ---: |
| A248 | `0x4DFF5A` | `0x5E28B6` | `0x438CB4` | `0x08` |
| A24C | `0x4E04F6` | `0x5E2E52` | `0x439250` | `0x26` |
| A250 | `0x4E051C` | `0x5E2E78` | `0x439276` | `0x170` |
| A254 | `0x4DFEEA` | `0x5E2846` | `0x438C44` | `0x70` |
| A258 | `0x4DFF62` | `0x5E28BE` | `0x438CBC` | `0x0A` |

The small handlers are instruction-for-instruction equivalent apart from
relocated globals/helpers. A250 also retains the same 0x170-byte control flow in
all three ROMs; its byte differences are relocation/localization addresses, not
an ABI change. A254 likewise preserves the same four-slot, 30-byte stride and
selector logic.

## Safety and regression

A248, A24C and read-only A254 calls are suitable for emulator/hardware regression
with valid pointers. A250 is interactive and should be tested first in the
firmware-first emulator because it changes UI/input state and loops after a bad
password. A258 mutates the global enforcement state and should be restored to its
previous value after any test.

Minimum regression matrix:

1. A248 default/known state agrees with the protection global;
2. save A248, set 0/1 through A258, verify A248, then restore the saved state;
3. A24C with the configured master password -> low byte 1; wrong password -> 0;
4. A250 correct password -> 1, Escape -> 0, wrong password -> error/delay/retry;
5. A250 with NULL versus non-NULL prompt preserves verification semantics;
6. A254 selector 0 returns the current token group when the table is active;
7. A254 valid explicit 1..4 with a populated slot returns that selector and
   copies the same group name;
8. A254 selector >4 and empty/unavailable group return 0;
9. correlate A254 output with the high byte supplied to A1FC.

No emulator or hardware regression is claimed as already executed by this note.
