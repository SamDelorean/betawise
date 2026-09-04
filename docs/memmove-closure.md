# A364 / index 217 — `memmove`

Status: **mechanically closed (confidence A)**.

Reconstructed contract:

```c
void *memmove(void *dst, const void *src, size_t num);
```

The A364 handler is exactly 0x3C bytes and is byte-identical in the canonical AS3000 2005, NEO 2005, and NEO 2013 firmware generations. It consumes three full 32-bit slots: destination, source, and byte count, and explicitly returns the original destination in full `D0.L`.

Unlike the separately reconstructed A360/`memcpy`, A364 compares the source and destination addresses and selects a copy direction. One route copies bytes forward from the starts of the ranges; the other advances both cursors to the ends and copies backward toward lower addresses. This direction selection provides the mechanically observed overlap-safe move behavior and independently distinguishes A364 from `memcpy`.

The official SmartApplet corpus contains **95 executable A364 callers in 13 table-bearing applets**. The remaining 17 table-bearing applets are negative and 11 further official applets are structural negatives, completing the 41-app corpus. Direct absolute firmware calls also provide independent integration evidence: 27 JSR sites in AS3000 2005, 31 in NEO 2005, and 32 in NEO 2013.

The historical BetaWise index-217 mapping to `memmove` is retained only as secondary corroboration. The public contract above is established from firmware dataflow, the forward/backward copy paths, the explicit destination return, and caller evidence.

Private static regression verifies canonical ROM hashes, A-line vectors, exact handler bytes and boundary, ABI loads, terminal return, direct ROM references, and the complete official caller sweep. It executed **OVERALL PASS**. Dynamic/emulator-first validation was not executed and is not claimed here.

## SOURCE-FIRST re-audit — 2026-09-04

The re-audit began from the historical source anchors rather than from the binary label: `os3k/syscall.c` maps index 217 to `memmove`, and `os3k/os3k.h` preserves `void *memmove(void *dst, const void *src, size_t num)`. Those references were treated as hypotheses, not authority.

The existing primary firmware workpapers independently reconfirm the same contract in all three canonical ROMs: the handler is 0x3C bytes and byte-identical, consumes full-width `dst/src/count` slots, preserves and returns the original destination in `D0.L`, and explicitly chooses forward versus backward traversal from the relative source/destination addresses. That overlap-aware branch is the decisive mechanical discriminator from A360/`memcpy`. The complete 41-app caller corpus (95 executable callers) and direct ROM xrefs (27/31/32) remain consistent with the three-slot ABI and destination return. No source/firmware contradiction was found.

Classification after re-audit: **CLOSED A / SOURCE_FIRST / PUBLISHED**. The previously executed private static regression remains `OVERALL PASS`; dynamic/emulator-first regression remains **NOT EXECUTED**.
