# A430 / index 268 — source-first ABI trace

Status: **MECHANICS RECONSTRUCTED (A) / SOURCE_FIRST / RETURN CONTRACT UNKNOWN**.

## Source correlation

Historical BetaWise material and `ioma8/neo-re` map index 268 to the neutral placeholder `SYS_A430`. No independent vendor symbol or semantic prototype has been recovered.

## Generation split

- AS3000 2005 and NEO 2005 do not expose a callable handler at this slot.
- NEO 2013 exposes the callable implementation at runtime `0x0043467C`.

## Confirmed physical ABI and mechanics

Only one external 32-bit stack slot is read, and only its low byte affects execution. The value `0xFF` is an observed early-exit sentinel; no semantic enum name is assigned.

Before entering the record loop, the handler also gates execution on private state at `0x0000F416` and `0x00012C9B`. The loop uses private base state at `0x0000F412`, walks records with stride `0x012C`, and invokes private helper `0x004424BC` with one computed record pointer per iteration.

The loop counter itself is a word and the stride multiply is `MULU.W #0x012C`, so the record iteration is mechanically unsigned with respect to that internal counter.

The private helper accesses multiple fields in the selected record and calls lower private routines. A430 does not consume the helper's D0 result before advancing the loop.

## Result behavior

Several early-exit paths reach the epilogue without A430 defining D0. When the loop executes, D0 is merely inherited from the last helper call. There is no final normalization or result construction by A430.

This strongly supports procedure-like behavior, but `void` remains an inference rather than a confirmed public contract because no independent prototype or external caller has been recovered.

## Callers / xrefs

A whole-ROM direct search finds no `JSR`, `JMP`, `BSR.W`, or `BSR.B` to A430. The official applet corpus provides no structural caller for this late slot.

## Adversarial conclusions

- a multi-argument ABI is not supported by the handler: only the low byte of one external slot is read;
- `0xFF` is confirmed only as a sentinel, not as a named semantic constant;
- a uniform scalar return is rejected because early exits leave D0 untouched and loop exits inherit helper D0;
- a callable `void` prototype is intentionally withheld despite strong procedure-like evidence.

`RETURN_CONTRACT=UNKNOWN` and semantic identity are evidence boundaries, not implementation blockers. The neutral `SYS_A430` name is retained and no callable header is exposed.

## Regression state

A fresh static regression against all three canonical ROMs passed **33/33** assertions, covering canonical hashes, generation-specific vector slots, exact handler boundary/hash/bytes, private gates, sentinel access, record stride and helper call, helper hash/subcalls, instruction coverage, early-exit structure, absence of return normalization, and negative direct-xref scans.

Dynamic/emulator-first regression: **specified, not executed**.

Raw ROM bytes, extensive disassembly, and private reverse-engineering artifacts remain outside the public repository.
