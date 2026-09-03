# A260–A270 SmartApplet selection-block mask family

## Current audit status

The historical family reconstruction is retained as context, but the current
source-first audit closes services individually. The following members have now
been revalidated directly against the three canonical ROMs with **confidence A**:

- `A260` — source-first closed
- `A264` — source-first closed
- `A268` — source-first closed

`A26C` and `A270` retain their earlier mechanical reconstruction only and remain
pending under the restarted source-first audit. They must not be treated as
freshly closed merely because the older family analysis grouped them here.

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
- `A26C` and `A270` retain the signatures above as prior reconstruction anchors
  pending their individual source-first reaudits.

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

not a single-bit clear. This distinction matters because A26C/A270 can place
additional blocking-reason bits in the same aggregate entry.

The three canonical handlers are mechanically equivalent except for the
relocated RAM table base. Complete direct-JSR scanning finds 2/2/2 native
callers across AS3000 2005 / NEO 2005 / NEO 2013. Both caller sites pass the
runtime SmartApplet index as a longword and occur in the complementary
SmartApplet enable path previously correlated with A264's disable path.

Static regression for the A268 re-audit is **33/33 PASS (executed)**. Dynamic
emulator/hardware regression remains specified but not executed and is not
claimed by this closure.

## Semantic confidence

**CONFIRMED:** A268 clears the whole aggregate selection-block entry for the
supplied runtime SmartApplet index, with no internal range validation.

**STRONG INFERENCE:** in the System 3 enable/disable workflow, clearing the
aggregate entry corresponds to the enabling path. This inference is supported
by the paired native callers, A260/A264/A22C data-flow, and the documented
SmartApplet enable/disable behavior; it is intentionally kept separate from the
raw primitive contract.

**UNKNOWN:** the original vendor symbol for A268 and any vendor name for the
shared mask table.

## Safety and validation

A260, A264 and A268 do not validate `applet_index`; callers must keep it within
the 32-entry runtime table. A268 clears every accumulated blocking/state reason
in the selected entry rather than merely undoing A264 bit 0.

## Publication boundary

This specification and the generic header contain only the reconstructed
contract and reproducible high-level evidence. ROM bytes, verbatim extensive
disassembly, complete private xrefs and workpapers remain outside the public
repository.
