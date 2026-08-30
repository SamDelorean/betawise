# A3C8 / index 242 — mechanically reconstructed, callable contract unresolved

A3C8 is reconstructed to the maximum level supported by the present primary corpus, but no callable C prototype is published because both the return intent and semantic type of the single input slot remain unconfirmed.

## Platform availability

- AlphaSmart 3000 (2005): raw slot `0x5A0E004E`, not a demonstrated code pointer.
- NEO (2005): raw slot `0x7F10005E`, not a demonstrated code pointer.
- NEO (2013): real handler at runtime `0x0043DAFC`, file offset `0x02DAFC`.

No semantic label is assigned to the older-generation values.

## NEO 2013 handler

The handler is exactly `0x20` bytes long and has SHA-256:

`3b6f4e37605adf639cd0cd953ddd369ff9715f084343030cdc1d85566933fe4d`

It reads one 32-bit stack slot. It tests a shared byte state. If the state is non-zero it returns immediately. If the state is zero, it stores the slot verbatim into a global longword, writes state value `7`, calls a private helper, and returns.

The slot is therefore mechanically a full 32-bit value, but no caller exists to justify publishing it as a pointer, integer, handle, or other semantic C type.

## Private helper

The helper called only by A3C8 is mechanically delimited to `0x76` bytes, with SHA-256:

`fd2f78590e0ce14ea54354aaa6b7cb11ce5da979c169b06199c3820dfcfd062e`

Reproducible 68000 decoding yields 23 instructions, internal non-BSR branch targets on valid instruction boundaries, a single terminal RTS at `+0x74`, and an independent function prologue immediately afterward. It performs a sequence of global-state tests/setup calls and converges before a final helper call.

A3C8 therefore does not define a uniform D0 value across its own paths: the state-nonzero path returns with incoming D0 untouched, while the state-zero path leaves whatever D0 results from the private helper chain.

## Caller evidence

- Direct NEO 2013 firmware JSR/JMP/BSR.W references to A3C8: 0.
- Official SmartApplet corpus: 41/41 checked. Sixteen extended applets physically contain the A3C8 slot, but corrected PC-index analysis finds 0 executable A3C8 callers. Fourteen legacy table-bearing applets end before this extension and eleven applets are structural negatives.

Consequently there is no positive caller evidence for a return width or semantic input type.

## Adversarial disposition

- Rejected: inheriting a `void` return solely from neighboring A3C4.
- Rejected: promoting the slot to `void *` merely because A3C4 has pointer-like callers.
- Rejected: interpreting the older-generation slot values as callable pointers.
- Rejected: publishing the mechanically residual/helper-derived D0 as a return value.
- Repository/history search did not recover an independent A3C8 symbol or prototype.

Status: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO** for `RETORNO_CONTRACTUAL=DESCONOCIDO` and semantic typing of the single 32-bit slot. The physical ABI and side effects are documented privately, but no callable prototype is asserted.

Static regression result: **OVERALL PASS**. Dynamic/emulator regression is specified but not executed. Detailed firmware, helper, and workpaper evidence remains private in Drive.
