# DebugTool-AS2000 — Increment 8 INFO result

Status: **INCREMENT 8 CLOSED / PASS**

Increment 8 implemented the fifth and final v0 user-visible function: INFO.
The view is read-only and contains no filesystem repair logic.

## Final v0 INFO fields

INFO deliberately shows only the state required to correlate logical DynFS
state with physical MEM inspection.

### Row 0

`Sxx Fxx Bxx`

- S = stock active F-slot selector
- F = active DynFS file_id
- B = DebugTool physical RAM bank currently selected for MEM inspection

### Row 1

`Pxxxx Kxxxx`

- P = active payload-view pointer
  - `0000` means resident physical-payload mode
  - nonzero means sparse/view context
- K = active compat_base

### Row 2

`Cxxxx Vxxxx Exxxx`

Each field is derived live as a 16-bit offset relative to compat_base:

- C = stock cursor - compat_base
- V = stock view - compat_base
- E = stock end - compat_base

Row 3 is intentionally unused in v0.

## Fields deliberately omitted

The original candidate list also included mount pointer, origin, sequential
position and capacity. They remain valid binding/state references, but are not
needed for the minimum v0 diagnosis and were removed from the display to remain
inside the 150-byte INFO ceiling.

No INFO snapshot is allocated. Every render reads the live bound state.

## Diagnostic interpretation

INFO and MEM have separate roles:

- INFO = logical compatibility/DynFS truth
- MEM = physical bank/RAM truth

A mismatch in S/F/P/K/C/V/E indicates a logical/context translation problem.
If those values are correct but the bytes observed through MEM in the selected
physical bank are wrong, the investigation moves to physical bank/RAM data.

The B field is the DebugTool MEM inspection bank. It is not presented as an
automatically resolved DynFS extent bank.

## Measured size

Final GNU m68hc11 checkpoint:

| Component | Increment 7 | Increment 8 | Delta |
|---|---:|---:|---:|
| portable core .text | 1039 B | 1187 B | +148 B |
| diagnostic binding .text | 102 B | 102 B | 0 B |
| fixed DebugTool workspace | 30 B | 30 B | 0 B |
| product .data | 0 B | 0 B | 0 B |

INFO-specific core cost is **148 B**, below the 150-byte hard planning ceiling.

Current executable checkpoint:

**1187 B core + 102 B binding = 1289 B**

This remains inside the project's normal ACCEPTABLE range of 1025-1536 bytes.

No fixed RAM was added.

## Deterministic MAME transition test

One diagnostic ROM exercised two consecutive logical/physical contexts. The
state transition was triggered through the real stock keyboard path.

### State A

Injected/live state:

- slot = 01
- file_id = 02
- MEM bank = 01
- mount pointer = 4100
- payload view = 5000
- compat_base = 2000
- cursor = 2010
- view = 2008
- end = 2040

Independent MAME Lua reads from the CPU program space reported:

`INFO_OBS_A slot=01 file=02 bank=01 mount=4100 payload=5000 base=2000 cursor=2010 view=2008 end=2040 dC=0010 dV=0008 dE=0040`

INFO displayed:

`S01 F02 B01`

`P5000 K2000`

`C0010 V0008 E0040`

The displayed values match the independently observed memory state exactly.

### State B

After one physical keyboard event, the harness changed the active context to:

- slot = 05
- file_id = 09
- MEM bank = 03
- mount pointer = 4200
- payload view = 0000
- compat_base = 3400
- cursor = 3411
- view = 3400
- end = 3480

Independent MAME Lua reads reported:

`INFO_OBS_B slot=05 file=09 bank=03 mount=4200 payload=0000 base=3400 cursor=3411 view=3400 end=3480 dC=0011 dV=0000 dE=0080`

INFO displayed:

`S05 F09 B03`

`P0000 K3400`

`C0011 V0000 E0080`

Again the displayed state matches the CPU memory observation exactly.

## Required transitions covered

File/slot transition:

- S01/F02 -> S05/F09: **PASS**

Bank/context transition:

- MEM inspection bank 01 -> 03: **PASS**
- payload view 5000 -> 0000 physical mode: **PASS**
- compat_base 2000 -> 3400: **PASS**

Logical offset transition:

- C 0010 -> 0011
- V 0008 -> 0000
- E 0040 -> 0080

All are computed from live values rather than cached INFO state.

## Scope control

Not added:

- filesystem mutation
- allocator/extent traversal
- automatic physical block resolution
- repair functions
- mount-table parser
- history/log screen
- extra INFO pages
- new persistent RAM

No Codex was used.

## Exit check

Required by `WORKPLAN.md` Increment 8:

- smallest DynFS state view: **PASS**
- read-only behavior: **PASS**
- displayed logical state vs independent MAME observation: **PASS**
- file/slot transition: **PASS**
- bank/context-relevant transition: **PASS**
- logical vs physical diagnosis boundary: **PASS**
- exact ROM/RAM delta recorded: **PASS**
- INFO hard ceiling <=150 B: **PASS (148 B)**
- no scope growth: **PASS**

**Increment 8 exit criterion: PASS.**

All five v0 functions now exist:

- MEM
- GOTO
- EDIT
- CALL
- INFO

Next and only authorized increment:

**Increment 9 — v0 consolidation.**
