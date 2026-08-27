# A1D0-A1DC file password/state block closure

This note closes the mechanically usable ABI of System 3 traps A1D0, A1D4,
A1D8 and A1DC.  The implementation was compared across the November 2005
AlphaSmart 3000 ROM, the November 2005 NEO ROM and the July 2013 NEO/System
3.15 ROM, then cross-checked against official SmartApplet callers and the
original 2000 `PasswordModule` source.

The four traps remain named `SYS_A1D0`, `SYS_A1D4`, `SYS_A1D8` and
`SYS_A1DC`.  Their behavior is now sufficiently closed for SDK use, but no
reliable modern public symbol has been recovered.  Giving them new semantic
names would confuse reconstructed behavior with historical nomenclature.

## SYS_A1D0 — mutate per-file state flags

Mechanical ABI:

```c
int32_t SYS_A1D0(uint16_t file_id, uint32_t state_mask, uint8_t enable);
```

The handler resolves `file_id` through the shared File API resolver.  The
selected descriptor owns a 32-bit state/flags field at descriptor offset
`+0x20`.

Accepted low-byte masks differ by firmware generation:

- AS3000 November 2005: `0x0D` (`0x01 | 0x04 | 0x08`)
- NEO November 2005: `0x0D`
- NEO/System 3.15 July 2013: `0x1D` (adds `0x10`)

The validation is against the low byte of `state_mask`.  If no supported bit
is present, the handler returns raw System 3 status `-14`.  No original symbolic
name for this status has been recovered.

On success:

```text
enable == 1  -> descriptor->state_flags |= state_mask
otherwise    -> descriptor->state_flags &= ~state_mask
```

The success return is the canonical resolved 16-bit file token, zero-extended
into D0.  Resolver failure returns zero.  Official AlphaWord, AlphaQuiz and
KeyWords callers exercise masks including `0x01`, `0x04` and, on later
firmware, `0x10`.

The individual meanings of these bits are not sufficiently closed to publish
`FILE_FLAG_*` constants.  The SDK therefore exposes the mechanical primitive
without inventing flag names.

## SYS_A1D4 — query per-file state flags

Mechanical ABI:

```c
int32_t SYS_A1D4(uint16_t file_id, uint32_t state_mask);
```

A1D4 is the query companion of A1D0.  It uses the same resolver, the same
`descriptor+0x20` state field and the same generation-specific allowed low-byte
masks (`0x0D` in the 2005 images, `0x1D` in 2013).

- unsupported mask -> raw `-14`
- resolver failure -> `0`
- successful resolution -> `1` when any requested state bit is set, otherwise
  `0`

Official AlphaWord callers query at least masks `0x01` and `0x10`.

## SYS_A1DC — get/set one file password

Mechanical ABI:

```c
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);
```

The resolved descriptor contains a NUL-terminated file-password buffer at
`+0x18`.

When `read_back == 0`, A1DC sets the password:

1. `strlen(password)` is evaluated;
2. length `>= 6` returns raw status `-8` without copying;
3. otherwise `strcpy(descriptor_password, password)` is performed.

Therefore a valid modern file password is at most **five characters plus the
terminating NUL**.

When `read_back != 0`, A1DC performs the reverse copy:

```text
strcpy(password, descriptor_password)
```

The caller must therefore supply a writable buffer large enough for the stored
password (six bytes is sufficient for the normal five-character limit).

Successful get or set returns the canonical resolved 16-bit file token,
zero-extended.  Resolver failure returns zero.  No NULL-pointer guard for the
password argument is visible in the handler, so passing an invalid pointer is
not a supported edge case.

Official AlphaWord code contains both modes, establishing that the modern trap
is deliberately bidirectional.  This is why it is **not** renamed to the
historical `PasswordSetFilePassword`: the 2000 routine was setter-only and had
a different `(file_number, length, pointer)` interface.

## SYS_A1D8 — reset all file passwords to the factory default

Mechanical ABI:

```c
uint32_t SYS_A1D8(void);
```

A1D8 consumes no caller arguments.  An official Control Panel caller invokes
it with an empty argument stack.

The routine is interactive and destructive:

1. it obtains current file-group context;
2. clears/builds a password UI;
3. uses `TextBox` in password mode with Enter/Escape exit keys and a six-byte
   local input area;
4. Escape/cancel returns zero;
5. Enter passes the typed string to A24C, the master-password comparison helper;
6. a failed master-password comparison displays an error and returns zero;
7. a successful comparison iterates every file descriptor in the current
   group and copies the literal string `"write"` into descriptor `+0x18`;
8. it displays the success message and returns the number of file passwords
   reset (`file_count`).

The System 3 ROM strings make the UI purpose explicit:

- `Enter the master password to set all`
- `file passwords to the factory default:`
- `File passwords set to factory default.`

The literal factory password is `write`.

This is independently corroborated by the original 2000 `PasswordModule`:
`PASSWORD_MAX_LENGTH` is 5, cold-boot initialization sets each file password to
`w r i t e`, and the historical master password is initialized separately.
The early source does not contain an exact equivalent of the later interactive
A1D8 routine, so that historical evidence confirms the data model and factory
value but does not justify inventing a modern function name.

## Historical naming boundary

The original `PasswordModule.h` exposes names such as:

- `PasswordSetFilePassword`
- `PasswordVerifyFileDialog`
- `PasswordVerifyMasterPassword`
- `PasswordChangeFileDialog`
- `PasswordGetMasterPassword`

None is ABI-identical to the modern A1D0/A1D4/A1D8/A1DC quartet.  In particular,
A1DC combines get and set in one trap, while the old public setter was a
three-argument setter only.  The project therefore preserves `SYS_A1D*` names
until a genuine later System 3 symbol source is recovered.

A prior Ghidra analysis renamed A1DC to a descriptive
`a1dc_set_password_syscall`; that is useful comparative evidence but incomplete,
because official callers prove the same trap also reads passwords.

## Cross-ROM stability

The four handlers occupy the same logical slots and preserve the same control
flow across AS3000 2005, NEO 2005 and NEO 2013.  A1D0/A1D4 add support for the
`0x10` flag bit in the 2013 generation; that is a real ABI-generation difference
and is documented rather than normalized away.

Handler addresses used during reconstruction:

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 |
| --- | ---: | ---: | ---: |
| A1D0 | `0x4E1C50` | `0x5E45AC` | `0x43A9AE` |
| A1D4 | `0x4E1CB8` | `0x5E4614` | `0x43AA16` |
| A1D8 | `0x4E29D2` | `0x5E532E` | `0x43B730` |
| A1DC | `0x4E2B26` | `0x5E5482` | `0x43B884` |

## Regression policy

A1D0, A1D4 and A1DC can be regression-tested on disposable file descriptors in
the firmware-first emulator.  Tests must cover supported/unsupported masks,
set/clear/query behavior, the five-character password boundary, get-after-set,
resolver failure and the raw `-14`/`-8` returns.

A1D8 must be tested **emulator-first on disposable state only**.  It intentionally
changes every file password in the selected namespace.  It should not be used as
a hardware discovery probe.  A hardware regression would only be appropriate
when password state has been explicitly backed up and the user intends the reset.

No regression is claimed as already executed by this document.  Firmware,
official callers and historical source close the ABI contract; emulator
execution remains a verification step.

## Closure decision

A1D0-A1DC are no longer mechanically unknown traps.  All four have publishable
SDK prototypes and documented state effects.  Their generic trap names are
retained because semantic behavior is known more confidently than original
System 3 symbol nomenclature.
