# A3C4 / index 241 — mechanically reconstructed, return contract unresolved

A3C4 has been reconstructed to the maximum level justified by the current primary corpus, but **no callable C prototype is published** because the return contract is not independently recoverable.

## Platform availability

- AlphaSmart 3000 (2005): raw index-241 value `0x004E004E` is not a valid handler entry. Mapping it into the canonical ROM lands inside the extension word of an existing `PEA 7(SP)` instruction, not at an instruction boundary.
- NEO (2005): raw index-241 value `0x004E005E` lies below the mapped ROM runtime base and is not a demonstrated code pointer.
- NEO (2013): real handler at runtime `0x0043DAE2`, file offset `0x02DAE2`.

The old-generation values remain neutral `NON_POINTER_OR_UNRESOLVED`; no vendor meaning is assigned.

## NEO 2013 mechanics

The NEO 2013 handler is exactly `0x1A` bytes long, ending immediately before A3C8. SHA-256:

`7619bfc511b2f496744dc3cc0db4f3b7a27095e604f30824fc5d5d006f6b29d9`

Mechanically it receives one 32-bit pointer-like value. It tests a shared byte state. When that state is non-zero it returns without mutation. When it is zero, the handler stores the argument longword in another global, writes literal byte `5` to the shared state, and returns.

No helper is called and **D0 is not defined by the handler on any path**.

## Caller evidence

The official corpus was checked 41/41. Seven executable A3C4 calls were found in seven extended-table applets. Every caller constructs one functional 32-bit slot from a pointer held in A0. Every caller overwrites D0 before any use of the value left by A3C4.

Direct NEO 2013 firmware JSR/JMP/BSR.W references to A3C4 are negative.

Thus the physical input shape and side effects are well supported, but the return intent is not.

## Why no `void` prototype is published

`void` is a **very strong inference** because the handler never writes D0 and all seven known callers ignore it. That is still absence-of-use evidence rather than an independent declaration of vendor intent. Following the same conservative rule used for other OS3K routines with residual D0 and no independent prototype, the return contract remains **UNKNOWN**.

Publishing a callable `void SYS_A3C4(void *)` would therefore overstate the evidence.

## Regression

Reproducible static regression validates canonical ROM hashes, the old-generation invalid/non-pointer entries, the NEO 2013 handler bytes/hash, the seven executable official callers, the single functional input slot, and zero return consumption. Result: **OVERALL PASS**.

Dynamic/emulator regression is specified but not executed. Full firmware bytes, caller contexts, and detailed workpapers remain private in Drive.
