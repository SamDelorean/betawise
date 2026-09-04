# A348 / `fgetc` — ABI correction and closure

Status: **mechanically closed (A) / source-first revalidated / historical BetaWise mapping corrected**.

## Correct contract

```c
int fgetc(FILE *stream);
```

A348 consumes exactly one 32-bit stream slot. OS3K represents `stdin`, `stdout`, and `stderr` as stream values 0, 1, and 2. A348 accepts only `stdin` (0): unsupported streams return `-1`. The valid path returns one input byte zero-extended to the 32-bit C `int` result domain, 0..255.

## Why the historical `fgets` mapping is wrong

BetaWise historically mapped syscall index 210 / A348 as:

```c
char *fgets(char *str, int num, FILE *stream);
```

Primary firmware contradicts that declaration. The A348 handler reads only the first caller argument, never accesses a destination pointer or length argument, and returns either `-1` or an integer byte rather than a pointer/NULL. Keeping `fgets` as an alias for this trap would therefore be ABI-unsafe; this is a contract correction, not a cosmetic rename.

## Firmware mechanics

The A348 handler is 0x9C bytes in all three canonical ROMs and ends in `RTS` at +0x9A. After normalizing generation-specific absolute operands, all three implementations have SHA-256 `a7e6b1411b5edc7b506c27aa4d76f357503cfa14dc887fe8bc4396e40d03b4c7`.

The prologue saves four registers. Its access at post-prologue `0x14(SP)` is therefore the original first caller slot. That value is compared with a firmware stream table containing the consecutive longwords 0, 1, and 2. A nonzero stream returns `-1`.

The stdin route manages OS-owned buffered line input. It handles backspace `0x08`, maps carriage return `0x0D` to line feed `0x0A`, buffers through line termination or the observed `0x50`-byte bound, and returns the next buffered byte. The normal epilogue explicitly clears D0 before moving the result byte into D0, proving the 0..255 result domain.

## Source-first correction

The source-first pass begins with the historical `fgets` declaration as a falsifiable hypothesis. Firmware rejects it on every ABI dimension that matters: one slot rather than three, no destination/length access, integer byte/`-1` return rather than pointer/NULL, and a hard comparison against the stream constants. The historical label therefore cannot be retained safely.

Independent correlation then supplies the positive identity. A354 passes A348 as the input-character callback to the common formatted-scanning engine and pairs it with A3B0; A3B0 independently reconstructs as `ungetc`. That get/unget callback pairing, together with A348's stream and return mechanics, confirms `fgetc(FILE *stream)` without relying on library-name adjacency. This is a source-first correction in which primary firmware overrides the historical SDK mapping.

## Independent callback correlation

A later scanning wrapper, A354, passes the A348 runtime address as its input-character callback to the common scan engine. The paired callback is A3B0. Independent mechanical inspection of A3B0 matches `ungetc(int c, FILE *stream)`: it rejects EOF or non-stdin streams, moves the input cursor backward, restores the byte, and returns the character.

The A348/A3B0 get/unget callback pair used by the fscanf-like scanner independently corroborates A348 as `fgetc`, rather than relying only on the standard-library name pattern.

## Caller survey

The corrected PC-index detector exhausted 30 table-bearing official SmartApplets plus 11 structural negatives: **0 executable direct A348 trap callers / 41**. The same detector finds 67 A33C callers, so the negative result is not treated as a detector failure. Direct absolute ROM JSR/JMP references are also zero; A354 integrates A348 indirectly by passing its address as a callback.

## Validation and migration

Private static regression: **OVERALL PASS**. Dynamic execution was not performed.

The SDK should expose index 210 as `fgetc`, not `fgets`. Existing code that called the historical BetaWise `fgets` declaration cannot be made source-compatible by aliasing it to A348 because its argument and return contracts are incompatible; such code must be migrated explicitly.

Private workpapers retain exact ROM hashes, vectors, raw handler hashes, line-buffer mechanics, literal callback xrefs, A3B0 correlation, and the reproducible 41/41 scan.
