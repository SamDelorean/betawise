# A3CC / index 243 — mechanically reconstructed, return contract unresolved

A3CC has been reconstructed to the maximum level justified by the current primary corpus, but **no callable C prototype is published** because the return intent cannot be independently demonstrated.

## Platform availability

- AlphaSmart 3000 (2005): raw index-243 slot `0x5A0E004E`, not a demonstrated code pointer.
- NEO (2005): raw index-243 slot `0x7FC0005E`, not a demonstrated code pointer.
- NEO (2013): real handler at runtime `0x0043EFE8`, file offset `0x02EFE8`.

No semantic meaning is assigned to the old-generation slot values.

## NEO 2013 mechanics

The NEO 2013 handler is exactly `0xD6` bytes long, contains 72 reproducibly decoded 68000 instructions, and terminates at the first function-local `RTS` at `+0xD4`. The code immediately following begins a distinct function. SHA-256 of the exact A3CC handler bytes is:

`460e37673239f9d7fcc4feaaf0a606350f724c411bb0447e5c8076e11ef3ca4b`

A3CC consumes no caller stack arguments. Its direct internal dependencies include A3B8 twice and A3B4 twice, plus private helpers. The routine maintains internal byte/word status values and deliberately constructs `D0.B` as either zero or one before the terminal epilogue.

Thus a byte-sized result exists **mechanically**, but that alone does not prove a callable return contract.

## Caller evidence

NEO 2013 firmware contains exactly three direct absolute JSRs to A3CC and no direct BSR.W calls. At all three call sites the value left in D0 is ignored before any demonstrated use.

The official SmartApplet corpus was checked structurally 41/41. The extended A-line tables demonstrated in the preceding generational extension terminate physically at A3BC; A3CC is not exposed as an A-line slot in the official corpus. Official SmartApplet caller count is therefore 0/41 by structural absence.

No independent vendor symbol or prototype was recovered from the repository/history evidence searched during this pass.

## Why no `uint8_t` prototype is published

`uint8_t SYS_A3CC(void)` is a **strong inference** because the handler has zero arguments and explicitly creates a 0/1 byte value in D0. However, none of the three firmware callers consumes that value, and there is no independent prototype or SmartApplet caller to establish vendor-level return intent.

Following the same conservative rule used for other OS3K handlers where mechanical D0 state is not independently demonstrated as contractual, A3CC remains blocked specifically on `RETORNO_CONTRACTUAL=DESCONOCIDO` rather than receiving an asserted callable signature.

## Regression and evidence disposition

Reproducible static regression revalidates the three canonical ROM hashes, generation-specific slot values, the NEO 2013 vector/file mapping, handler length/hash and terminal boundary, direct A3B8/A3B4 dependencies, the three firmware JSRs, zero caller arguments, mechanical D0.B domain `{0,1}`, and zero return consumption by the known callers. Result: **OVERALL PASS**.

Dynamic/emulator regression is specified but has not been executed. Full firmware bytes, detailed disassembly and private workpapers remain in Drive.

Status: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO** exclusively for contractual return intent. No callable prototype is asserted.
