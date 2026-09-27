# DebugTool-AS2000 — Increment 7 CALL result

Status: **INCREMENT 7 CLOSED / PASS**

Increment 7 implemented and validated only CALL on top of MEM, GOTO and EDIT.
No INFO behavior was added.

## Native HC11 CALL contract

Input state:

- target: 16-bit executable CPU address
- A: 8-bit
- B: 8-bit
- D: derived as A:B
- X: 16-bit
- Y: 16-bit

CCR is not an input field.

Captured return state:

- A
- B
- X
- Y
- CCR

SP is not user-editable and the target must return with SP balanced.

## Transfer mechanism

The engine uses the frozen stack-built native transfer:

1. save the current environment mapping byte;
2. push the DebugTool capture-return address;
3. push the target address;
4. load A/B/X/Y from workspace;
5. execute RTS.

The first RTS transfers control to the target. At target entry the top of stack
contains one ordinary 16-bit return address, equivalent to a normal JSR call
shape.

On target RTS, control returns to the capture stub, which:

1. saves returned A;
2. copies returned CCR through TPA;
3. stores CCR;
4. restores returned A;
5. stores A/B/X/Y;
6. restores the saved environment mapping;
7. sets RESULT_VALID.

No executable RAM trampoline, argument array or 68k compatibility layer exists.

## User interface

CALL uses a fixed sequential entry order:

- T = target, 4 hex digits
- A = A, 2 hex digits
- B = B, 2 hex digits
- X = X, 4 hex digits
- Y = Y, 4 hex digits

Escape cancels.

The result display is intentionally compact:

- row 0: `AA BB CC` = A, B, CCR
- row 1: `XXXX YYYY` = X, Y

After one key, the tool returns to MEM.

## Deterministic target tests

A compiled HC11 self-test exercised the same `DBG_CALL_EXECUTE` engine under
`as2kdiag`.

### 1. Synthetic target

Input:

- A=12
- B=34
- X=5678
- Y=9ABC

The target first verified all four incoming registers, then returned:

- A=A1
- B=B2
- X=C3D4
- Y=E5F6

All returned values were captured correctly and RESULT_VALID was set.

### 2. Verified stock ROM target

Target:

`$9350` — stock byte-to-hex helper.

Input:

- A=AF
- B=99
- X=1234
- Y=5678

Observed:

- A=41 ('A')
- B=46 ('F')
- X=1234
- Y=5678

This matches the independently disassembled stock routine.

### 3. DynFS primitive

The actual placement-free DynFS source
`src/dynfs/profile1/phase1/dynfs_p1_block_map.asm` was assembled and linked
into the temporary diagnostic image. No resident production address was
invented.

Target:

`FS_BLOCK_MAP`

Input block id:

`45h`

Expected and observed:

- B=02
- X=1400
- Y preserved as 2468

Self-test display result:

`CALL SELFTEST PASS`

## Physical-keyboard end-to-end test

A second MAME run entered through `DBG_CALL_RUN` and used the real stock
keyboard scan/dequeue/translate path.

Typed compact field sequence:

`9350af0012345678`

This corresponds to:

- target 9350
- A=AF
- B=00
- X=1234
- Y=5678

Observed result screen:

`41 46 60`

`1234 5678`

Therefore the complete user path passed:

`physical keyboard -> stock translation -> CALL parser -> stack/RTS target -> capture -> result rendering`

The displayed CCR for this stock invocation was `60h`.

## Environment mapping note

CALL snapshots and restores the complete environment mapping byte through the
binding layer.

Because the target receives no hidden stack frame, it must return in a mapping
state from which the DebugTool workspace remains addressable long enough for
the capture stub to store results and perform the saved-map restore.

CALL is not a fault sandbox; arbitrary routines that destroy SP or the memory
map can still crash the debugger.

## Measured size

Final checkpoint:

| Component | Increment 6 | Increment 7 | Delta |
|---|---:|---:|---:|
| portable core .text | 675 B | 1039 B | +364 B |
| diagnostic binding .text | 94 B | 102 B | +8 B |
| fixed DebugTool workspace | 30 B | 30 B | 0 B |
| product .data | 0 B | 0 B | 0 B |

Current executable total:

**1039 B core + 102 B binding = 1141 B**

This is inside the project's normal ACCEPTABLE range of 1025-1536 bytes.

CALL budgeting is split by the existing budget categories:

- CALL state/parser + native execute/capture engine:
  `$02A3-$0379` = **215 B**, inside the 260-byte CALL hard planning ceiling.
- the remaining CALL-specific interaction/result wrapper is **149 B** and is
  accounted under the separate UI/dispatcher/string/output budget categories.

No fixed RAM was added.

## Scope control

Not added:

- stack arguments
- six generic arguments
- editable SP
- editable input CCR
- A-line/syscall compatibility
- breakpoint/single-step handling
- fault trapping
- disassembler
- extra DynFS private ABI

No Codex was used.

## Exit check

Required by `WORKPLAN.md` Increment 7:

- target address load: **PASS**
- A/B(D)/X/Y pre-call state: **PASS**
- native stack/RTS transfer: **PASS**
- A/B/X/Y capture: **PASS**
- CCR capture: **PASS**
- safe return to DebugTool: **PASS**
- verified stock-ROM target: **PASS**
- DynFS primitive target: **PASS**
- physical-keyboard CALL path: **PASS**
- exact ROM/RAM delta recorded: **PASS**
- no generic six-argument ABI: **PASS**
- no INFO scope growth: **PASS**

**Increment 7 exit criterion: PASS.**

Next and only authorized increment:

**Increment 8 — INFO only.**
