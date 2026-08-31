# A44C / index 275 — ABI trace

Status: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO_SEMÁNTICA_DOBLE_SELECTOR + ARG5 + RETORNO_D0**.

A44C is callable in the canonical NEO 2013 ROM. The corresponding vector slots in the canonical AS3000 2005 and NEO 2005 ROMs contain non-pointer data (`0x02040307`), so no cross-generation callable contract is assigned there.

## NEO 2013 mechanical contract

- vector index: 275 (`A44C`)
- runtime handler: `0x004432B0`
- file offset: `0x0332B0`
- exact handler length: `0x6C` bytes
- exact-handler SHA-256: `ac24442e5758ff6fdc9bd12ab8c9dde51565fe1ad13eea4ef3c5af2e11d34ec0`
- incoming `A1` is preserved and receives an aggregate result of exactly 6 bytes
- six external 32-bit stack slots are mechanically present because the handler reads through `28(A6)`
- external slot 5 (`24(A6)`) is not consumed by the handler

The handler composes two neighboring services:

1. `SYS_A448(arg1, arg2, descriptor_array, 2, &count)` with the 6-byte aggregate returned through `A1`;
2. if the aggregate state is `0` or `8` and signed `count > 0`, `SYS_A440(&descriptor_array[0], arg3, arg4, 18, arg6)`, again with the aggregate through `A1`.

Only the first descriptor from A448 is forwarded to A440. The literal `18` is supplied internally to A440; it is not the unused fifth external stack slot. On paths that skip A440, the A448 aggregate is propagated unchanged. The final aggregate is copied to the caller's original `A1` destination as a longword plus a word.

## Argument evidence

| External channel | Mechanical evidence |
| --- | --- |
| `A1` | writable 6-byte aggregate destination |
| slot 1, `8(A6)` | full 32-bit value forwarded as A448 arg1 |
| slot 2, `12(A6)` | full 32-bit value forwarded as A448 arg2; A448 proves an exact NUL-string selector/filter role |
| slot 3, `16(A6)` | full 32-bit value forwarded to A440 arg2 on stage 2 |
| slot 4, `20(A6)` | full 32-bit value forwarded to A440 arg3, a writable byte buffer |
| slot 5, `24(A6)` | present in the external stack layout but not read by A44C; source-level meaning unknown |
| slot 6, `28(A6)` | full 32-bit value forwarded to A440 arg5, a pointer-like word output on A440's nontrivial path |

The source-level type/name of both selector-like inputs remains unresolved. The two-stage structure strongly supports a nested structured-markup lookup/extraction interpretation, but does **not** prove a specific XML/HTML tag/attribute API or vendor name.

## Return boundary

A44C does not normalize `D0`. If stage 2 is skipped, `D0` is inherited from A448; if stage 2 runs, it is inherited from A440. A440 independently demonstrates a route where `D0` is not a stable scalar result. Consequently no scalar `D0` contract is published for A44C.

The 6-byte `A1` aggregate is mechanically contractual. Whether the original source-level ABI represented that channel as a compiler hidden structure return or another OS3K convention remains unknown.

## Callers / negative search

The NEO 2013 ROM has no direct BSR/JSR/JMP reference to the A44C handler.

The official NEO AlphaWordPlus image contains one A44C A-line stub inside the contiguous A43C/A440/A444/A448/A44C/A450/A454 family, but exhaustive resolution of all 2,011 indexed-PC JSR calls in that image finds no call to A44C (or to A43C through A454). The same method resolves nine calls to neighboring A46C, providing a positive control for the search. Therefore AlphaWordPlus provides a linked stub, not a concrete A44C caller, and no argument values are attributed to it.

## Regression

Static regression against the three canonical ROMs and official AlphaWordPlus: **33/33 PASS**. Dynamic/emulator/hardware regression has **not** been executed.

No callable SDK header is published from this reconstruction. Retain the neutral symbol `SYS_A44C` until the remaining semantic debts are independently resolved.
