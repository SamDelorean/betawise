# A3F4 — neutral blocked disposition

Status: **mechanically reconstructed; callable contract blocked only on return intent/type**.

## Generational availability

- AlphaSmart 3000 (2005): table value `0x5EC6004E`, non-pointer/non-callable.
- NEO (2005): table value `0x83D2005E`, non-pointer/non-callable.
- NEO (2013): runtime `0x00433FE2`, file offset `0x023FE2`.

The NEO 2013 handler is exactly `0x66` bytes, 35 decoded 68000 instructions, with one terminal RTS. Handler SHA-256: `0f3e2c9544939309eca0dcb9bb981561956df36a1b1e47aa2e12802c1b8e17c3`.

## Physical ABI

A3F4 has five physical 32-bit stack slots with effective use:

`[arg1_pointer, low8(arg2), low8(arg3), low8(arg4), low8(arg5)]`

The repeated `+0x1B(SP)` byte loads do not refer to one repeated external argument: the stack pointer changes after each push, so they resolve successively to arg5, arg4, arg3 and arg2 in the original entry frame.

The handler derives records from `arg1+4` using stride `0x12C`, selects a secondary subrecord from offset `+0x34` using stride `0x1A` and arg5, and invokes private helper `0x004337B0` with four derived values. Internal sign/zero extension is described mechanically only and does not establish public signedness.

## Return ambiguity

No direct JSR, JMP, BSR.W or BSR.B xref to A3F4 was found in NEO 2013, and the official SmartApplet corpus does not physically expose this late slot.

A3F4 returns without redefining D0 after private helper `0x004337B0`. That helper has a null path leaving D0 as zero but a non-null path that reloads only a low byte after deeper calls and does not normalize the upper 24 bits before RTS. This is insufficient to support a coherent scalar return contract.

`void` remains a strong intent inference, not a confirmed contract. No concrete scalar type is published.

## Validation

A preliminary regression run failed one assertion because the expected offsets of the repeated `+0x1B(SP)` loads were transcribed incorrectly. After byte/disassembly review, the corrected test passes **21/21 assertions, OVERALL PASS**. The failed preliminary run is not used for closure. Dynamic/emulator-first validation was not executed.

Disposition: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`**. No callable header is published.
