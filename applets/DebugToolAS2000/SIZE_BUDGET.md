# DebugTool-AS2000 size budget

This file is a design constraint, not a prediction. Actual assembled size wins.

| Module | Target bytes | Hard planning ceiling |
|---|---:|---:|
| Entry/exit + main key dispatcher | 96 | 160 |
| Memory view/redraw | 180 | 260 |
| Cursor/navigation | 80 | 140 |
| Hex/ASCII byte editor | 150 | 230 |
| GOTO bank:address input | 100 | 180 |
| HC11 CALL interface | 150 | 260 |
| INFO/status display | 80 | 150 |
| Hex conversion/format helpers | 70 | 120 |
| ROM-call glue / bank glue | 100 | 180 |
| Strings/tables/constants | 80 | 140 |
| **Total planning target** | **1086** | **1820** |

## Acceptance gates

- **GREEN:** <= 1024 bytes. Integrate when functional tests pass.
- **ACCEPTABLE:** 1025-1536 bytes. Normal target range.
- **REVIEW:** 1537-1792 bytes. Audit for duplicated stock-ROM functionality.
- **STOP/REDESIGN:** >1792 bytes before optional features.

Optional/deferred behaviors such as indirect GOTO, previous-address history,
Home/End navigation, or richer INFO output may only be added after the five
v0 functions are complete and the assembled core remains inside the accepted
budget.

## RAM budget

Target additional persistent RAM: <= 50 bytes.

Preferred state representation:

- current address: 16 bits
- previous address: optional 16 bits
- current bank: byte/bitfield as required
- cursor/mode: packed bytes
- one short shared input buffer only if direct key-entry cannot replace it
- no 256-byte scratch region in v0
- no seven-buffer CALL dialog

## Measurement rule

Every implementation increment that changes code must record:

1. assembler output size,
2. code bytes,
3. constant/string bytes,
4. persistent RAM bytes,
5. delta versus previous build.

No feature is accepted solely because the source looks small.


## Measured implementation checkpoints

| Checkpoint | Core .text | Binding .text | Fixed DebugTool RAM | Notes |
|---|---:|---:|---:|---|
| Increment 3 skeleton | 1 B | 2 B | 30 B | no product function |
| Increment 4 MEM | 248 B | 38 B | 30 B | MEM + real diagnostic RAM adapter |

Increment 4 adds **+247 B core** and **+36 B diagnostic bank glue** versus the
Increment 3 skeleton, with **0 B RAM growth**. The 7-byte synthetic DynFS
context in the diagnostic link is external state and is not counted as
DebugTool RAM.


| Increment 5 GOTO | 449 B | 54 B | 30 B | MEM + GOTO + complete stock keyboard binding |

Increment 5 delta versus Increment 4 is **+201 B core** and **+16 B binding**,
with **0 B RAM growth**. Of the core delta, **28 B** is the shared ASCII-hex
nibble helper; GOTO-specific core cost is therefore **173 B**, inside its
180-byte hard planning ceiling.
