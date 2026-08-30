# A34C / `fprintf` — ABI closure

Status: **mechanically closed (A) / published contract**.

## Contract

```c
int fprintf(FILE *stream, const char *fmt, ...);
```

## Primary reconstruction

A34C is a 0x28-byte variadic formatting wrapper in AS3000 2005, NEO 2005, and NEO 2013. After normalizing two generation-specific absolute operands, all three handlers have SHA-256 `6487e0fae6e96867b52817d52c2f94133517d86965165d687acac91acef7d6ec`.

At entry the caller stack is `stream`, `fmt`, followed by variadic arguments. The handler saves one scratch register, constructs a pointer to the first variadic slot, recovers the format pointer and stream value while accounting for its own pushes, and pushes A350 as the output-character callback. It then calls a common formatting engine with four longword arguments, removes exactly 0x10 bytes of helper arguments, preserves the helper's D0.L result, restores the saved register, and returns.

A350 is the next OS3K standard-library trap and is historically `_OS3K_fputc`; the absolute callback operand in A34C equals the A350 runtime vector in every canonical ROM. The explicit stream context, format pointer, variadic-area pointer, output callback, and integer helper result independently confirm the historical `fprintf` mapping.

This closure establishes the wrapper ABI. It does not import undocumented details of individual printf-format conversions or error policies from a host libc.

## Caller survey

The corrected PC-index detector exhausted 30 table-bearing official SmartApplets plus 11 structural negatives: **0 executable direct A34C callers / 41**. Caller absence is not used to infer the contract; the wrapper's own stack construction provides that evidence.

## Adversarial conclusions

Fixed-arity formatting is rejected because the handler explicitly constructs a pointer to the caller's variadic area. `sprintf` is rejected because A350 is passed as an output callback and the first caller slot is forwarded as stream context rather than a destination string. Plain `printf` is rejected because the stream argument is explicitly supplied by the caller rather than fixed to stdout.

## Validation

Private static regression: **OVERALL PASS**. Dynamic execution was not performed. Private workpapers retain exact ROM hashes, vectors, raw hashes, stack arithmetic, callback correlation, caller sweep, and reproducible regression evidence.
