# DebugTool-AS2000 — Increment 2 ABI and data-state freeze

Status: **INCREMENT 2 CLOSED / PASS**

This document freezes the internal v0 representation before functional
assembly is written. Final absolute RAM/ROM addresses remain late-bound.

The workspace is defined relative to symbolic base `DBG_WS`. No address in
this document claims free RAM in the current or future firmware.

## 1. General representation rules

- CPU model: MC68HC11.
- All 16-bit workspace values use native HC11 big-endian memory order.
- No workspace field requires alignment; byte packing is intentional.
- The DebugTool core is not reentrant.
- One invocation is one debugger session. `DEBUGTOOL_ENTRY` initializes the
  v0 session state deterministically rather than depending on stale RAM.
- Fixed workspace reserved by v0: **30 bytes**.
- Stack use is separate from the 30-byte fixed workspace and is measured per
  implementation increment.

## 2. DebugTool core entry/exit ABI

### `DEBUGTOOL_ENTRY`

Environment/binding calls the core as an RTS-returning subroutine.

Core input registers:

- A: no contract
- B/D: no contract
- X: no contract
- Y: no contract
- CCR: no contract

Core return contract:

- SP is balanced to the caller;
- A/B/X/Y/CCR are clobbered;
- any stock hook that needs register preservation must provide it in the
  environment-specific wrapper, not in the portable core.

This prevents the core from embedding assumptions about the eventual current-
ROM or future-ROM hook site.

### Session initialization

First entry initializes:

- `mem_bank = 0`
- `mem_addr = $8000`
- `mem_cursor = 0`
- `ui_mode = MEM`
- shared input transaction empty
- `input_bank = $FF` (no explicit GOTO bank supplied)
- CALL selected field = TARGET
- CALL result-valid = 0
- CALL input/output storage cleared

No v0 requirement exists to preserve debugger UI state across exits.

## 3. Current memory position

### `mem_bank` — one byte

Canonical values: `0..3`.

It represents the selected **physical RAM bank**.

Rules:

- for `mem_addr < $8000`, MEM/EDIT use `mem_bank`;
- for `mem_addr >= $8000`, reads are from fixed main ROM and `mem_bank` is
  retained but has no effect on the read;
- no ROM pseudo-bank or 32-bit flattened address is introduced;
- an invalid bank is never committed to `mem_bank`.

### `mem_addr` — one 16-bit word

This is the authoritative currently selected CPU address.

It is not the top-left display address and is not a DynFS logical offset.

GOTO behavior:

- address-only form replaces `mem_addr` and preserves `mem_bank`;
- bank:address form validates bank 0..3, then replaces both;
- parsing state is not committed until the complete input is valid.

### `mem_cursor` — one byte

This is only the visible MEM slot occupied by `mem_addr`.

The exact number/layout of visible byte slots is frozen in Increment 4 when
the 40x4 MEM layout is implemented. The representation is already sufficient
because the display can never require more than 255 byte slots.

`mem_addr`, not `mem_cursor`, is the authoritative selected address.

## 4. UI and edit/input state

### `ui_mode` values

- `00` MEM
- `01` EDIT
- `02` GOTO
- `03` CALL
- `04` INFO

No additional hidden user-visible mode is defined for v0.

### Shared direct hexadecimal input

v0 has **no character input buffer**.

The parser uses:

- `input_value` — 16-bit progressive hexadecimal accumulator;
- `input_digits` — number of accepted hex digits;
- `input_bank` — GOTO bank prefix or `FF` when absent.

For every accepted hex digit:

`input_value = (input_value << 4) | nibble`

Mode-specific maximums:

- EDIT byte: 2 digits;
- GOTO address: 4 digits;
- GOTO bank prefix: exactly 1 digit and must resolve to 0..3;
- CALL A or B field: 2 digits;
- CALL target, X or Y field: 4 digits.

EDIT uses `input_digits=0/1` as its nibble phase. A byte write occurs only
after the second valid digit. Cancel before commit performs no physical write.

GOTO with no colon leaves `input_bank=FF`. When a bank prefix is committed,
the accumulator is reset and the following four digits form the address.

## 5. CALL state representation

There is one native HC11 CALL engine.

### Editable pre-call state

- `call_target`: 16-bit executable CPU address
- `call_in_a`: 8-bit A
- `call_in_b`: 8-bit B
- `call_in_x`: 16-bit X
- `call_in_y`: 16-bit Y

D has no independent storage:

`D = call_in_a : call_in_b`

This prevents contradictory A/B/D state.

### Captured post-call state

- `call_out_a`: returned A
- `call_out_b`: returned B
- `call_out_x`: returned X
- `call_out_y`: returned Y
- `call_out_ccr`: returned CCR

Returned D is likewise derived as:

`D = call_out_a : call_out_b`

SP is neither editable nor stored in v0.

### `call_ctl`

One byte:

- bits 0..2 = selected editable field
  - 0 TARGET
  - 1 A
  - 2 B
  - 3 X
  - 4 Y
- bits 3..6 = zero/reserved
- bit 7 = `RESULT_VALID`

Changing any CALL input clears `RESULT_VALID`. Successful return sets it.

## 6. Frozen native CALL convention

### Entry register contract presented to the target

Immediately before transfer:

- A = `call_in_a`
- B = `call_in_b`
- D = A:B
- X = `call_in_x`
- Y = `call_in_y`

CCR is deliberately **not** an input field in v0.

Because the final register loads affect condition codes, **entry CCR is
unspecified**. A routine that requires a caller-selected carry/zero/etc. input
is outside v0 CALL semantics.

### Transfer mechanism

The core uses a stack-built native call rather than a RAM executable
trampoline:

1. save the current environment mapping byte in `env_saved_map`;
2. push the DebugTool post-call capture address as a 16-bit word;
3. push `call_target` as a 16-bit word;
4. load A/B/X/Y from the frozen input state;
5. execute `RTS`.

The first RTS pulls `call_target` into PC. The target therefore begins with
exactly one normal 16-bit return address on the stack, equivalent in stack
shape to entry from an ordinary JSR.

A temporary GNU m68hc11 assembler feasibility probe accepted this mechanism
and the capture sequence; the probe was 54 bytes of .text. This is **not** the
final CALL-module size and is not committed implementation code.

### Target obligations

The target:

- is an RTS-returning subroutine;
- must leave SP balanced to its entry value before RTS;
- may clobber A/B/X/Y/CCR;
- receives no debugger-owned hidden argument frame;
- is not sandboxed or fault-contained.

CALL itself does not add a bank field. The target is interpreted in the
normal CPU execution map supplied by the environment binding. The current
AS2000 binding may therefore restrict CALL to verified executable regions.

### Post-call capture order

The first post-return operations preserve the callee's status exactly:

1. `PSHA` saves returned A;
2. `TPA` copies returned CCR into A;
3. store A to `call_out_ccr`;
4. `PULA` restores returned A;
5. store A, B, X and Y to their result fields;
6. restore the saved environment mapping;
7. mark `RESULT_VALID`.

The Motorola/Freescale programming reference defines PSHA/PSHX/RTS as not
altering condition codes and TPA as `CCR -> A` without changing CCR. Thus
`call_out_ccr` represents the target's returned CCR before debugger rendering
or environment-restoration work changes flags.

## 7. Environment-map scratch byte

### `env_saved_map`

One shared byte, transient lifetime.

Current AS2000 meaning: complete PORTA snapshot needed to restore the lower-
half view and PA5:PA4 bank selection after a debugger-controlled mapping
transaction.

Owners:

- MEM RAM read
- EDIT RAM write
- CALL survivability wrapper

It is not valid across nested DebugTool operations. Reentrancy is prohibited.

Keeping this byte in fixed workspace rather than hidden beneath the CALL
return address ensures the target sees the same stack depth as a normal JSR.

## 8. INFO input/state references

INFO owns **no cached filesystem state**.

It reads live binding symbols and uses only shared scratch for formatting.

### Stock compatibility references

Environment binding supplies symbolic references for:

- active F-slot selector — current stock `$018E`
- view position — current stock `$0120`
- cursor/edit position — current stock `$0122`
- logical end encoding — current stock `$0124`
- capacity compatibility guard — current stock `$0126`
- start/origin encoding — current stock `$0128`
- sequential position — current stock `$0067`

### Active DynFS context

Binding supplies symbolic `DBG_DYNFS_ACTIVE_CTX`.

Frozen shape:

- +0 mount pointer16
- +2 sparse payload-view pointer16
- +4 file_id8
- +5 compat_base16

Total: 7 bytes, owned by DynFS integration, **not by DebugTool workspace**.

INFO may derive, using `compat_base`:

- cursor offset = stock cursor - base
- view offset = stock view - base
- logical length/end = stock end - base
- start offset = stock origin - base
- sequential offset = stock sequential position - base

INFO is read-only. It never repairs or republishes DynFS state.

## 9. Shared scratch rules

Fixed shared scratch:

- `tmp0`: 1 byte
- `tmp1`: 1 byte
- `tmp_word`: 2 bytes

Lifetime: one core primitive/rendering step.

Rules:

- values are not preserved across external stock-ROM or DynFS calls;
- no user-visible state may depend on them after such a call;
- all modules may reuse them;
- no module may allocate another fixed scratch buffer without revising this
  ledger and the <=50-byte gate.

The hardware mapping adapter may use registers/stack in addition to
`env_saved_map`; it receives no additional fixed RAM allocation here.

## 10. Exact first RAM ledger

All offsets are relative to `DBG_WS`; absolute address is deliberately
unassigned.

| Offset | Symbol | Bytes | Owner | Lifetime | Overlay/reuse |
|---:|---|---:|---|---|---|
| +00 | mem_bank | 1 | MEM/GOTO/EDIT | debugger session | none |
| +01..02 | mem_addr | 2 | MEM/GOTO/EDIT | debugger session | none |
| +03 | mem_cursor | 1 | MEM/EDIT | debugger session | none |
| +04 | ui_mode | 1 | dispatcher | debugger session | none |
| +05 | input_digits | 1 | EDIT/GOTO/CALL | current input transaction | shared by the three modes |
| +06 | input_bank | 1 | GOTO | current input transaction | FF sentinel outside bank-prefix use |
| +07..08 | input_value | 2 | EDIT/GOTO/CALL | current input transaction | shared by the three modes |
| +09 | call_ctl | 1 | CALL | debugger session | field + result-valid packed |
| +0A..0B | call_target | 2 | CALL | debugger session | none |
| +0C | call_in_a | 1 | CALL | debugger session | none |
| +0D | call_in_b | 1 | CALL | debugger session | none |
| +0E..0F | call_in_x | 2 | CALL | debugger session | none |
| +10..11 | call_in_y | 2 | CALL | debugger session | none |
| +12 | call_out_a | 1 | CALL | until next successful CALL/input edit | none |
| +13 | call_out_b | 1 | CALL | until next successful CALL/input edit | none |
| +14..15 | call_out_x | 2 | CALL | until next successful CALL/input edit | none |
| +16..17 | call_out_y | 2 | CALL | until next successful CALL/input edit | none |
| +18 | call_out_ccr | 1 | CALL | until next successful CALL/input edit | none |
| +19 | env_saved_map | 1 | MEM/EDIT/CALL binding glue | one mapping transaction | shared, non-nested |
| +1A | tmp0 | 1 | all core modules | one primitive | fully shared |
| +1B | tmp1 | 1 | all core modules | one primitive | fully shared |
| +1C..1D | tmp_word | 2 | all core modules | one primitive | fully shared |

**Total fixed DebugTool workspace: 30 bytes.**

No final physical RAM address has been selected.

## 11. RAM-budget result

Architecture target: <=50 bytes.

Frozen Increment-2 workspace: **30 bytes**.

Margin to target: **20 bytes**.

The 7-byte active DynFS context is external state owned by DynFS and is not
double-counted as DebugTool RAM.

No 40-byte display buffer, text-entry buffer, CALL argument array, executable
RAM trampoline, or INFO snapshot is allocated.

## 12. Increment 2 exit check

Required by `WORKPLAN.md`:

- current bank representation: **FROZEN**
- current 16-bit address: **FROZEN**
- cursor/edit/input mode: **FROZEN**
- CALL input registers: **FROZEN**
- CALL captured return registers: **FROZEN**
- INFO state references: **FROZEN**
- shared temporary/input bytes: **FROZEN**
- first byte-counted RAM ledger: **30 B / PASS**
- CALL convention unambiguous: **PASS**

**Increment 2 exit criterion: PASS.**

Next and only authorized increment:

**Increment 3 — binding interface skeleton.**
