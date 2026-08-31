# SYS_A428 — NEO 2013 — blocked contract

Status: **mechanically reconstructed / contract blocked**.

`A428` is callable only in the NEO 2013 vector among the three canonical OS3K generations examined. AS3000 2005 and NEO 2005 contain non-pointer/unresolved values at the equivalent slot.

The NEO 2013 implementation is a distinct 0x82-byte routine. Its observable external stack access reads the second 32-bit ABI slot as a writable pointer-like value; the first ABI slot is not read by the routine. The implementation writes through that pointer, passes it to an internal helper, and finishes by copying only an internal selector byte into the low byte of `D0`.

A callable C prototype is intentionally not published because two contractual facts remain unresolved:

- whether the externally visible call shape includes a first reserved/ignored 32-bit slot, or whether another calling convention explains the observed stack layout;
- whether the low byte left in `D0` is a contractual return value, and if so its type/meaning.

No direct firmware caller was found by exhaustive direct JSR/JMP/BSR scan, and the late A-line slot is outside the structurally available syscall range of the official SmartApplet corpus already validated in the forward sweep. No independent symbol or prototype is currently known.

## Consolidation resolution trail

Future evidence capable of resolving the block includes: an external caller with observable argument construction, an independent historical prototype, a consumer that treats `D0` in a width-discriminating way, dynamic observation of the writable buffer, or independent identification of the two internal helpers / associated record layout.

Until such evidence appears, use the neutral name `SYS_A428` only and do not expose a callable header.
