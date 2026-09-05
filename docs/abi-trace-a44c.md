# A44C / index 275 — ABI trace

Status: **MECANICA_RECONSTRUIDA A / SOURCE_FIRST / PUBLICADO**.

Historical correlation: `betawise-master/os3k/syscall.c` establishes neutral placeholder `SYS_A44C` at index 275; `os3k.h` contains no independent A44C vendor prototype. The corresponding vector slots in canonical AS3000 2005 and NEO 2005 contain non-pointer data `0x02040307`; NEO 2013 is callable at `0x004432B0`.

## NEO 2013 mechanical contract

- runtime handler: `0x004432B0`
- file offset: `0x0332B0`
- exact length: `0x6C` bytes
- SHA-256: `ac24442e5758ff6fdc9bd12ab8c9dde51565fe1ad13eea4ef3c5af2e11d34ec0`
- incoming `A1` is preserved and receives an aggregate result of exactly 6 bytes
- six external 32-bit stack slots are mechanically present because the handler reads through `28(A6)`
- external slot 5 (`24(A6)`) is physically present but not consumed

The handler composes two neighboring services:

1. `SYS_A448(arg1, arg2, descriptor_array, 2, &count)` with the aggregate returned through `A1`;
2. if aggregate state is `0` or `8` and signed `count > 0`, `SYS_A440(&descriptor_array[0], arg3, arg4, 18, arg6)`, again through `A1`.

Only the first A448 descriptor is forwarded to A440. Literal `18` is supplied internally and is not the unused fifth external slot. Paths that skip A440 propagate the A448 aggregate unchanged. The final aggregate is copied to the caller's original A1 destination as a longword plus a word.

## Argument and pipeline evidence

- slot 1 forwards A448 arg1;
- slot 2 forwards A448's exact-string structural selector/filter;
- slot 3 becomes A440 arg2 on stage 2;
- slot 4 becomes A440's writable byte buffer;
- slot 5 is not read by A44C and remains semantically unknown;
- slot 6 becomes A440's pointer-like word output.

The selector1 → descriptor[0] → selector2/buffer pipeline is **confirmed mechanically**. A nested structured-markup lookup/extraction interpretation is a strong inference, but tag/attribute/path terminology and vendor naming remain unproved.

## Return boundary

A44C does not normalize `D0`: it inherits A448 when stage 2 is skipped and A440 when stage 2 runs. Because A440 independently refutes a stable scalar D0 contract, no scalar return is published for A44C. The six-byte A1 aggregate is the demonstrated structured result channel.

## Callers / negative search

The NEO 2013 ROM has no direct BSR/JSR/JMP reference to A44C. The official NEO AlphaWordPlus image links the A44C stub but exhaustive indexed-PC JSR resolution found no call to it; the method positively detects neighboring A46C calls. Thus no argument values are fabricated from mere stub presence.

## Regression and disposition

Fresh canonical-ROM directed static regression on 2026-09-05: **29/29 PASS**, including all three ROM hashes, slots 269..276, exact handler, six-slot shape, unused slot5, A448 capacity 2, state gate, A440 literal 18, both BSR targets, terminal A1 copy and negative xrefs.

Earlier full correlated regression remains **33/33 PASS**. Dynamic/emulator/hardware regression is **specified, not executed**.

The former semantic-block label is superseded: unresolved selector names, unused-slot meaning and scalar-D0 interpretation are **evidence limits, not sequencing blockers**. No callable SDK header or invented vendor name is emitted.
