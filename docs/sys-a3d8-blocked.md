# A3D8 — neutral blocked disposition

Status: **mechanically reconstructed; callable contract blocked**.

This note records only public-safe conclusions from the OS3K ABI reconstruction. It does not publish firmware bytes or a callable prototype.

## Generational availability

- AlphaSmart 3000 (2005): the A3D8 vector-table word is not a callable handler pointer.
- NEO (2005): the A3D8 vector-table word is not a callable handler pointer.
- NEO (2013): A3D8 resolves to a real handler.

The NEO 2013 handler is 0x178 bytes long and has SHA-256 `15c33d2963c3ada1e1dc15669cf7a4b109c213f95f5b37896b10825f1c94f837`. Its control-flow graph has one ordinary return.

## Mechanical ABI observations

The NEO 2013 handler consumes four physical 32-bit stack slots:

1. low 16 bits are used;
2. low 8 bits are used;
3. all 32 bits are used;
4. all 32 bits are used as a pointer-like value.

Semantic argument names remain unknown.

Across all reachable exits, the low byte of D0 is mechanically one of `0`, `3`, `5`, or `8`. The nonzero exits use full-register immediate assignments, while the normal zero path clears only `D0.B`; therefore the upper 24 bits are not uniformly defined.

A byte-sized return is consequently a strong mechanical inference, but it is **not promoted to a public contract**: no direct firmware caller of A3D8 was found, the official SmartApplet tables do not expose this late slot, and no independent prototype or recovered vendor symbol was found in the reference codebases checked. Declaring `void` would also overstate the evidence because the handler deliberately constructs several low-byte status values.

## Validation

A reproducible static regression over the canonical ROM evidence passed 29/29 assertions. Dynamic/emulator-first validation has not been executed.

## Disposition

A3D8 remains named neutrally and no callable header is published. Its unresolved item is specifically the **contractual return type/intent**, not its handler boundaries, physical argument slots, control flow, or mechanical D0 behavior.
