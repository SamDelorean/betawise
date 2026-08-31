# SYS_A42C — NEO 2013 — blocked contract

Status: **mechanically reconstructed / contract blocked**.

`A42C` is callable only in the NEO 2013 vector among the three canonical OS3K generations examined. AS3000 2005 and NEO 2005 contain non-pointer/unresolved values at the equivalent slot.

The NEO 2013 implementation has three observable 32-bit ABI slots. The first acts as a base/pointer-like value, the low byte of the second selects a record using a `0x12C` stride, and the third is passed through to a private helper. Mechanically, the selected record contributes an address at offset `+0x34` to that helper.

The private helper leaves a meaningful 16-bit quantity in the low word of `D0`. `A42C` preserves `D0` and deliberately updates condition codes from `D0.w` before returning. However, no direct firmware caller or independent prototype was found, so it is not justified to publish the signedness, semantics, or even the external contractual requirement of that result.

A callable C prototype and semantic/vendor name are therefore intentionally withheld.

## Consolidation resolution trail

Future evidence capable of resolving the block includes an external caller showing all three arguments, a consumer that distinguishes `D0.w` or the resulting condition codes, an independent historical prototype, reproducible identification of the private helper chain, or dynamic observation of the selected record and third argument.

Until such evidence appears, use the neutral name `SYS_A42C` only and do not expose a callable header.
