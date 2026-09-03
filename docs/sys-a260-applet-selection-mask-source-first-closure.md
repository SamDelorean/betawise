# SYS_A260 — applet selection/state mask getter — source-first closure

## Status

`A260/index152` is **CLOSED A / SOURCE-FIRST** for AlphaSmart 3000 2005, NEO 2005 and NEO/System 3.15 2013.

No trustworthy original vendor symbol has been recovered, so the neutral name remains:

```c
uint32_t SYS_A260(uint32_t applet_index);
```

## Source-first correlation

Earlier correlated work on A22C and the A260–A270 family established a shared 32-entry longword table indexed by SmartApplet runtime index. A22C queries A260 and permits installation of the requested applet context only when the returned aggregate mask is zero. Neighboring A264/A268/A26C/A270 manipulate bits in the same table.

No official user manual exposes this internal table or syscall. User/Manager documentation can corroborate high-level applet enable/disable and selection behavior, but the ABI and table mechanics are firmware-derived and no UI term is promoted to a vendor API name.

## Firmware-confirmed contract

All three canonical handlers are exactly `0x12` bytes and are instruction-for-instruction equivalent apart from the relocated RAM table base.

The handler:

1. reads the complete 32-bit `applet_index` argument slot;
2. scales it by four;
3. adds the generation-specific base of the shared longword mask table;
4. returns the 32-bit table entry unchanged in D0.

There is **no bounds check, normalization, helper call, or side effect** in A260 itself. Consequently callers must supply a valid runtime index when they require defined table access.

The table bases differ only with RAM layout: AS3000 2005 `0x0000344E`, NEO 2005 `0x000034A6`, and NEO 2013 `0x000034DE`.

## Relationship to A22C

A22C provides the strongest semantic caller correlation: it calls A260 for the candidate runtime applet and accepts the selection only when the returned mask is zero. A260 therefore exposes the aggregate selection/state mask; it is not a boolean helper and must return the full 32-bit value.

This also explains the rest of the family without inventing vendor names: A264 sets a persistent/base bit, A268 clears an entry, and A26C/A270 add/remove a dynamically selected reason bit.

## Caller evidence

A complete direct absolute-JSR scan finds **10 / 12 / 13** native callers in AS3000 2005 / NEO 2005 / NEO 2013 respectively. The larger caller population relative to A22C is consistent with the table being a shared runtime gate/state primitive rather than a wrapper dedicated solely to applet selection.

## Refutation

The firmware refutes interpretations of A260 as:

- a boolean `can_select` query — it returns the raw longword mask;
- a mutator — the handler performs no write;
- an index-validating API — no range check exists;
- a byte/word-indexed table — the index is consumed as a full 32-bit slot and scaled by four.

## Regression

Static source-first regression: **47 / 47 PASS — EXECUTED**.

Coverage includes canonical ROM hashes, vector[152]/vector[153] placement, exact `0x12` handler bounds, full-longword argument load, x4 indexing, generation-specific table bases, raw longword return, absence of branching/bounds logic, normalized 3/3 handler equivalence, and complete direct-JSR counts.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**. Emulator/device tests should query known valid applet indices across zero and nonzero mask states and verify that A260 itself does not modify table contents.

## Publication boundary

This document publishes only the recovered contract and reproducible conclusions. ROM bytes, extensive disassembly and private binary workpapers remain outside the public repository.
