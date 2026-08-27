# System 3 / OS3K current-SmartApplet settings closure — A278

This note closes the mechanical ABI of trap A278 across the November 2005
AlphaSmart 3000 ROM, November 2005 NEO ROM, and July 2013 NEO/System 3.15 ROM.
The original public/vendor symbol has not been recovered, so the SDK retains the
neutral `SYS_A278` name.

## Reconstructed public form

```c
void *SYS_A278(void);
```

A278 consumes no stack arguments and returns either a settings-region pointer or
`NULL` in full `D0`.

The handler reads the runtime index of the SmartApplet that is currently
executing. If the index is outside the normal `0..31` domain, it returns `NULL`.
For an in-range index it loads the corresponding `AppletHeader_t` pointer and
reads the longword at header `+0x0c`.

The historical BetaWise `AppletHeader_t` layout independently identifies that
field as `settingsOffset`:

| Offset | Field |
| ---: | --- |
| `+0x00` | `signature` |
| `+0x04` | `romUsage` |
| `+0x08` | `ramUsage` |
| `+0x0c` | `settingsOffset` |
| `+0x10` | `flags` |
| `+0x14` | `id` |

A zero `settingsOffset` returns `NULL`. Otherwise the handler converts the offset
to an absolute address. Ordinary SmartApplets use their runtime header/base
pointer as the relocation base. Runtime index 0 is special: System 3 uses its ROM
runtime base explicitly rather than the table pointer.

Equivalent pseudocode is:

```text
index = current_applet_runtime_index
if (index >= 32)
    return NULL

header = applet_header_table[index]
if (header->settingsOffset == 0)
    return NULL

base = (index == 0) ? system_runtime_base : header
return base + header->settingsOffset
```

## Runtime state correlation

The three implementations are instruction-for-instruction equivalent apart from
relocated globals, tables and System 3 bases.

| ROM | A278 | current index | header table | System base |
| --- | ---: | ---: | ---: | ---: |
| AS3000 2005 | `0x4D43C4` | `0x3552` | `0x0D82` | `0x4C0000` |
| NEO 2005 | `0x5D65C2` | `0x35AA` | `0x0DDA` | `0x5C0000` |
| NEO 2013 | `0x427A22` | `0x35E2` | `0x0E0A` | `0x410000` |

The current-index global is the same execution context that
`AppletSendMessage`/A244 temporarily replaces while invoking another applet and
restores afterward. It is distinct from the selected-applet context managed by
A22C–A234.

## Caller confirmation

Each compared ROM contains one equivalent direct internal caller. The caller
passes no argument, saves the returned `D0`, and tests it for zero before using
the result. This independently confirms the no-argument nullable-pointer shape.

## Validation and limits

Mechanically confirmed:

- no arguments;
- full-width pointer/`NULL` return in `D0`;
- normal runtime-index domain `0..31`;
- lookup through the 32-entry `AppletHeader_t *` table;
- exact use of `AppletHeader_t.settingsOffset`;
- explicit System-app index-0 relocation rule;
- equivalent behavior on the three compared ROM generations.

The descriptive phrase **current SmartApplet settings pointer** is functional
terminology, not a claim about the lost vendor symbol.

A278 does not explicitly NULL-check `applet_header_table[index]` before reading
`+0x0c`. Normal runtime state therefore must provide a valid installed header for
the current execution index. Do not create corrupted runtime-index states on
irreplaceable hardware merely to test this failure mode.

No emulator or hardware regression is claimed as already executed by this note.
The closure is based on cross-ROM static correlation plus the independent public
header layout and direct callers.
