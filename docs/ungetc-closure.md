# A3B0 / `ungetc` closure

A3B0 (index 236) is mechanically reconstructed as:

```c
int ungetc(int c, FILE *stream);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004E0932`, file `+0x020932`
- NEO 2005: runtime `0x005E328E`, file `+0x02328E`
- NEO 2013: runtime `0x0043968C`, file `+0x02968C`

A3B4 is not used as a size boundary because its AS3000/NEO2005 table values are not valid handler pointers. Direct CFG analysis closes A3B0 at the first reachable RTS: 0x38 bytes, RTS at `+0x36`. The immediately following RTS is adjacent code and is not part of the handler.

Raw handler SHA-256 values are:

- AS3000: `6a5f13d4a2d8532652f112159b0be5e2b38d1ca61d017e844f8483fd69138f40`
- NEO 2005: `bec5321ea7419dc266571b89775b8b272bd29891141b8617563aa4f26e607191`
- NEO 2013: `b425af6198ef615788ad53331bcd0f4512121dbf349855c0469d44dc64a06631`

After neutralizing only the three relocated absolute operands (stdin descriptor, read index, and buffer base), all three normalize to SHA-256 `200e119286c806c79998d99c9f2b1624b9ecdae6510ad8adc78d71f80f7a068a`.

The handler consumes two 32-bit slots. `c == -1`, a stream other than the stdin descriptor, or a zero read index returns `-1` without pushback. On success it decrements the shared read index, stores the low byte of `c` at `buffer_base + index`, and returns the original full longword `c` in `D0.L`.

This last point is an observed implementation detail: for a value outside the byte range, the stored byte is truncated while the success return remains the original longword. The closure records that firmware behavior rather than imposing a cleaner ISO-C interpretation.

## Correlation with the input subsystem

A348 was independently reconstructed as `int fgetc(FILE *stream)`. It uses the same buffer base/read-index state that A3B0 rewinds. A354/`fscanf` and A374/`scanf` each pass A348 and A3B0 as the paired get/unget callbacks to the common scan engine in all three ROM generations.

The stdin descriptor used by A3B0 is the same descriptor row already correlated for the scan family, whose first three longwords are 0/1/2 for stdin/stdout/stderr representation in the preserved SDK.

## Caller and xref evidence

The official SmartApplet corpus is complete at 41/41: zero executable A-line calls to A3B0. All 30 table-bearing applets were rehashed, and the other 11 are structural negatives without the table. Controls on the same detector reproduce A3A8=39 and A3AC=57.

There are no direct JSR/JMP/BSR entries to A3B0 in any canonical ROM. Instead, each ROM contains exactly two executable `PEA` references to the A3B0 runtime address, from the `fscanf` and `scanf` wrappers. A third literal occurrence is the A-line table entry. This is positive functional use as a callback rather than an absence of use.

## Adversarial alternatives

- `fgetc`/`getc` is rejected: A3B0 rewinds and writes the shared input buffer; A348 is the paired getter.
- A generic buffer setter is rejected by the stream validation, deliberate success/failure return, and get/unget callback pairing.
- `void` is rejected because both failure and success deliberately construct `D0.L`.
- Generic multi-stream support is rejected for this implementation because any stream value other than stdin returns `-1`.

Historical BetaWise `ungetc` naming is secondary corroboration only. The contract follows from the handler, shared state, and callback consumers.

## Validation status

Static regression executed with **OVERALL PASS** across all three canonical ROMs and the complete official SmartApplet corpus. It verifies ROM hashes, 0x38 CFG boundary, normalized handler identity, zero direct applet/ROM calls, exactly two executable callback references per ROM, shared A348 state, and failure/success vectors including the low-byte-store/full-long-return quirk.

Dynamic emulator-first regression is specified but not executed.

Status: **MECÁNICA_CERRADA A / PUBLICADO**. The existing `os3k.h` prototype already matches the reconstructed contract, so no header change is required.
