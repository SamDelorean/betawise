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
