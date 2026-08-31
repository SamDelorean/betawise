# A3F0 — neutral blocked disposition

Status: **mechanically reconstructed; callable contract blocked only on return intent/type**.

## Generational availability

- AlphaSmart 3000 (2005): table value `0x5EA6004E`, non-pointer/non-callable.
- NEO (2005): table value `0x83B2005E`, non-pointer/non-callable.
- NEO (2013): runtime `0x00433F60`, file offset `0x023F60`.

The NEO 2013 handler is exactly `0x82` bytes, 45 decoded 68000 instructions, with one terminal RTS. Handler SHA-256: `2410257e36a576815e9ab14986f50cc1077cea2e5df2c1038a8f22e99628141a`.

## Physical ABI

A3F0 has four physical 32-bit stack slots with effective use:

`[arg1_pointer, low8(arg2), low8(arg3), low8(arg4)]`

The byte values are sign-extended internally for indexing/forwarding, but no external signedness is claimed.

A3F0 derives records from `arg1+4` with stride `0x12C`. It forwards four values to private helper `0x004337B0`: a derived pointer at offset `+0x11E`, the third and fourth byte arguments, and a selected base pointer which receives an additional `+0x1A` when the third byte lies in the mechanical range 5..7.

## Caller and return evidence

The NEO 2013 ROM contains exactly one direct absolute JSR to A3F0 at `0x0043E9FA`. That caller supplies four concrete slots, using either `(global_value, 0, 4, 1)` or `(global_value, 0, 0, 0)` depending its branch. It performs stack cleanup and immediately returns, mechanically propagating D0.

That caller is itself reached by eleven direct BSR.W sites. In the known sites there is no positive explicit consumer that establishes a return type.

The producer of D0 is also problematic as contractual evidence: private helper `0x004337B0` returns zero on a null route, while its non-null route reloads only the low byte of one argument after deeper calls and does not normalize the upper 24 bits before RTS. Therefore preservation of D0 through A3F0 and its immediate wrapper is insufficient to establish a coherent scalar return contract.

`void` is a strong inference of intent from the observed usage, but it is not promoted to fact. A concrete scalar return type is likewise not justified.

## Validation and disposition

Static structural/ABI regression: **23/23 assertions, OVERALL PASS**. Dynamic/emulator-first validation was not executed.

A3F0 remains **MECÁNICA_RECONSTRUIDA A / BLOQUEADO exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`**. No callable header is published.
