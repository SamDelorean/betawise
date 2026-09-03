# SYS_A254 — File API token-group selector/name query — source-first closure

Status: **CLOSED A / SOURCE-FIRST / PUBLISHED**

```c
uint8_t SYS_A254(uint8_t token_group, char *group_name_out);
```

The original modern vendor symbol was not recovered, so the neutral `SYS_A254` name is retained.

## Correlation first

The source-first pass correlated the existing File API reconstruction, the previously closed A1FC token builder/validator, the A38C `strcpy` closure, retrospective private notes, prior SDK declarations, and the three canonical firmware generations. No official user-manual feature name was found for this internal token-group mechanism; the manuals therefore do not supply a stronger public name and are not used to invent one.

The previously published header already has the correct ABI and does not require modification.

## Canonical handler map

- AS3000 2005: runtime `0x004DFEEA`, file `0x0001FEEA`
- NEO 2005: runtime `0x005E2846`, file `0x00022846`
- NEO 2013: runtime `0x00438C44`, file `0x00028C44`

All three handlers are exactly `0x70` bytes and have instruction-for-instruction equivalent control flow apart from relocated globals and the relocated `strcpy` target.

Handler SHA-256:

- AS3000 2005: `d8b6fa72ea27c9ee9ffb2e30703be020fef4c4c7ffe3c5165691ce0c463973c5`
- NEO 2005: `d2a20b34e10092412bf8b6f4c1e9e325fdd84a1b3c27281261ba86d418ba4e28`
- NEO 2013: `cb1257a1d8c6c46f01bd02bf81e68bec7eb90dfe00e213896de4f41f85d4d3eb`

## Confirmed mechanics

The handler consumes the low byte of the first ABI slot and a pointer from the second slot.

Each ROM maintains a token-group runtime cluster consisting of four 30-byte name records, a state/gate byte, and a current-group/flags byte. The corresponding addresses are:

| ROM | name table | state/gate | current group/flags |
| --- | ---: | ---: | ---: |
| AS3000 2005 | `0x00003D7A` | `0x00003DF2` | `0x00003DF3` |
| NEO 2005 | `0x00003DD2` | `0x00003E4A` | `0x00003E4B` |
| NEO 2013 | `0x00003E0A` | `0x00003E82` | `0x00003E83` |

Behavior is mechanically equivalent to:

```c
if (group_state == 0)
    return 0;

uint8_t effective = token_group;
if (effective > 4)
    return 0;

if (effective == 0)
    effective = current_group_flags & 0x7f;

char *record = name_table + 30 * (effective - 1);
if (record[0] == '\0')
    return 0;

if (group_name_out != NULL)
    strcpy(group_name_out, record);

return effective;
```

The pseudocode intentionally mirrors the real ordering. In particular, the explicit `>4` test occurs **before** selector zero is replaced by `current_group_flags & 0x7F`. The handler does not perform a second range check after that substitution. Normal System 3 state therefore relies on the current-group invariant established by the surrounding File API runtime. The SDK must not falsely claim that A254 itself validates a substituted current group a second time.

The first byte of the selected 30-byte record doubles as the populated/empty test. If an output buffer is supplied, the helper target is exactly the already closed A38C `strcpy` entry in each generation:

- AS3000: `0x004DDC0E`
- NEO 2005: `0x005E032C`
- NEO 2013: `0x0043672A`

Thus the output operation is a NUL-terminated string copy, not a fixed 30-byte memcpy. A 30-byte writable destination remains the conservative SDK requirement because each table record occupies 30 bytes.

A254 does not modify the token-group globals or table. Its only possible write is through a non-NULL `group_name_out`.

## File-token correlation

The A1FC source-first closure independently found ten direct A1FC callers per canonical ROM. All thirty are immediately preceded by A254: A254's low-byte result is zero-extended and supplied as A1FC's `token_high`, while a separate byte supplies `token_low`. A1FC then constructs and validates:

```text
token = (token_high << 8) | token_low
```

This directly confirms that A254 resolves the File API token-group byte rather than a password state or generic application index.

Complete-ROM direct absolute JSR search for A254 itself finds **12 / 12 / 12** sites in AS3000 2005 / NEO 2005 / NEO 2013. Ten per ROM feed A1FC as described above; the remaining direct uses are consistent with querying the same group runtime state without constructing a token.

## Refutation

The primary evidence rejects these alternatives:

- password-related getter/setter role — A254 addresses the independent File API group table and state cluster;
- simple current-group getter — explicit selectors 1..4 are accepted and optional group-name output is supported;
- five-group table — explicit selector values greater than 4 are rejected and address arithmetic uses four 30-byte records;
- fixed-length copy — the helper is confirmed `strcpy`;
- output buffer mandatory — NULL has an explicit skip path;
- selector zero literally means group zero — it requests `current_group_flags & 0x7F`;
- substituted current group is range-checked by A254 — it is not; the second check does not exist.

## Classification

**CONFIRMED:** ABI; handler bounds; four 30-byte records; state gate; explicit range 1..4; selector 0 substitution with `&0x7F`; populated-record test; NULL-safe optional output; `strcpy`; returned effective group byte; 12/12/12 direct JSRs; A254→A1FC token-high correlation; cross-generation equivalence.

**INFERENCIA FUERTE:** the surrounding File API runtime maintains the substituted current-group value in the valid 1..4 domain whenever the group state is active.

**DESCONOCIDO:** original modern vendor symbol and any vendor-facing human name for this internal group mechanism.

## Regression

Static primary regression was **EXECUTED: 48/48 PASS** across the canonical ROMs. Sixteen checks per ROM cover handler length, prologue, both ABI slots, state gate, explicit `>4` rejection, selector-zero branch, `0x7F` mask, 30-byte stride arithmetic, table base, populated-record test, NULL output branch, exact A38C/`strcpy` helper target, low-byte return, RTS, and complete-ROM direct caller count.

Dynamic regression is **SPECIFIED / NOT EXECUTED**: active/inactive group state; explicit 1..4 populated and empty slots; selector 0; selector >4; NULL and writable output buffers; and A254→A1FC token construction. Emulator-first execution remains preferred.

Firmware bytes and complete disassembly remain private.
