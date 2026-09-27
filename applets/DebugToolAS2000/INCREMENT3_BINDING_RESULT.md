# DebugTool-AS2000 — Increment 3 binding skeleton result

Status: **INCREMENT 3 CLOSED / PASS**

This increment establishes the environment-binding boundary only. It does not
implement MEM, EDIT, GOTO, CALL or INFO.

## Artifacts

Portable:

- `include/debugtool_state.inc`
- `include/debugtool_bindings.inc`
- `core/debugtool_core.asm`

Environment-specific:

- `bindings/diagnostic_mame.asm`
- `bindings/current_rom.template.asm`
- `bindings/future_rom.template.asm`

Verification:

- `tests/binding_smoke.asm`
- `tools/build_skeleton.sh`

## Binding contract

Portable code may refer only to the symbols declared by
`debugtool_bindings.inc`.

The interface currently covers:

- DebugTool workspace base
- stock LCD clear/string/byte/command/row helpers
- stock byte-to-hex helper
- stock keyboard queue consumer
- RAM enter/restore adapter
- stock/DynFS compatibility state references
- active 7-byte DynFS compatibility context

No current/future ROM address is embedded in `debugtool_core.asm`.

## Diagnostic binding

The diagnostic/MAME skeleton binds verified AS2000 v3.1.4 stock services:

- LCD clear: `$A3D6`
- LCD string: `$A435`
- LCD byte: `$A44B`
- LCD command: `$A440`
- candidate LCD row helpers: `$A54B/$A556/$A561/$A56C`
- byte-to-hex: `$9350`
- keyboard dequeue: `$938C`
- stock compatibility state: `$018E/$0120/$0122/$0124/$0126/$0128/$0067`

RAM mapping and the active DynFS context remain explicit link stubs/synthetic
storage in Increment 3. They are **not** runtime-complete bindings.

## First deterministic build

GNU m68hc11 assembler/linker on the T640 produced:

| Object/section | Size |
|---|---:|
| portable core .text | 1 B |
| diagnostic binding .text stubs | 2 B |
| diagnostic binding synthetic .bss | 37 B |
| binding smoke .bindcheck | 42 B |

The 37-byte synthetic BSS consists of:

- 30 B frozen DebugTool workspace
- 7 B synthetic active DynFS context solely to satisfy the link

Only the 30-byte workspace belongs to the DebugTool RAM budget.

Result:

`SKELETON_BINDING_PASS`

The synthetic link map used only for this test was:

- `.text = $A000`
- `.bindcheck = $6000`
- `.bss = $7000`

These are not resident placement decisions.

## Rebinding/relocation proof

The exact same portable core source/object was then linked against a completely
different temporary binding and a different synthetic linker map:

- core entry moved from the first synthetic text area to `$B800`
- workspace moved to `$6800`
- DynFS context resolved at `$681E`
- representative service aliases were replaced with different values

The two independently assembled portable `core.o` files had identical SHA-256:

`9524510886f57236cf203107eb67ea48665a0de4f6088bcafcec6a8308d55301`

Result:

`REBIND_PASS`

Therefore changing environment bindings/placement does not require editing the
portable core.

## Absolute-address guard

The build script now fails if a 16-bit absolute hexadecimal address appears in:

- `core/debugtool_core.asm`
- `include/debugtool_state.inc`
- `include/debugtool_bindings.inc`

Current result:

`CORE_INTERFACE_HAS_NO_16BIT_ABSOLUTE_ADDRESSES`

Absolute addresses are allowed only inside environment binding/linker material.

## ROM/RAM accounting for Increment 3

Product functionality added: none.

Portable skeleton code:

- core .text: **1 B**

Diagnostic-only link stubs:

- **2 B**, not a final resident cost

Fixed DebugTool RAM contract:

- **30 B**, unchanged from Increment 2

Physically assigned resident RAM:

- **0 B**

Final resident ROM placement:

- **unassigned**

## Exit check

Required by `WORKPLAN.md` Increment 3:

- core symbolic interface/include: **PASS**
- diagnostic/MAME binding: **PASS, skeleton-level**
- current-ROM placeholder: **PASS**
- future-ROM placeholder: **PASS**
- known/stub bindings assemble and link: **PASS**
- unresolved final addresses remain symbolic/environment-owned: **PASS**
- core relocates/rebinds without source changes: **PASS**

**Increment 3 exit criterion: PASS.**

Next and only authorized increment:

**Increment 4 — MEM only.**
