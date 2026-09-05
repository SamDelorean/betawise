# A434 / index 269 — source-first ABI trace

Status: **MECHANICS RECONSTRUCTED (A) / SOURCE_FIRST / SEMANTICS UNKNOWN**.

## Source correlation

Historical BetaWise material and `ioma8/neo-re` map index 269 to the neutral placeholder `SYS_A434`. No independent vendor symbol or semantic prototype has been recovered.

## Generation split

- AS3000 2005 and NEO 2005 do not expose a callable handler at this slot.
- NEO 2013 exposes the callable implementation at runtime `0x0043490C`.

## Confirmed physical ABI

Two external stack slots are observed:

1. `arg1`: a full 32-bit pointer to readable bytes;
2. `arg2`: only the low 16 bits are used as the traversal bound.

The bound comparison branches with the unsigned `BCC` condition, so the narrow mechanical interpretation of `arg2` is unsigned-16-like. No wider contractual type is inferred.

## Confirmed mechanics

The handler initializes a 16-bit accumulator, walks input bytes until either the unsigned bound or a NUL byte, and treats byte value `0x2F` specially. For the ordinary path it reads a shared pointer rooted at `0x00007E2E`, dereferences its `+4` field, uses the current input byte as a zero-extended byte-table index, zero-extends the resulting table byte, and adds it to the 16-bit accumulator.

The shared `0x00007E2E` address appears in seven NEO 2013 locations. That supports a shared transformation/classification-table interpretation, but is not sufficient to assign a vendor name or semantic contract.

## Result behavior

The accumulator is initialized to zero and the epilogue deliberately executes `MOVE.W D6,D0`. Therefore the low 16-bit machine result is **confirmed** and arithmetic is modulo 16 bits.

The upper 16 bits are not explicitly normalized on every path, so a 32-bit scalar return is not justified. An unsigned-16 return interpretation is mechanically strong, but the semantic meaning of the value remains unknown and there is no independent prototype or direct caller to promote that inference to a public callable contract.

## Callers / xrefs

A whole-ROM direct search finds no `JSR`, `JMP`, `BSR.W`, or `BSR.B` to A434. The official applet corpus provides no structural caller for this late slot.

## Adversarial conclusions

- a one-argument ABI is rejected because the low word of the second external slot bounds traversal;
- a signed traversal bound is not supported by the observed unsigned branch condition;
- a 32-bit return is rejected because only D0.W is explicitly defined at the epilogue;
- a semantic/vendor name is withheld because the shared table and accumulated quantity remain unidentified.

The semantic unknown is an evidence boundary, not an implementation blocker. The neutral `SYS_A434` name is retained and no callable header is exposed.

## Regression state

A fresh static regression against all three canonical ROMs passed **29/29** assertions after correcting a discarded test-only instruction-count assumption. It covers canonical hashes, generation-specific slots, handler boundary/hash, 34-instruction full decode, both arguments, unsigned bound, NUL termination, special `0x2F` handling, shared-table indexing, accumulator/return behavior, seven raw references to `0x00007E2E`, and negative direct-xref scans.

Dynamic/emulator-first regression: **specified, not executed**.

Raw ROM bytes, extensive disassembly, and private reverse-engineering artifacts remain outside the public repository.
