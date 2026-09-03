# A260–A270 SmartApplet selection-block mask family

## Current audit status

The historical family reconstruction is retained as context, but the current
source-first audit closes services individually. The following members have now
been revalidated directly against the three canonical ROMs with **confidence A**:

- `A260` — source-first closed
- `A264` — source-first closed
- `A268` — source-first closed
- `A26C` — source-first closed

`A270` retains its earlier mechanical reconstruction only and remains pending
under the restarted source-first audit. It must not be treated as freshly closed
merely because the older family analysis grouped it here.

Reliable original public/vendor symbols have not been recovered, so the neutral
`SYS_Axxx` names remain.

## Shared model

The five services operate on the same runtime table of 32-bit masks indexed by
SmartApplet runtime index. A nonzero aggregate mask prevents the selection path
used by A22C; the table must not be modeled as a Boolean array.

## ABI contracts

```c
uint32_t SYS_A260(uint32_t applet_index);
void SYS_A264(uint32_t applet_index);
void SYS_A268(uint32_t applet_index);
void SYS_A26C(uint32_t applet_index);
void SYS_A270(uint32_t applet_index);
```

- `A260` returns the complete 32-bit mask at `applet_index`. It performs no
  range check.
- `A264` sets bit 0 of the selected entry and preserves every other bit. It
  performs no range check.
- `A268` clears the complete 32-bit entry. It performs no range check.
- `A26C` accepts only indexes 1 through 31 and ORs one dynamically selected bit
  into the existing aggregate entry.
- `A270` retains the signature above as a prior reconstruction anchor pending
  its individual source-first reaudit.

## A268 source-first closure

`A268/index154` is confirmed as:

```c
void SYS_A268(uint32_t applet_index);
```

Direct firmware verification on the canonical AS3000 2005, NEO 2005 and NEO
2013 images shows the same 0x12-byte handler shape in all three generations:
the full 32-bit argument is loaded, multiplied by four, added to the relocated
mask-table base, the complete selected longword is cleared, and the routine
returns. There is no bounds check, helper call, branch, Boolean normalization,
or semantic return value; D0 is address scratch.

The handler therefore implements:

```c
mask_table[applet_index] = 0;
```

not a single-bit clear. Static regression for the A268 re-audit is **33/33 PASS
(executed)**. Dynamic emulator/hardware regression remains specified but not
executed.

## A26C source-first closure

`A26C/index155` is confirmed as:

```c
void SYS_A26C(uint32_t applet_index);
```

The canonical AS3000 2005, NEO 2005 and NEO 2013 handlers are each 0x2C bytes
and are mechanically equivalent except for relocated RAM addresses. The handler
loads the complete longword argument, rejects index zero and every index greater
than or equal to 32, seeds a longword mask with one, reads a byte-sized global
selector, shifts the mask left by that selector, scales the runtime applet index
by four, and ORs the resulting bit into the existing 32-bit table entry.

The raw operation is therefore:

```c
if (applet_index != 0 && applet_index < 32)
    mask_table[applet_index] |= (1u << selector_byte);
```

This is not another bit-0 setter and not a whole-entry clear. Existing mask bits
are preserved. No helper call or semantic return exists in the handler; the
return type is therefore `void`.

Complete direct absolute-JSR scanning of the three System 3 ROMs finds no direct
calls to A26C (0/0/0). This negative result does not imply that the service is
unused: the earlier reproducible linked-applet survey identifies paired
A26C/A270 use in AlphaQuiz and AlphaWord Plus. Applet syscall-table entries are
kept distinct from call sites in the evidence model.

Static regression for the A26C re-audit is **63/63 PASS (executed)**. It covers
all three canonical hashes, handler bounds, longword argument handling, the
1..31 range guards, relocated selector/table references, dynamic shift, index
scaling, OR semantics, epilogue, absence of helper calls, and normalized
cross-ROM equivalence. Dynamic regression is specified but not executed.

## Semantic confidence

**CONFIRMED:** A268 clears the whole aggregate selection-block entry for the
supplied runtime SmartApplet index, with no internal range validation.

**CONFIRMED:** A26C accepts runtime indexes 1..31 and adds exactly one bit chosen
by a byte-sized OS-global selector while preserving all previously set bits.

**STRONG INFERENCE:** the A26C selector is a dynamic blocking-reason/context
index shared with A270. A22C/A260 establish that a nonzero aggregate mask blocks
selection, but the original vendor name and the semantic enumeration of selector
values have not been recovered.

**UNKNOWN:** the original vendor symbols for A268/A26C and vendor names for the
shared mask table and A26C/A270 selector byte.

## Safety and validation

A260, A264 and A268 do not validate `applet_index`; callers must keep it within
the 32-entry runtime table. A26C does validate and silently ignores index 0 and
indexes >=32. A268 clears every accumulated blocking/state reason in the entry,
whereas A26C preserves the aggregate and adds one selected reason bit.

## Publication boundary

This specification and the generic header contain only the reconstructed
contract and reproducible high-level evidence. ROM bytes, verbatim extensive
disassembly, complete private xrefs and workpapers remain outside the public
repository.
