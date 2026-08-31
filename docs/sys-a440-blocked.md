# SYS_A440 — mechanically reconstructed, semantic contract blocked

Status: **MECANICA_RECONSTRUIDA A / BLOQUEADO_SEMANTICA**.

`A440` is callable only in the NEO 2013 corpus examined here. The AS3000 2005 and NEO 2005 vector entries are non-pointer/unresolved values and are not treated as implementations of this service.

The NEO 2013 handler has five 32-bit stack slots plus a contractual aggregate-result channel through `A1`. The stack traffic establishes the following mechanical roles without assigning semantic names:

- slot 1 is a readable pointer to at least two 32-bit fields at offsets 0 and 4;
- slot 2 is a 32-bit pointer-like value consumed by a callback as a byte sequence terminated by NUL;
- slot 3 is a 32-bit writable byte-buffer pointer;
- slot 4 is physically a 32-bit slot but is consumed as a signed 16-bit value (`MOVE.W` followed by sign extension);
- slot 5 is a 32-bit writable pointer receiving a 16-bit value on the nontrivial path.

The incoming `A1` is preserved and, immediately before return, receives an exact six-byte copy. The layout is mechanically: one state byte at offset 0, an uninitialized/padding-compatible byte at offset 1, and a 32-bit value at offset 2. This is not merely inferred from local code: the sole recovered direct firmware caller prepares `A1` to point at a six-byte local object, calls A440, ignores `D0`, and later forwards those same six bytes to its own preserved `A1`. Thus the aggregate-result role of `A1` is confirmed at the binary ABI level. The pattern remains compatible with a GCC/m68k hidden structure-return convention, but that source-level attribution is intentionally not promoted without independent compiler/prototype evidence.

`D0` is specifically rejected as a coherent stable scalar return for A440. On the early path, the aggregate result contains a zero 32-bit field while `D0` is left holding an earlier value read from slot-1 data. On the normal path, `D0` ends with a different internally computed value. The sole direct caller does not consume `D0` after the call.

The early path also proves that slot 5 is not an unconditional output: when the two 32-bit fields referenced by slot 1 compare equal, A440 builds a six-byte result with state byte 12 and zero 32-bit field and returns without writing slot 5. On another internal condition the state byte can be changed to 13. These numeric states are mechanically observed but their vendor semantics are unknown.

A callback used by the handler operates over the slot-2 byte sequence and the slot-3 writable buffer, bounds writes using state derived from signed slot 4, NUL-terminates the destination, and updates private state. This supports the pointer/width roles above but does not justify naming the operation or the private state fields.

## Resolution clues for consolidation

The remaining debt is semantic rather than mechanical: identify the source-level meaning of the two-field object in slot 1, the byte sequence in slot 2, the writable buffer in slot 3, the signed-16 quantity in slot 4, the conditional 16-bit output in slot 5, and the state values including 12 and 13. Useful future evidence would include a historical prototype for A440 or its direct caller family, an external caller that types the five arguments, independent identification of the two 32-bit fields, or a dynamic probe that exercises the early and normal paths while observing slot 5 and the six-byte aggregate.

The second byte of the aggregate must not be assigned a semantic field from this evidence: it is not explicitly initialized by the handler and remains padding/indeterminate-compatible.

No callable public header is emitted while these semantic/source-level uncertainties remain. Private ROM bytes, full disassembly, caller listing, helper analysis, hashes, and regression transcripts are retained outside the public repository.
