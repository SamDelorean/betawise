# ABI trace — A39C / index 231

Forward-sweep disposition: **MECÁNICA_CERRADA A / PUBLICADO**.

Reconstructed contract:

```c
char *strncpy(char *dst, const char *src, size_t num);
```

Public closure: [`strncpy-closure.md`](strncpy-closure.md).

Evidence summary: byte-identical 0x3E-byte contractual handler across AS3000 2005, NEO 2005 and NEO 2013; complete official SmartApplet corpus 41/41 with 34 executable calls in 12 applets; direct firmware JSR counts 1/1/8; static regression **OVERALL PASS**. Dynamic emulator-first regression was not executed.

The existing `os3k.h` declaration is ABI-compatible and requires no source edit.

This small additive trace entry intentionally avoids replacing the concurrently maintained cumulative C-library sweep document. It is the atomic public index record for A39C and can be folded into the cumulative document by a later non-destructive reconciliation.
