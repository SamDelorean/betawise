# DebugTool-AS2000 — Increment 9 v0 consolidation

Status: **INCREMENT 9 CLOSED / PASS**

No new user-visible function was added. The five frozen v0 functions remain:

- MEM
- GOTO
- EDIT
- CALL
- INFO

## 1. Clean build and exact size

Final consolidated product checkpoint:

| Artifact | Size |
|---|---:|
| portable core .text | 1175 B |
| diagnostic binding .text | 102 B |
| executable v0 .text | **1277 B** |
| fixed DebugTool workspace | **30 B** |
| external synthetic DynFS context in diagnostic link | 7 B |

There is no product `.data` allocation.

Acceptance gate:

- 1277 B -> **ACCEPTABLE** (1025-1536 B)
- no bounded size-reduction pass is required by the work plan.

## 2. Duplicate-helper audit

The stock ROM was searched through all 13 direct JSR xrefs to `$9350`.

One exact local duplication was found:

### stock `$A3A0`

Behavior:

1. JSR `$9350`
2. PSHB
3. TAB
4. JSR `$A44B`
5. PULB
6. JMP `$A44B`

This is exactly the former local `DBG_OUT_HEX_A` behavior: input A=byte,
emit both uppercase hexadecimal digits.

Action:

- local helper removed;
- portable core now calls symbolic `DBG_BIND_OUT_HEX`;
- diagnostic binding maps it to stock `$A3A0`.

Measured saving:

- core 1187 B -> 1175 B
- **12 B removed**

No other verified stock routine was found that safely replaces the remaining
small local helpers under their frozen contracts.

In particular:

- A->LCD adaptation remains a tiny local helper because stock `$A44B`
  consumes B;
- ASCII printable substitution is DebugTool-specific;
- ASCII hex-character -> nibble parsing has no verified stock equivalent in
  the audited dependency set;
- bank mapping adapters retain behavior not supplied by stock `$9466`
  alone, especially PA6/view preservation and restoration.

## 3. Dead-code/interface audit

The final core undefined-symbol set contains only symbols actually consumed by
the v0 core.

Unused portable contract declarations were removed for:

- LCD command helper
- raw keyboard dequeue
- raw-key character translator
- direct byte-to-hex converter
- stock capacity
- stock origin
- stock sequential position

The keyboard binding may still use its raw internal stock helpers internally;
they are no longer part of the portable core contract.

The unused stock state aliases that remain in the environment source cost zero
executable bytes and remain documented as possible later binding information.

No removable executable helper or string remained after the call/reference
audit.

## 4. Combined v0 regression

Committed harness:

`tests/v0_regression.asm`

One diagnostic image exercised all five functions in sequence.

### MEM

- seeded physical bank 1 at `$1234` with `V0REGRES`;
- `DBG_MEM_READ_X` returned the expected first byte;
- full MEM redraw executed successfully.

### GOTO

- parsed and committed `1:1234`;
- resulting state was bank 1 / address `$1234`.

### EDIT

- selected cursor +7;
- first nibble `2` did not write;
- second nibble `1` committed `21h` ('!');
- independent physical bank-1 read confirmed `21h`.

### CALL

Synthetic target verified incoming:

- A=12
- B=34
- X=5678
- Y=9ABC

and returned:

- A=A1
- B=B2
- X=C3D4
- Y=E5F6

All returned fields and RESULT_VALID passed.

### INFO

Final injected state:

- slot 03
- file 07
- MEM bank 01
- payload 0000
- compat_base 3000
- cursor 3012
- view 3004
- end 3080

MAME independently observed:

`V0_OBS edited=21 slot=03 file=07 bank=01 payload=0000 base=3000 dC=0012 dV=0004 dE=0080`

Final screen:

`S03 F07 B01`

`P0000 K3000`

`C0012 V0004 E0080`

`V0 PASS`

Therefore the regression exercised and passed MEM/GOTO/EDIT/CALL/INFO in one
booted image.

The disposable test ROM is not a repository artifact.

## 5. Reproducible v0 build

Committed build tool:

`tools/build_v0.sh`

It generates, with synthetic diagnostic placement only:

- `core.o`
- `binding.o`
- `v0.elf`
- `v0.bin`
- `symbols.txt`
- `SHA256SUMS`

The build was run independently twice. Byte comparisons passed for all four
binary artifacts:

- `CORE_IDENTICAL`
- `BINDING_IDENTICAL`
- `ELF_IDENTICAL`
- `BIN_IDENTICAL`

Frozen hashes for this consolidation checkpoint:

- core.o:
  `aa31237fa3664c2f9c19a9873ccb41ce2fe54cf89e4459dca982d37ba4cb2be2`
- binding.o:
  `e726cf9db13083bd99fd8b04596aeadc81bfe13e21638c3ab83aa57880885712`
- v0.elf:
  `58b22a4191bd72c48ac71de29b07e046b68dbaf2b843aa72f2c10878193342b2`
- v0.bin:
  `6ab2f66b9fbddb19395ba1cff44a76a352571862071f7886186d3d7efd66a699`

The synthetic link map used here is only:

- .text = `$A000`
- .bss = `$7000`

Neither is a current-ROM placement decision.

## 6. Guard correction

During consolidation, the absolute-address source guard itself was audited.

The previous skeleton regex was over-escaped and could miss `$xxxx`
addresses. Both build scripts now:

1. strip assembly comments;
2. reject 16-bit absolute hexadecimal addresses in portable core/interface
   source;
3. permit documented addresses in comments.

The corrected skeleton build passes.

## 7. RAM ledger

The v0 fixed workspace remains exactly **30 bytes**.

No new persistent RAM was introduced by consolidation.

Final physical RAM assignment remains deliberately unresolved until Increment
10, where free-RAM ranges must be proven against stock and DynFS users.

## Exit check

Required by `WORKPLAN.md` Increment 9:

- clean assemble/link: **PASS**
- exact size audit: **PASS — 1277 B executable / 30 B workspace**
- duplicate-helper audit: **PASS**
- stock duplicate removed: **PASS — 12 B saved**
- MEM regression: **PASS**
- GOTO regression: **PASS**
- EDIT regression: **PASS**
- CALL regression: **PASS**
- INFO regression: **PASS**
- dead-code/interface audit: **PASS**
- reproducible v0 core/binary: **PASS**
- no new features: **PASS**
- size <=1536 B: **PASS**

**Increment 9 exit criterion: PASS.**

Next and only authorized increment:

**Increment 10 — current-ROM integration.**
