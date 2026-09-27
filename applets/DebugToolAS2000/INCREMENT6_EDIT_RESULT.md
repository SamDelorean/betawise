# DebugTool-AS2000 — Increment 6 EDIT result

Status: **INCREMENT 6 CLOSED / PASS**

Increment 6 implemented and validated only EDIT on top of the already-closed
MEM and GOTO increments. No CALL or INFO behavior was added.

## Implemented behavior

EDIT targets:

`selected_address = mem_addr + mem_cursor`

v0 write policy:

- lower-half banked RAM `$0000-$7FFF`: writable;
- fixed-ROM / upper-half `$8000-$FFFF`: protected, no write attempted;
- no direct I/O/DictROM editing;
- no arbitrary MMIO write path.

Input modes:

- default HEX: exactly two hexadecimal nibbles commit one byte;
- Tab switches to ASCII;
- ASCII: one printable `20h..7Eh` character commits directly;
- Tab switches back to HEX;
- Escape cancels;
- changing modes discards a partial HEX nibble.

HEX/ASCII mode is represented by control flow, not by a persistent state byte.
This preserves the frozen Increment-2 30-byte workspace ABI exactly.

## Atomic RAM-write binding

The environment binding now exposes:

`DBG_BIND_RAM_WRITE`

Contract:

- A = bank 0..3
- B = byte value
- X = lower-half CPU address

The binding:

1. keeps the target address in Y;
2. saves the byte value, incoming CCR and PORTA on the CPU stack;
3. selects PA6=1 plus requested PA5:PA4 bank;
4. performs the physical byte write;
5. restores PORTA;
6. restores the exact incoming CCR;
7. returns with the original mapping re-established.

No DebugTool workspace byte is required while the alternate RAM bank is
visible.

## Measured size

Final GNU m68hc11 assembler/linker checkpoint:

| Component | Increment 5 | Increment 6 | Delta |
|---|---:|---:|---:|
| portable core .text | 449 B | 675 B | +226 B |
| diagnostic binding .text | 54 B | 94 B | +40 B |
| fixed DebugTool workspace | 30 B | 30 B | 0 B |
| product .data | 0 B | 0 B | 0 B |

EDIT-specific portable-core cost is therefore **226 B**, inside the 230-byte
hard planning ceiling.

Current executable checkpoint:

**675 B core + 94 B binding = 769 B**

This remains below the project's aggressive 1024-byte total target at this
stage.

## Deterministic HC11/MAME matrix

A compiled self-test ran under the existing `as2kdiag` build.

### HEX nibble correctness

Physical bank 2, address `$2345` was initialized to `3C` ('<').

- after first input nibble `5`: direct physical read still returned `3C`;
- after second nibble `A`: direct physical read returned `5A` ('Z').

Thus no half-entered byte is written.

### Banked target

The same HEX test targeted bank 2 explicitly and was verified by a direct
manual PORTA bank switch/read in the test harness, independently of MEM and the
DebugTool write binding.

### ASCII edit

A second target at bank 3, address `$3007` was initialized to '?'.

The ASCII edit primitive wrote `Q` and an independent physical read returned
`51h`.

### Protected ROM

The test selected `$8000` and attempted HEX edit `55`.

The byte remained the stock `8Eh`.

No upper-half write was issued through the RAM-write binding.

### MEM redraw

The bank-2 row was seeded as:

`EDIT-<OK`

After the HEX edit, MEM displayed:

`B2:2340 45 44 49 54 2D 5A 4F 4B EDIT-ZOK`

This independently confirms the physical result through the already-validated
MEM read path.

## Full physical-keyboard path

A final user-facing test exercised `DBG_EDIT_RUN` rather than calling parser
primitives directly.

The harness seeded physical bank 1 at `$1234` as:

`EDIT<OK!`

and selected cursor offset +4.

The MAME keyboard then sent:

1. Tab — switch HEX -> ASCII;
2. `q` — commit one ASCII byte.

The resulting MEM redraw showed:

`B1:1234 45 44 49 54 71 4F 4B 21 EDITqOK!`

Therefore the complete tested path is:

`physical key -> stock scan/dequeue/translate -> EDIT mode -> atomic banked RAM write -> MEM redraw`

## Scope control

Not added:

- cursor movement/navigation
- backspace editing
- multi-byte writes
- ROM patching
- I/O/MMIO editing
- DynFS-aware writes
- CALL
- INFO

One bounded size correction was made to preserve the frozen ABI and stay below
the EDIT hard ceiling. No further optimization branch was opened.

## Exit check

Required by `WORKPLAN.md` Increment 6:

- RAM byte edit: **PASS**
- nibble/byte correctness: **PASS**
- banked RAM target: **PASS**
- HEX editing: **PASS**
- ASCII editing: **PASS**
- redraw reflects physical result: **PASS**
- protected upper-half target: **PASS**
- physical result independently observed: **PASS**
- exact ROM/RAM delta recorded: **PASS**
- no scope growth into CALL/INFO: **PASS**

**Increment 6 exit criterion: PASS.**

Next and only authorized increment:

**Increment 7 — CALL only.**
