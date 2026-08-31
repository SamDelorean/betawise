# SYS_A43C — mechanically reconstructed, contract blocked

Status: **MECANICA_RECONSTRUIDA A / BLOQUEADO**.

`A43C` is callable only in the NEO 2013 corpus examined here. The older AS3000 2005 and NEO 2005 vector entries are non-pointer/unresolved values and are not treated as implementations of this service.

The NEO 2013 handler has been bounded and independently regression-checked. Its externally visible stack traffic demonstrates five 32-bit argument slots. The fourth slot is consumed only through its low byte. In addition, the handler preserves the incoming `A1` and, immediately before return, copies a six-byte local result to the address supplied in that register.

The six-byte result is mechanically composed as follows: byte 0 is a state byte, byte 1 is not explicitly initialized by this handler, and bytes 2..5 contain a 32-bit delta derived from internal state and the first stack argument. The byte-1 behavior is consistent with padding and must not be assigned a semantic field without independent evidence.

This repeated `A1` output pattern is also present in the contiguous A440/A444/A448/A44C handlers. It is strongly compatible with the m68k GCC-style hidden structure-return convention, but no recovered OS3K caller or prototype currently distinguishes that compiler convention from a proprietary output-pointer convention using `A1`. For that reason, no callable C prototype is published.

`D0.L` contains the same 32-bit delta at return, but the handler does not establish that this register is a contractual scalar return rather than an incidental value left by construction of the aggregate result. It therefore remains non-contractual.

## Resolution clues for consolidation

The unresolved debt is narrowly defined: determine whether incoming `A1` is a hidden aggregate-return pointer or a proprietary output parameter, and independently determine whether `D0.L` is also contractual. Useful future evidence would include a recovered caller that explicitly prepares `A1`, a historical C/C++ prototype for any member of the A43C–A44C family, compiler-identifiable call-site code, or a dynamic probe that observes the caller-side ABI. The repeated family pattern and the exact six-byte copy already constrain any future interpretation; a model that ignores the `A1` destination is inconsistent with the handler.

Semantic names for the five stack arguments, state globals, helper codes, and callback roles remain unknown and are intentionally not invented.

Private ROM bytes, full disassembly, helper workpapers, hashes, and regression transcripts are retained outside the public repository.
