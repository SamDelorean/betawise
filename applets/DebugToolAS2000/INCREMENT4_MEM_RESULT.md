# DebugTool-AS2000 — Increment 4 MEM result

Status: **INCREMENT 4 CLOSED / PASS**

Increment 4 implemented and validated only the MEM viewer. No GOTO, EDIT, CALL
or INFO behavior was added.

## Implemented behavior

Portable core now provides:

- deterministic MEM session initialization;
- physical-byte read at a 16-bit CPU address;
- fixed-ROM read for addresses with bit 15 set;
- banked-RAM read for lower-half addresses through the binding adapter;
- 4 x 40 character screen redraw;
- stock-ROM byte-to-hex conversion reuse;
- printable ASCII rendering, with non-printable bytes shown as '.';
- natural 16-bit address progression across the RAM/ROM boundary.

## Frozen MEM layout

Each row is exactly 40 characters:

`Bn:AAAA XX XX XX XX XX XX XX XX abcdefgh`

or, for fixed ROM:

`RO:AAAA XX XX XX XX XX XX XX XX abcdefgh`

Accounting:

- 8-character header;
- 8 hexadecimal bytes at 3 characters each = 24;
- 8 ASCII characters;
- total = 40 characters.

A screen therefore displays 32 bytes without a line or screen buffer.

## RAM mapping adapter

The diagnostic binding now implements a real immediate/non-nestable
ENTER/RESTORE pair.

ENTER:

- receives bank 0..3 in A;
- preserves X;
- saves incoming CCR and PORTA in a private two-byte frame on the internal CPU
  stack;
- masks interrupts;
- selects PA6=1 and requested PA5:PA4.

RESTORE:

- restores PORTA first;
- restores the exact saved CCR;
- removes the private stack frame;
- preserves B and X.

This avoids requiring the DebugTool workspace to remain visible while a
different external RAM bank is selected.

The frozen `env_saved_map` byte remains reserved by the v0 ABI but is unused
by this binding.

## Measured code/RAM

GNU m68hc11 assembler/linker result:

| Component | Increment 3 | Increment 4 | Delta |
|---|---:|---:|---:|
| portable core .text | 1 B | 248 B | +247 B |
| diagnostic binding .text | 2 B | 38 B | +36 B |
| fixed DebugTool workspace | 30 B | 30 B | 0 B |
| product .data | 0 B | 0 B | 0 B |

The diagnostic binding also carries the existing synthetic 7-byte DynFS
context solely for skeleton linking. It is not DebugTool-owned RAM.

Increment-4 executable total represented by core + diagnostic mapping glue:
**286 B**.

No optimization pass is opened at this stage.

## MAME functional probe

Emulator:

- existing T640 `as2kdiag` diagnostic build;
- stock AS2000 v3.1.4 firmware as baseline;
- disposable derived ROM only;
- private stock ROM remained unchanged.

Temporary diagnostic placement:

- test wrapper + DebugTool image at W1 starting `$D099`;
- temporary idle hook changed stock `$87D6-$87D8`
  `0E CF 01` (CLI/STOP/NOP) to `BD D0 99` (JSR test wrapper);
- synthetic workspace at bank-0 `$7000`;
- no post-ROM trailer byte changed.

This placement/hook is test infrastructure only and is not a current-ROM
integration decision.

### Test vector

The wrapper:

1. selected physical RAM bank 1;
2. wrote `41 42 43 44 45 46 47 48` at `$7FF8-$7FFF`;
3. restored bank 0, so the DebugTool workspace was visible;
4. set MEM state to bank 1, address `$7FF8`;
5. invoked `DBG_MEM_REDRAW`.

Expected first row:

`B1:7FF8 41 42 43 44 45 46 47 48 ABCDEFGH`

Expected next row to cross to fixed ROM at `$8000`.

### Observed screen

MAME snapshot showed:

`B1:7FF8 41 42 43 44 45 46 47 48 ABCDEFGH`

followed by fixed-ROM rows beginning:

`RO:8000 8E 00 FF 15 00 40 86 FD ...`

`RO:8008 B7 30 10 B7 30 10 86 04 ...`

`RO:8010 B7 40 00 14 00 40 BD 87 ...`

The first 24 bytes independently read from the stock ROM are exactly:

`8E 00 FF 15 00 40 86 FD B7 30 10 B7 30 10 86 04 B7 40 00 14 00 40 BD 87`

Therefore the display matches the source bytes.

## What this proves

- Known ROM bytes: **PASS**
- Known RAM bytes: **PASS**
- Bank switch: **PASS**
- RAM->ROM boundary at `$7FFF->$8000`: **PASS**
- LCD row order: **PASS**

The wrapper restored physical bank 0 before invoking the viewer. The viewer
still retrieved the bank-1 pattern, and correct continuation of the redraw
requires restoring the bank-0 workspace between byte reads.

The four physical rows appear in top-to-bottom order through
`$A54B/$A556/$A561/$A56C`, agreeing with stock `$9FEF/$A0FD` traversal.

## Patch hygiene

The temporary derived test ROM changed 340 bytes total between the hook and W1
test image. No byte at or beyond file offset `$8000` changed, so the ZPSD
trailer was preserved.

The temporary derived ROM and screenshot are not repository artifacts.

## Exit check

Required by `WORKPLAN.md` Increment 4:

- bank/address MEM state: **PASS**
- physical memory read: **PASS**
- fixed 40x4 compact dump: **PASS**
- minimal hex/ASCII rendering: **PASS**
- known ROM-byte test: **PASS**
- known RAM-byte test: **PASS**
- bank-switch observation: **PASS**
- display boundary behavior: **PASS**
- exact ROM/RAM delta recorded: **PASS**
- no EDIT/GOTO/CALL/INFO implementation added: **PASS**

**Increment 4 exit criterion: PASS.**

Next and only authorized increment:

**Increment 5 — GOTO only.**
