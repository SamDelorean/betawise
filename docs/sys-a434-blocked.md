# SYS_A434 — NEO 2013 — blocked semantics

Status: **mechanically reconstructed / semantics blocked**.

`A434` is callable only in the NEO 2013 vector among the three canonical OS3K generations examined. AS3000 2005 and NEO 2005 contain non-pointer/unresolved values at the equivalent slot.

The NEO 2013 implementation is a distinct 0x5E-byte routine with two externally observed stack slots: a full 32-bit pointer-like first slot to readable bytes, and a second slot whose low 16 bits are used as an unsigned traversal bound. The routine walks input bytes until either the bound or a NUL byte, treats byte value `0x2F` specially, indexes a shared byte table derived from global `$00007E2E`, and accumulates zero-extended byte values into a 16-bit accumulator.

The low 16 bits of the return value are mechanically confirmed: the routine copies the accumulator word into `D0.W` immediately before return. The upper 16 bits are not contractual on every path, so a 32-bit scalar return must not be assumed. An unsigned-16 interpretation is mechanically strong, but the semantic meaning of the accumulated value remains unknown.

No direct firmware caller was found by exhaustive direct JSR/JMP/BSR scan, and no independent historical symbol or prototype is currently known. A semantic/vendor name is therefore intentionally not assigned.

## Consolidation resolution trail

Useful future evidence includes identification of the shared structure/table rooted at `$00007E2E`, correlation of its six other NEO 2013 references, an external or indirect caller that consumes `D0.W`, an independent historical prototype/symbol, or discriminating dynamic input/output tests.

Until then, retain the neutral name `SYS_A434` and avoid a semantic name or 32-bit return contract.
