# A3C4 / index 241 — source-first mechanics reconstructed, return contract unresolved

A3C4 has been reconstructed to the maximum level justified by the current primary corpus and independently revalidated under the source-first methodology. **No callable C prototype is published** because the contractual return type is not recoverable from the evidence currently available.

## Source-first correlation

Before re-reading firmware, the available named sources were checked for an independent contract. BetaWise and `ioma8/neo-re` both preserve only the neutral slot name `SYS_A3C4` at index 241. The checked `os3k.h` declarations provide no A3C4 prototype, and the checked DebugTool source provides neither a symbol nor a semantic contract. No independent vendor-style name was recovered.

This negative correlation is important: it prevents the neutral table label from being mistaken for a recovered vendor API.

## Platform availability

Fresh revalidation first reproduced the SHA-256 values of all three canonical ROMs.

- AlphaSmart 3000 (2005): raw index-241 value `0x004E004E` is not a handler entry; mapping it into the canonical ROM lands inside an instruction extension word.
- NEO (2005): raw index-241 value `0x004E005E` lies below the mapped ROM runtime base and is not a demonstrated code pointer.
- NEO (2013): a real 26-byte handler exists. Its previously archived handler hash was reproduced exactly.

The old-generation values remain `NON_POINTER_OR_UNRESOLVED`; no contract is inherited from NEO 2013.

## NEO 2013 mechanics

The handler receives one 32-bit pointer-like value. It tests a shared byte state. If that state is already non-zero it returns without mutation. If the state is zero, it stores the argument longword in another private global, writes literal byte `5` to the state, and returns. No helper is called.

**D0 is not defined by the handler on either path.**

The surrounding NEO-only routines access the same state machine, but the semantic name of the state, pointer, and operation remains unknown; no wireless/registration/callback name is promoted without an independent anchor.

## Caller evidence

The official corpus contains seven executable A3C4 callers in seven extended-table applets. A fresh directed pass rechecked those call sites: every caller contributes one functional 32-bit slot derived from A0, and every caller overwrites D0 before any use of the residual value.

Fresh direct searches of the NEO 2013 firmware again found zero absolute JSR/JMP and zero BSR.W references to the handler.

Therefore the input shape and side effects are **CONFIRMADO**, while the contractual return remains **DESCONOCIDO**.

## Why no `void` prototype is published

`void` is an **INFERENCIA MUY FUERTE** because the implementation never establishes a return value and every recovered caller ignores it. That still does not prove the source-level/vendor declaration. Publishing `void SYS_A3C4(void *)` as confirmed would convert absence-of-consumption evidence into an invented contract.

A return-bearing contract is likewise unsupported because D0 is merely residual machine state. The correct source-first disposition is to leave the return type unresolved.

## Regression

The archived integral static regression remains **EJECUTADA / OVERALL PASS**. A fresh directed source-first pass over canonical ROM hashes, slot validity, handler bounds/hash, direct xrefs, and the seven caller shapes also completed **EJECUTADA / PASS**.

Dynamic/emulator regression remains **ESPECIFICADA / NO EJECUTADA**. Firmware bytes, private global addresses, and detailed caller contexts remain private in Drive.
