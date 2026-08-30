# A360 / index 216 — `memcpy`

Status: **mechanically closed (confidence A)**.

Reconstructed contract:

```c
void *memcpy(void *dst, const void *src, size_t num);
```

The A360 entry point is mechanically identical in the AS3000 2005, NEO 2005, and NEO 2013 canonical firmware generations. Its contractual handler ends at the first terminal `RTS`, 0x4A bytes after entry; code located later before A364 belongs to adjacent internal routines and is not part of A360.

The handler consumes exactly three 32-bit slots: destination pointer, source pointer, and full-width byte count. It preserves the original destination, computes the source end from `src + num`, and copies forward. For sufficiently large aligned transfers it uses longword moves and then copies the remainder as bytes; smaller or unaligned transfers use byte moves. A zero count performs no copy. The epilogue explicitly places the original destination in full `D0.L` before return.

There is no overlap test and no backward-copy path. That behavior is consistent with `memcpy`, whose source and destination ranges may not overlap, and distinguishes A360 from `memmove`.

The official SmartApplet sweep found **233 executable A360 callers in 26 table-bearing applets**; four remaining table-bearing applets and eleven structurally non-table applets are negative, completing the 41-app corpus. The same detector reproduces previously established positive controls. Direct firmware references also reach A360 extensively: 19 absolute JSR sites in AS3000 2005, 22 in NEO 2005, and 33 in NEO 2013.

The historical BetaWise index-216 mapping to `memcpy` is retained only as secondary corroboration; the contract is independently established by firmware mechanics and callers.

Private regression rechecks canonical ROM hashes, the A-line vector, exact handler boundary and bytes, ABI slot loads, terminal return, direct ROM xrefs, and the complete official caller sweep. It executed **OVERALL PASS**. Dynamic/emulator-first validation was not executed and is not claimed here.
