# A1D0-A1DC file password/state block closure

This note closes the mechanically usable ABI of System 3 traps A1D0, A1D4,
A1D8 and A1DC. The implementation was compared across the November 2005
AlphaSmart 3000 ROM, the November 2005 NEO ROM and the July 2013 NEO/System
3.15 ROM, then cross-checked against official SmartApplet callers and the
original 2000 `PasswordModule` source. A1D8 and A1DC have since been reaudited
source-first directly from the canonical firmware; where historical behavior
conflicts with modern firmware, the firmware result below is authoritative.

The four traps remain named `SYS_A1D0`, `SYS_A1D4`, `SYS_A1D8` and
`SYS_A1DC`. Their behavior is sufficiently closed for SDK use, but no reliable
modern public symbol has been recovered.

## SYS_A1D0 — mutate per-file state flags

```c
int32_t SYS_A1D0(uint16_t file_id, uint32_t state_mask, uint8_t enable);
```

The handler resolves `file_id` through the shared File API resolver. The
selected descriptor owns a 32-bit state/flags field at descriptor offset
`+0x20`.

Accepted low-byte gate masks differ by generation:

- AS3000 November 2005: `0x0D` (`0x01 | 0x04 | 0x08`)
- NEO November 2005: `0x0D`
- NEO/System 3.15 July 2013: `0x1D` (adds `0x10`)

If no supported low-byte bit is present, the handler returns raw System 3
status `-14`. On success, `enable == 1` sets requested state bits and every
other value clears them. The success return is the canonical resolved 16-bit
file token, zero-extended; resolver failure returns zero. Individual bit names
remain deliberately unpublished.

## SYS_A1D4 — query per-file state flags

```c
int32_t SYS_A1D4(uint16_t file_id, uint32_t state_mask);
```

A1D4 is the query companion of A1D0. It uses the same resolver and the same
`descriptor+0x20` field. The generation-specific `0x0D`/`0x1D` test is an
admission gate on the low byte; once admitted, the full 32-bit `state_mask`
participates in the state intersection. It returns `1` when any requested bit
is set, `0` when none is set or resolution fails, and raw `-14` when the gate
is not satisfied.

## SYS_A1DC — get/set one file password

```c
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);
```

The resolved descriptor contains the NUL-terminated file-password buffer at
`+0x18`.

When `read_back == 0`, A1DC sets the password. Source-first revalidation of the
actual handler and its private `strlen` helper establishes the modern boundary:

```text
strlen(password) <= 6  -> accepted and copied
strlen(password) > 6   -> raw status -8, no copy
```

The handler compares the `strlen` result with immediate `6` and branches to the
copy path with 68k `BLS` (unsigned lower-or-same). This **corrects the earlier
project statement** that inherited the 2000 `PasswordModule` five-character
limit and claimed `length >= 6 -> -8`. That historical rule is not the modern
A1DC ABI.

For an accepted write, the private `strcpy` helper copies
`password -> descriptor+0x18`. When `read_back != 0`, the same helper copies in
the reverse direction: `descriptor+0x18 -> password`.

Successful get or set returns the canonical resolved 16-bit file token,
zero-extended. Resolver failure returns zero. No safe NULL-pointer guard is
visible before `strlen`/`strcpy`, so invalid pointers are outside the contract.

The modern trap remains deliberately bidirectional and is therefore not renamed
to the historical setter-only `PasswordSetFilePassword` interface.

## SYS_A1D8 — reset all file passwords to the factory default

```c
uint32_t SYS_A1D8(void);
```

A1D8 consumes no caller arguments. Direct source-first revalidation confirms
the interactive destructive flow: it obtains current file-group context,
builds a password UI, passes the typed master password to A24C, returns zero on
cancel/failure, and only after successful comparison iterates descriptors with
stride `0x48`. Each iteration calls a mechanically confirmed private `strcpy`
with destination `descriptor+0x18` and source resolving directly to the firmware
literal `"write"`. The successful return is the number of passwords reset.

The original `PasswordModule` independently corroborates the factory value
`"write"`, but the early source does not define an ABI-identical A1D8 routine.

## Historical naming boundary

The original `PasswordModule.h` exposes names such as
`PasswordSetFilePassword`, `PasswordVerifyFileDialog`,
`PasswordVerifyMasterPassword`, `PasswordChangeFileDialog`, and
`PasswordGetMasterPassword`. None is ABI-identical to the modern quartet.
Historical source is used for lineage and data-model correlation, not for
forcing modern vendor symbols or limits.

A prior Ghidra analysis described A1DC as a setter. That is useful comparative
evidence but incomplete because the firmware has an explicit nonzero
`read_back` path with reversed copy direction.

## Cross-ROM stability

The four handlers occupy the same logical slots and preserve the same core
semantics across AS3000 2005, NEO 2005 and NEO 2013. A1D0/A1D4 add support for
the `0x10` gate bit in 2013; A1D8/A1DC show no observed contractual divergence.

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 |
| --- | ---: | ---: | ---: |
| A1D0 | `0x4E1C50` | `0x5E45AC` | `0x43A9AE` |
| A1D4 | `0x4E1CB8` | `0x5E4614` | `0x43AA16` |
| A1D8 | `0x4E29D2` | `0x5E532E` | `0x43B730` |
| A1DC | `0x4E2B26` | `0x5E5482` | `0x43B884` |

## Regression policy and status

A1D8 source-first static regression is **EXECUTED: 57/57 PASS**. Its dynamic
regression remains emulator-first on disposable state because it intentionally
changes every file password in the selected namespace.

A1DC source-first static regression is **EXECUTED: 96/96 PASS**. Dynamic tests
must cover write lengths 0 through 6 as accepted, length 7 as `-8` without
mutation, get-after-set, resolver failure and read/write round trips. NULL
pointers are unsupported rather than testable error cases.

A1D0/A1D4 dynamic state tests likewise belong on disposable/restorable state.

## Closure decision

A1D0-A1DC are no longer mechanically unknown traps. All four have publishable
SDK prototypes and documented state effects. Generic trap names are retained
because semantic behavior is known more confidently than original System 3
symbol nomenclature. The modern A1DC six-character acceptance boundary is now
explicitly separated from the older five-character `PasswordModule` rule.
