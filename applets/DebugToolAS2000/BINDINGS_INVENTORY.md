# DebugTool-AS2000 — Increment 1 dependency inventory

Status: **INCREMENT 1 CLOSED**

This inventory is the only dependency-discovery artifact required by
`WORKPLAN.md` Increment 1. It records the smallest set of external services and
state needed by MEM, GOTO, EDIT, CALL and INFO. It does not authorize ROM
placement, RAM allocation, new DynFS services, or a broader reverse-engineering
campaign.

## Status legend

- **VERIFIED** — behavior is established strongly enough to bind against.
- **CANDIDATE** — suitable interface identified; exact binding or one narrow
  semantic detail is intentionally deferred.
- **UNRESOLVED_LATE_BINDING** — value cannot/should not be fixed until resident
  DynFS/current-ROM or later-ROM placement is available. This does not block
  the portable core.
- **REJECTED** — historical idea that must not become a dependency.

## Evidence baseline

Current stock firmware baseline used for narrow verification:

- AlphaSmart 2000 v3.1.4
- stock ROM SHA-1 documented by the AS2000 project:
  `e0b777dc68c671c31ba808e214fb9d2573b9a853`

Current firmware/DynFS project baseline:

- `/home/spc/Projects/alphasmart/AS2K-V3.14.x`
- current Profile 1 logical surface: 19/19 services implemented
- resident integration remains responsible for final addresses/bindings

Only behavioral summaries and addresses are recorded here; private ROM
disassembly is not copied into this repository.

---

## 1. LCD/output dependencies

### `ROM_LCD_CLEAR` -> stock `$A3D6`

Status: **VERIFIED**

Observed behavior:

- issues LCD clear command `01h` to both display-controller halves;
- then enters the stock first-row/controller positioning path.

Use:

- initial DebugTool screen clear/reset;
- deterministic redraw start for MEM/INFO.

Binding note:

- treat it as a stock display-reset/clear primitive, not as a pure side-effect-
  free byte clear routine.

### `ROM_LCD_PUTS` -> stock `$A435`

Status: **VERIFIED**

Observed behavior:

- X points to a zero-terminated stock string;
- emits bytes through the stock LCD byte-output primitive;
- advances X until the zero terminator and returns.

Use:

- fixed DebugTool labels only when this is smaller than repeated character
  calls.

### `ROM_LCD_PUTBYTE` -> stock `$A44B`

Status: **VERIFIED**

Observed behavior:

- emits the byte supplied in B through the stock two-nibble LCD path;
- waits on the LCD busy state through `$9400`;
- data/command selection is controlled by stock display state.

Use:

- normal DebugTool character output;
- output of the two ASCII digits returned by `$9350`.

Precondition:

- DebugTool must enter through one of the stock row/command paths before
  assuming ordinary data mode.

### `ROM_LCD_COMMAND` -> stock `$A440`

Status: **VERIFIED**

Observed behavior:

- accepts command byte in A;
- temporarily selects command mode;
- sends it through `$A44B`;
- restores normal data mode.

Use:

- direct DDRAM/cursor commands only if doing so is smaller than a stock row
  helper.

### Four stock row/controller selectors

Verified row order:

- row 0 -> `$A54B`
- row 1 -> `$A556`
- row 2 -> `$A561`
- row 3 -> `$A56C`

Status: **VERIFIED**

Evidence:

- the four paths select the two LCD-controller halves and issue DDRAM commands
  `80h` or `C0h`;
- stock `$9FEF/$A0FD` display traversal uses them in that order;
- Increment 4 MAME MEM rendering produced the expected physical top-to-bottom
  40x4 row order.

Decision:

- MEM uses these four stock helpers directly; no local DDRAM row calculator is
  required.

---

## 2. Hex conversion

### `ROM_BYTE_TO_HEX` -> stock `$9350`

Status: **VERIFIED**

Observed contract:

- input: A = byte;
- output: A = uppercase ASCII high hexadecimal digit;
- output: B = uppercase ASCII low hexadecimal digit;
- handles 0-9 and A-F directly.

### `ROM_OUT_HEX` -> stock `$A3A0`

Status: **VERIFIED / USED BY CONSOLIDATED v0**

Observed implementation:

- calls `$9350`;
- preserves the low ASCII digit;
- emits the high digit through `$A44B`;
- emits the low digit through `$A44B`.

Increment 9 proved this is exactly equivalent to the former local
`DBG_OUT_HEX_A` helper. The local 12-byte duplicate was removed and the core
now consumes symbolic `DBG_BIND_OUT_HEX`.

Decision:

- use `$A3A0` for all two-digit hexadecimal rendering;
- `$9350` remains a verified stock primitive/internal dependency but is no
  longer required directly by the portable core.

---

## 3. Keyboard dependency

### `ROM_KEY_SCAN_PUMP` -> stock `$89B6`

Status: **VERIFIED**

Retained stock keyboard loops call this before dequeuing. A DebugTool polling
loop must not treat `$938C` alone as a complete keyboard service.

### `ROM_KEY_DEQUEUE` -> stock `$938C`

Status: **VERIFIED**

Observed contract:

- consumes the stock local keyboard ring queue;
- returns raw key code in A with V=0 when one is available;
- returns V=1 when the queue is empty;
- queue cursors are stock `$008E/$008F`;
- the retained queue storage is `$0090-$009F`.

### `ROM_KEY_TRANSLATE` -> stock `$A33C`

Status: **VERIFIED**

Observed contract:

- input A = raw dequeued key code;
- returns translated character directly in A with V=0;
- V=1 means no character translation;
- stock caller `$90B9` immediately transfers returned A to B and writes it to
  the LCD, confirming A is the translated character;
- stock byte `$0070` is raw/dequeued key state, not the translated output.

### DebugTool keyboard service

Status: **VERIFIED**

The diagnostic binding exposes one non-blocking `DBG_BIND_KEY_GETCHAR` that
performs:

`$89B6 -> $938C -> $A33C`

and returns A=character/V=0 or V=1 when no usable character is available.

Decision:

- reuse the complete stock scan/dequeue/translate path;
- do not reproduce matrix scan, debounce, modifier or character-table logic.

Optional stock `$93A3` queue peek behavior is not needed by v0.

---

## 4. Physical memory and banking

### Fixed CPU-visible map

Status: **VERIFIED**

Current AS2000 model:

- `$8000-$FFFF`: fixed main firmware ROM for reads.
- lower `$0000-$7FFF`:
  - PA6=1 -> selected RAM bank;
  - PA6=0 -> I/O + DictROM view.
- RAM = 4 x 32 KiB banks.
- PA5:PA4 select RAM bank 0..3.
- write `$9000` is a keyboard-matrix MMIO overlay even though reads in the
  upper half are ROM.

### `ROM_RAM_BANK_BITS` -> stock `$9466`

Status: **VERIFIED**

Observed contract:

- input A bits 1:0 select RAM-bank bits PA5:PA4;
- only PA4/PA5 are changed;
- PA6 is **not** set by this routine.

Use:

- candidate primitive inside the current-ROM RAM-selection adapter.

### `ROM_RAM_BANK0_BITS` -> stock `$947F`

Status: **VERIFIED**

Observed contract:

- clears PA4 and PA5;
- does **not** by itself restore PA6/view state.

Decision:

- do not describe `$947F` as a complete view restore.
- the DebugTool binding must preserve/restore the full relevant PORTA state.

### `$9486` stock F-key-derived bank selection

Status: **VERIFIED but NOT USED as the DebugTool bank API**

Observed behavior:

- derives the stock bank from F-key selector `$018E`;
- is correct for retained stock contiguous-file behavior.

Reason not used:

- MEM/GOTO/EDIT require arbitrary explicit bank selection independent of F1-F8.

### DebugTool RAM access adapters

Status: **VERIFIED**

Increment 4 proved the immediate/non-nestable RAM ENTER/RESTORE pair used by
MEM:

1. save incoming CCR and PORTA on the CPU stack;
2. select PA6=1 plus requested PA5:PA4 bank;
3. perform the physical read;
4. restore PORTA;
5. restore the exact incoming CCR.

Increment 6 added and proved an atomic `DBG_BIND_RAM_WRITE` used by EDIT:

- input A = bank 0..3;
- input B = byte value;
- input X = lower-half CPU address;
- the target, value, CCR and PORTA are carried in registers/CPU stack;
- no DebugTool workspace byte is required while another bank is visible;
- the original mapping is restored before return.

Both adapters were exercised in `as2kdiag` against multiple physical banks.

### Physical block geometry helper

`FS_BLOCK_MAP`

Status: **VERIFIED placement-free symbol; optional for DebugTool**

Contract:

- A = physical block_id `00..7F`;
- returns B = bank 0..3;
- returns X = CPU offset `0000..7C00`.

Decision:

- not required by MEM/GOTO/EDIT.
- INFO or CALL may expose/test it later through normal CALL, but the debugger
  core does not depend on it.

---

## 5. Safe MEM/EDIT access policy

Status: **VERIFIED**

### MEM

Allowed v0 reads:

- fixed ROM `$8000-$FFFF`;
- banked RAM `bank:0000-7FFF` through the RAM-select adapter.

### EDIT

Allowed v0 writes:

- banked RAM `bank:0000-7FFF` only.

Forbidden in v0:

- writes to `$8000-$FFFF`;
- therefore no accidental write side effect at the `$9000` keyboard overlay;
- direct I/O/DictROM editing;
- arbitrary MMIO writes.

Reason:

EDIT is intended to inspect/intervene in DynFS RAM, not to become an unrestricted
hardware register editor.

The CALL probe remains the controlled route for deliberately exercising known
firmware routines.

---

## 6. CALL dependency

### Core mechanism

Status: **VERIFIED in Increment 7**

Contract:

- target = 16-bit HC11 executable address;
- pre-call values = A, B/D, X, Y as defined by Increment 2;
- DebugTool saves the state needed to survive the call;
- the target is invoked by one HC11-native indirect/direct call mechanism;
- post-call A/B(D)/X/Y and CCR are captured where practical;
- returns safely to DebugTool.

No BetaWise six-`uint32_t` ABI and no 68k A-line syscall mechanism are
dependencies.

Increment 7 proved the same native engine against:

- a synthetic A/B/X/Y verifier/return target;
- stock `$9350`;
- the actual placement-free DynFS `FS_BLOCK_MAP` source assembled into the
  disposable MAME test image.

The diagnostic binding also exposes verified PORTA snapshot/restore helpers
`DBG_BIND_ENV_GET/SET`. Final resident target addresses remain late-bound.

### DebugTool entry/exit

Status: **CANDIDATE core / UNRESOLVED_LATE_BINDING hook**

Core contract:

- environment invokes one `DEBUGTOOL_ENTRY` subroutine;
- normal exit returns to the caller.

Deferred bindings:

- current-ROM activation hook;
- later-ROM activation hook;
- MAME diagnostic trampoline/harness.

No keyboard shortcut or stock patch site is selected in Increment 1.

---

## 7. DynFS callable symbols

The following are **VERIFIED placement-free symbols** in the current Profile 1
implementation, but their final resident addresses are
**UNRESOLVED_LATE_BINDING**:

- `FS_FSLOT_RESOLVE`
- `FS_FSLOT_ASSIGN`
- `FS_FSLOT_UNASSIGN`
- `FS_STREAM_READ`
- `FS_STREAM_GETC_CTX`
- `FS_STOCK_GETC_CTX`
- `FS_STOCK_READ`
- `FS_STOCK_REPLACE1`
- `FS_SERVICE_READY`
- `FS_BLOCK_MAP`
- `FS_PORTA_RAM_VALUE`

DebugTool does not require dedicated private entry points for these functions.
Once resident addresses exist, CALL can invoke any stable one using the normal
HC11 CALL engine.

### Historical names explicitly rejected

- `BLOCK_SELECT`
- `BLOCK_ADDR`
- historical `SLOT_TO_FILE`
- historical v0.19/`E103...` resident jump-table assumptions

Status: **REJECTED as current dependencies**

Current source of truth defines `FS_FSLOT_RESOLVE` as the logical replacement
for historical `SLOT_TO_FILE`, and current repository audit does not show a
live public `BLOCK_SELECT/BLOCK_ADDR` or old `E103` resident ABI.

In particular, protected stock ROM metadata at `$E102-$E103` must not be
reinterpreted as DebugTool/DynFS service-table space.

---

## 8. INFO diagnostic-state dependencies

### Stock/DynFS compatibility state

Status: **VERIFIED**

Current compatibility ABI:

- `$018E` — active F1-F8 selector;
- `$0122` — encoded logical cursor/edit position;
- `$0120` — encoded logical view origin;
- `$0124` — encoded logical end/length;
- `$0126` — synthesized compatibility capacity/end guard;
- `$0128` — encoded logical start/origin;
- `$0067` — encoded sequential stream position when active.

Important:

- these address-shaped words are compatibility state;
- they are **not** authoritative physical DynFS payload pointers.

INFO may display them explicitly to diagnose stock<->DynFS translation.

### Active DynFS compatibility context

Status: **VERIFIED SHAPE / UNRESOLVED_LATE_BINDING ADDRESS**

Frozen 7-byte shape:

- +0 mount table pointer16
- +2 sparse payload-view pointer16
- +4 file_id8
- +5 compat_base16

No resident RAM address is assigned yet.

Decision:

- INFO source refers to a symbolic `DBG_DYNFS_ACTIVE_CTX` binding.
- Current/final ROM integration supplies its actual address only after the
  DynFS resident plan proves lifetime and RAM ownership.

### v0 INFO values

Status: **VERIFIED in Increment 8**

The final minimum read-only INFO screen displays:

- F-slot from `$018E`;
- file_id from the bound active context;
- DebugTool MEM inspection bank;
- payload-view pointer;
- compat_base;
- cursor offset = `$0122 - compat_base`;
- view offset = `$0120 - compat_base`;
- logical end/length = `$0124 - compat_base`.

Increment 8 matched all displayed values against independent MAME CPU-space
reads across two consecutive slot/file/context states.

Mount pointer, origin, sequential position and capacity remain valid bound
state but are deliberately omitted from the v0 screen to keep INFO inside its
150-byte hard ceiling. Fields requiring allocator/extent traversal are not part
of v0 INFO.

---

## 9. Final late-bound dependency table

| Binding | Increment-1 status | Why deferred |
|---|---|---|
| DebugTool ROM base | UNRESOLVED_LATE_BINDING | placement belongs to integration |
| DebugTool entry hook | UNRESOLVED_LATE_BINDING | current/future ROM differ |
| DebugTool workspace RAM base | UNRESOLVED_LATE_BINDING | exact RAM ledger belongs to Increment 10 |
| Active 7-byte DynFS context address | UNRESOLVED_LATE_BINDING | resident DynFS integration assigns lifetime/address |
| Resident addresses of placement-free DynFS symbols | UNRESOLVED_LATE_BINDING | linker/placement result |
| Later-ROM equivalents of stock LCD/key helpers | UNRESOLVED_LATE_BINDING | use binding layer; core remains unchanged |

None of these requires a core-logic redesign.

---

## Increment 1 exit check

Required dependency classes from `WORKPLAN.md`:

- LCD clear/output/cursor: **VERIFIED, including 40x4 row mapping**
- keyboard read/decode: **VERIFIED**
- bank selection/window behavior: **VERIFIED; DebugTool adapter proven in Increment 4**
- safe byte read/write constraints: **VERIFIED**
- debugger entry/exit mechanism: **CANDIDATE**, physical hook late-bound
- stock conversion helper: **VERIFIED**
- stable DynFS symbols/state already available: **VERIFIED symbols/state**,
  resident addresses late-bound

**Exit criterion: PASS.**

The only unresolved values are deliberately placement-dependent addresses.
They are not blockers for the portable core.

Next and only authorized increment: **Increment 2 — ABI and data-state freeze**.
