# DebugTool-AS2000 — Increment 5 GOTO result

Status: **INCREMENT 5 CLOSED / PASS**

Increment 5 implemented and validated only GOTO on top of the already-closed
MEM implementation. No EDIT, CALL or INFO behavior was added.

## Implemented syntax

Accepted forms:

- `AAAA` — keep the current RAM bank and replace the 16-bit CPU address.
- `b:AAAA` — select RAM bank `0..3` and replace the 16-bit CPU address.

Rules:

- exactly four address hex digits are required before Return commits;
- hex input is case-insensitive;
- a bank prefix is accepted only when exactly one leading digit `0..3`
  precedes `:`;
- an invalid bank prefix does not update `mem_bank`;
- Escape cancels without changing the committed bank/address;
- no text input buffer is allocated.

## Parser/state implementation

GOTO reuses the Increment-2 shared input fields:

- `input_digits`
- `input_bank`
- `input_value`
- `tmp0`

The parser accumulates:

`input_value = (input_value << 4) | nibble`

No new fixed RAM is required.

A shared compact ASCII-hex helper was added and is intended for reuse by later
EDIT/CALL increments.

## Keyboard path correction

Increment 5 established the complete stock keyboard path required by a resident
DebugTool input loop.

The relevant stock path is:

1. `$89B6` — keyboard scan/pump used by retained firmware loops;
2. `$938C` — dequeue one raw local keyboard code, V=1 if empty;
3. `$A33C` — translate the raw key code and return the translated character
   directly in A with V=0.

The return contract of `$A33C` is mechanically supported by stock caller
`$90B9`, which immediately executes `TAB` and then the stock LCD byte
routine. `$0070` remains the raw/dequeued key state and must not be mistaken
for the translated character.

The diagnostic binding now exposes one non-blocking
`DBG_BIND_KEY_GETCHAR` that performs this stock scan/dequeue/translate path.

## Measured size

Final GNU m68hc11 assembler/linker checkpoint:

| Component | Increment 4 | Increment 5 | Delta |
|---|---:|---:|---:|
| portable core .text | 248 B | 449 B | +201 B |
| diagnostic binding .text | 38 B | 54 B | +16 B |
| fixed DebugTool workspace | 30 B | 30 B | 0 B |
| product .data | 0 B | 0 B | 0 B |

The 201-byte core delta includes a **28-byte shared hexadecimal-to-nibble
helper**. The GOTO-specific core contribution is therefore **173 B**, below the
180-byte hard planning ceiling for the GOTO module.

The complete current executable checkpoint is:

**449 B core + 54 B binding = 503 B**

The synthetic 7-byte DynFS context remains external test/link state and is not
DebugTool-owned RAM.

## Deterministic parser matrix

A compiled HC11 self-test was run in `as2kdiag` against the actual GOTO parser.

Passed cases:

1. same-bank jump:
   - starting bank 2
   - input `0100`
   - result bank 2, address `$0100`

2. explicit bank/address:
   - input `1:1234`
   - result bank 1, address `$1234`

3. lower boundary:
   - input `0000`
   - result address `$0000`

4. upper boundary:
   - mixed-case input `fFfF`
   - result address `$FFFF`

5. invalid bank prefix:
   - input prefix `4:`
   - `input_bank` remained `FF`
   - no invalid bank value was committed

6. cancellation:
   - committed state preset to bank 2 / `$4567`
   - partial input followed by Escape
   - committed bank/address remained unchanged
   - UI mode returned to MEM

Observed result on the AlphaSmart display:

`GOTO SELFTEST PASS`

## Full physical-keyboard/MAME path

A second test exercised the real keyboard path rather than feeding translated
characters directly.

Temporary harness:

- seeded physical bank 1 at `$1234` with bytes
  `47 4F 54 4F 31 32 33 34` = `GOTO1234`;
- restored bank 0 for the synthetic workspace;
- entered `DBG_GOTO_RUN`;
- used ordinary MAME input-field transitions to type `1:1234` and Return.

The locally compiled `as2kdiag` input matrix was queried at runtime before the
final run because its masks differ from the adjacent source checkout. No MAME
source was modified.

Final observed MEM screen:

`B1:1234 47 4F 54 4F 31 32 33 34 GOTO1234`

followed by zero-filled continuation rows from `$123C`, `$1244` and
`$124C`.

This proves the complete runtime chain:

`physical key -> IRQ/scan -> $89B6 -> $938C -> $A33C -> GOTO parser -> MEM`

## Scope control

No code was added for:

- EDIT
- CALL
- INFO
- symbolic expressions
- decimal input
- previous-address history
- backspace editing
- arbitrary bank values
- additional navigation features

Only one bounded size-reduction pass was performed after the first GOTO build
exceeded the module planning ceiling.

## Exit check

Required by `WORKPLAN.md` Increment 5:

- same-bank jump: **PASS**
- banked jump: **PASS**
- boundary addresses: **PASS**
- cancel/return path: **PASS**
- validation of bank 0..3: **PASS**
- MEM redraw after GOTO: **PASS**
- real keyboard path: **PASS**
- exact ROM/RAM delta recorded: **PASS**
- no scope growth: **PASS**

**Increment 5 exit criterion: PASS.**

Next and only authorized increment:

**Increment 6 — EDIT only.**
