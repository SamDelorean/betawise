# A3D4 / index 245 — mechanically reconstructed, return contract unresolved

A3D4 has been reconstructed to the maximum level justified by the current primary corpus, but **no callable C prototype is published** because the return intent cannot be independently demonstrated.

## Platform availability

- AlphaSmart 3000 (2005): raw index-245 slot `0x5B5E004E`, not a demonstrated code pointer.
- NEO (2005): raw index-245 slot `0x8068005E`, not a demonstrated code pointer.
- NEO (2013): real handler at runtime `0x0043F508`, file offset `0x02F508`.

No semantic meaning is assigned to the old-generation slot values.

## NEO 2013 mechanics

The NEO 2013 handler is exactly `0x3A` bytes long, contains 19 reproducibly decoded 68000 instructions, and terminates at a unique `RTS` at `+0x38`; the following bytes begin an independent function prologue. SHA-256 of the exact handler bytes is:

`c7ff7ac9d4fc4f34d2d1881238a0c3b88c8c6275813200050ffc9e67f0bc3aec`

A3D4 consumes one 32-bit caller slot as a full source pointer. A 16-bit count is assembled from two internal bytes, and A360/`memcpy` is invoked with a fixed internal destination buffer, the caller source pointer, and the zero-extended count. The handler performs no local NULL or source-capacity validation.

## Mechanical D0 state versus contract

After `memcpy`, A3D4 writes only the low word of D0 with the internal 16-bit count. Because A360 returns the destination pointer in D0.L, the upper word remains inherited from that value. For the observed internal destination address, the exact mechanical D0.L at `RTS` is therefore `0x00010000 | count16`.

That mixed value is useful evidence precisely because it shows why a C return type cannot be inferred from the last instruction alone:

- `uint16_t` is plausible from the final `MOVE.W`, but no caller or prototype demonstrates that the upper word is intentionally ignored.
- `uint32_t` would elevate an inherited upper word into an asserted API result without evidence.
- `void` is also plausible, but there is no demonstrated caller whose non-use can establish vendor intent.

Accordingly, the contractual return remains **unknown**.

## Caller evidence

NEO 2013 contains no direct absolute JSR, direct absolute JMP, or direct BSR.W to A3D4. The official 41-SmartApplet corpus also provides no A3D4 caller because the demonstrated extended A-line tables terminate physically at A3BC; A3D4 has no exposed SmartApplet slot in that corpus.

No independent A3D4 symbol or prototype was recovered from the current BetaWise repository or the searched `ioma8/neo-re` evidence.

## Regression and disposition

Reproducible static regression revalidates the three canonical ROM hashes, generation-specific slot values, NEO 2013 vector/file mapping, exact handler length/hash/boundary, the single physical input slot, the A360 dependency, internal count/buffer dataflow, absence of direct firmware callers, structural SmartApplet absence, and the mixed D0 mechanics. Result: **OVERALL PASS**.

Dynamic/emulator regression is specified but has not been executed. Full firmware bytes and detailed disassembly remain private in Drive.

Status: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO** exclusively for `RETORNO_CONTRACTUAL=DESCONOCIDO`. Input ABI and memory effects are mechanically confirmed; semantic naming remains unknown. No callable prototype is asserted.
