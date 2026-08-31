# SYS_A430 — NEO 2013 — blocked contract

Status: **mechanically reconstructed / contract blocked**.

`A430` is callable only in the NEO 2013 vector among the three canonical OS3K generations examined. AS3000 2005 and NEO 2005 contain non-pointer/unresolved values at the equivalent slot.

The NEO 2013 implementation is a distinct 0x4C-byte routine. The only externally observed argument access reads the low byte of one 32-bit stack slot. The value `0xFF` acts mechanically as an early-exit sentinel. The routine otherwise iterates records using a 0x012C stride and invokes an internal helper on each computed record pointer.

No callable C prototype is published. Several exit paths return without defining `D0`, while the iterative path leaves `D0` inherited from an internal helper whose result is not consumed by `A430`. This strongly suggests procedure-like behavior, but `void` is not promoted to a contract without an external caller, historical prototype, or discriminating dynamic evidence.

No direct firmware caller was found by exhaustive direct JSR/JMP/BSR scan, and this late A-line slot is outside the structurally available syscall range of the official SmartApplet corpus already validated in the forward sweep. No independent symbol or prototype is currently known.

## Consolidation resolution trail

Evidence capable of resolving the remaining contract includes an external or indirect caller that constructs the argument, an independent historical prototype/symbol, corroborated identification of the record structure and helper family, or dynamic evidence demonstrating whether `D0` is contractually irrelevant.

Until such evidence appears, use the neutral name `SYS_A430` only and do not expose a callable header.
