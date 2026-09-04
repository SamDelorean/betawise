# A354 / `fscanf` closure

A354 is mechanically closed against the three canonical OS3K firmware generations.
The public contract is:

```c
int fscanf(FILE *stream, const char *fmt, ...);
```

The existing declaration in `os3k/os3k.h` and the historical syscall mapping already
match this contract, so no source/header change is required.

## Source-first re-audit

The 2026-09-04 source-first pass started from the preserved SDK surface rather than
from the binary name: `os3k/syscall.c` independently maps index 213 to `fscanf`, and
`os3k/os3k.h` preserves the variadic `FILE *`/format prototype. Those references were
treated as hypotheses only. The firmware reconstruction below independently confirms
the same stream-scanner ABI, while comparison with A344/`sscanf` and A348/`fgetc`
provides cross-function correlation. No contradiction requiring a contract change was
found.

## Mechanical evidence

The A354 handler is 0x30 bytes / 15 decoded instructions in AS3000 2005, NEO 2005
and NEO 2013. Each version contains one call to the common scan engine, no conditional
branch, and a single terminal `RTS` at +0x2E. After neutralizing the two callback
addresses and the scan-engine address, all three handlers have the same SHA-256:
`08d173ab919600a629e4ba0416e60b5cd40d29dc3deac238f981696f447b3db3`.

Stack dataflow independently establishes the fixed ABI. The first physical slot is
forwarded as the stream context, the second as the format pointer, and the address of
the third slot is passed as the start of the variadic argument area. The wrapper
installs A348 and A3B0 as the stream adapters and calls the exact same scan engine used
by A344/`sscanf`. A348 has already been independently reconstructed as `fgetc`.
A344 differs by supplying string-source adapters and mode 1; A354 supplies the stream
adapters and mode 0. The scan engine's full D0 result is propagated unchanged.

This convergence establishes the `fscanf` interface without relying on the historical
name alone. The historical BetaWise index-213 mapping to `fscanf` is retained only as
secondary corroboration.

## Callers and regression

The corrected PC-index detector was run across the complete official SmartApplet
corpus: 30 table-bearing applets were hash-verified and yielded zero executable A354
calls; the remaining 11 applets are structural negatives without the relevant A-line
table. The same detector and corpus reproduce the canonical positive controls A330=12
callers and A33C=67 callers, validating the negative A354 result.

A private reproducible static regression checks canonical ROM hashes, handler length,
terminal RTS, callback targets, common scan-engine target, fixed-slot accesses,
variadic-area construction, stack cleanup and normalized handler identity. It executed
with `OVERALL PASS` for all three ROMs. Dynamic/emulator validation has not been
executed and is not claimed here.

No conversion semantics, EOF policy, error enumeration or errno behavior beyond what
is mechanically established by this wrapper and its shared engine are inferred.
Extensive disassembly and firmware bytes remain private in the project workpapers.
