# ABI trace — A400 / index256

Disposition: **MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / `RETORNO_CONTRACTUAL=DESCONOCIDO`**. The return uncertainty is an evidence boundary, not a human-blocking condition.

## Source-first correlation

Historical BetaWise `syscall.c` and `ioma8/neo-re` expose index 256 only as neutral placeholder `SYS_A400`. Neither provides a vendor symbol or semantic prototype, so no semantic name is synthesized.

## Primary mapping — freshly revalidated

- AS3000 2005: slot `0x5A06004E`, non-pointer/non-callable.
- NEO 2005: slot `0x7F08005E`, non-pointer/non-callable.
- NEO 2013: callable entry `0x004343A2`, file offset `0x0243A2`.
- A404/index257 points backward to `0x004341FE`; it therefore cannot delimit A400.
- Independently recovered A400 body: `0x2DA` bytes, sole RTS at `+0x2D8`, SHA-256 `407349319b3fbf0c0f5245126a20ed31a45649267c79e6a218655dbfb6a29e7d`.

## Physical ABI and mechanics

A400 consumes one physical 32-bit slot; only its low 16 bits are read. This establishes the mechanical form `SYS_A400(arg1_slot32)` with effective low-word consumption, but does not justify a public semantic type.

The handler operates on private state rooted at `0x0000F412`, `0x0000F416`, and `0x0000F41A`, including records with stride `0x12C`. Direct helper targets include `0x00442AF2`, `0x00442B54`, `0x004420C6`, `0x00441BAA`, and `0x00432A40`; semantic helper/global names are intentionally not assigned.

The low16 input is dispatched across ranges and exact codes. Numeric codes are preserved as mechanical constants and are not converted into key/event names without independent evidence.

## Callers and adversarial return check

A fresh aligned NEO 2013 sweep finds exactly one direct absolute JSR to A400, at `0x0043E314`, and zero direct JMP targets. The caller zero-extends its word value into a longword, pushes exactly one slot, invokes A400, cleans four bytes, then continues by testing its preexisting D7 rather than A400's D0.

Multiple A400 paths reach the common epilogue after side effects or helper calls without a uniform normalization of D0. A coherent scalar return contract therefore cannot be established. `void` remains an **INFERENCIA MUY FUERTE** from side-effect-heavy behavior and the positive caller ignoring D0, but is not CONFIRMADO; no callable header is emitted.

## Regression state

Historical static regression: **29/29 PASS**.

Fresh primary directed regression: **24/24 PASS**, covering all canonical ROM hashes/slots, backward A404 vector fact, independently recovered handler length/hash and sole RTS, low-word argument load, private-global/stride/helper anchors, exact direct-xref cardinality, caller push/cleanup shape, and common epilogue.

Dynamic/emulator-first regression: **ESPECIFICADA / NO EJECUTADA**.

Private ROM extraction, extensive disassembly and workpapers remain outside the repository.
