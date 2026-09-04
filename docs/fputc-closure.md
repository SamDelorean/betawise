# A350 / `_OS3K_fputc` — raw ABI closure

Status: **mechanically closed (A) / source-first revalidated / published contract**.

## Contract

```c
int _OS3K_fputc(int c, FILE *stream);
```

A350 is the raw System 3 syscall used by BetaWise's separate public `fputc` wrapper. The wrapper may intercept `stdout` for extended-font rendering; this closure applies specifically to A-line index 212 / `_OS3K_fputc`.

## Primary reconstruction

The canonical vectors are AS3000 `0x004E0756`, NEO 2005 `0x005E30B2`, and NEO 2013 `0x004394B0`. The handler is exactly 0x140 bytes in all three ROMs. Reproducible CFG decoding yields 107 instructions, 19 direct branches, no undecoded opcodes, no non-call branch outside the handler, and one terminal RTS at +0x13E.

The handler consumes two physical 32-bit stack slots. The first is loaded as a full longword, preserved, and compared against character values including 8, 10, and 13. The second is used as the stream object. Success deliberately copies the original first longword to D0; failure explicitly sets D0 to -1. Consequently the return is a full integer value, not a byte-only result or incidental register residue.

The two accepted output-stream objects correspond to stored values 1 and 2 in every canonical ROM. An unrecognized stream follows the failure path. This observation is sufficient for the ABI reconstruction but is not generalized into undocumented host-libc behavior.

A334 independently calls A350 with the recovered character and stream value 1, discards A350's return, and reconstructs its own return from the original character. A34C / `fprintf` independently embeds A350 as the formatter's output-character callback. Together these integrations corroborate both argument order and writer semantics from primary firmware behavior.

## Source-first correlation

The historical raw mapping `_OS3K_fputc(int c, FILE *stream)` is used as an identity anchor rather than proof. Primary firmware independently establishes two full 32-bit slots, stream discrimination, a success result equal to the preserved input character, and an explicit `-1` failure. A334 then supplies an independent concrete caller with `(character, stream=1)`, while A34C independently passes A350 as the output-character callback to its formatting engine.

The separate BetaWise `fputc` implementation is also correlated before naming the trap: it can handle the extended-font stdout path itself and otherwise delegates to `_OS3K_fputc`. This proves that A350 is the raw service, not the complete public wrapper. The identity is therefore supported by firmware dataflow, concrete integration, callback use, and historical source-layer separation.

## Raw syscall versus wrapper

Historical BetaWise sources map syscall index 212 to `_OS3K_fputc` and declare `int _OS3K_fputc(int c, FILE *stream)`. Separately, `os3k.c` implements the public `int fputc(int c, FILE *stream)` wrapper, which can handle `stdout` itself when the extended-font path is active and otherwise delegates to `_OS3K_fputc`.

This distinction is material: A350 is **not** documented here as the complete public wrapper implementation.

## Caller survey

The corrected PC-index detector exhausted the official corpus: 30 table-bearing SmartApplets were rehashed and scanned, while 11 applets are structural negatives. Result: **0 executable direct A350 callers / 41**. As a positive control, the same detector reproduces exactly the previously validated 12 A330 callers and 67 A33C callers, so the A350 zero is a validated negative search rather than detector failure.

Firmware xrefs contain one direct BSR to A350 per generation from A334 and no direct absolute JSR. Additional uses of A350 as a callback are treated separately from A-line callsites.

## Platform differences

The ABI, CFG shape, success/failure contract, and accepted stream values are common across AS3000 2005, NEO 2005, and NEO 2013. Absolute helper/global operands relocate normally. One auxiliary literal in the stream-2 path is `0x00012111` on AS3000 and `0x00011111` on both NEO ROMs; no undocumented meaning is assigned to that difference.

## Adversarial conclusions

`void` is rejected because D0.L is deliberately defined as `c` or `-1`. A byte-only return is rejected because success copies the complete preserved longword. A one-argument ABI is rejected by the two independent stack loads and A334 call shape. Treating A350 as the entire public `fputc` wrapper is rejected by the independently present wrapper implementation. The `fputc` identity is not accepted merely from libc resemblance: it is supported by the ROM ABI/return behavior, A334 integration, A34C callback use, and independent historical raw mapping.

## Validation

Private static regression: **OVERALL PASS**. It checks canonical ROM hashes, vector entry, handler hashes, CFG, argument loads, full-width return, stream values, and direct firmware xrefs. An initial regression assertion used an incorrect offset for the argument-preservation instruction and was corrected before the canonical PASS result. Dynamic execution was not performed.

Private Drive workpapers retain the exact ROM hashes, three separated 68k disassemblies, caller manifest, correlated helper/side-effect analysis, and regression source/output.
