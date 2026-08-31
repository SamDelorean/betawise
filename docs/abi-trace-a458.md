# SYS_A458 / index 278 — ABI trace

Status: **mechanically reconstructed (A), no callable contract in the canonical corpus**.

## Availability

- AlphaSmart 3000 (2005): slot 278 contains a value outside the mapped ROM image; it is not treated as a callable pointer.
- NEO (2005): slot 278 likewise contains a value outside the mapped ROM image; it is not treated as a callable pointer.
- NEO (2013): slot 278 is null. The following three slots are also null; the vector resumes with a valid pointer at index 282.

## Official SmartApplet corpus

The canonical corpus contains 16 SmartApplets with the extended late A-line table. All 16 carry the late sequence `A450, A454, A468, A46C`; none contains an intervening A458 stub. Fourteen additional table-bearing applets use an older table that terminates before this range, and eleven corpus applets contain no A-line table.

Thus the official 41-SmartApplet corpus has no valid A458 stub or caller.

## Mechanical disposition

For the canonical ROM/Applet corpus, A458 is classified as a non-callable ABI hole rather than a reconstructable function. There is therefore no handler body, argument list, return contract, helper/global contract, or C prototype to publish.

This classification is deliberately corpus-bounded: it does not assert a vendor name or rule out use of the numeric trap in firmware builds not present in the evidence set.

## Publication boundary

No ROM bytes, extended disassembly, vendor function name, signature, enum, or synthetic implementation is asserted here. Those evidence details remain in the private workpapers.
