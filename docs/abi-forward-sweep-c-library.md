# OS3K forward-sweep traceability — C-library segment

This public supplement continues the forward ABI sweep after A33C without
publishing firmware bytes or extensive disassembly. Detailed primary evidence
remains private in Drive.

| Trap | Reconstructed contract / disposition | Status | Public evidence |
| --- | --- | --- | --- |
| A33C | `int atoi(const char *str)` | mechanical A / published | [`atoi-closure.md`](atoi-closure.md) |
| A340 | `long atol(const char *str)` (`long` = 32 bits on this ABI) | mechanical A / published | [`atol-closure.md`](atol-closure.md) |
| A344 | `int sscanf(const char *str, const char *fmt, ...)` | mechanical A / published | [`sscanf-closure.md`](sscanf-closure.md) |
| A348 | `int fgetc(FILE *stream)` | mechanical A / published; historical `fgets` mapping corrected as ABI-incompatible | [`fgetc-closure.md`](fgetc-closure.md) |
| A34C | `int fprintf(FILE *stream, const char *fmt, ...)` | mechanical A / published | [`fprintf-closure.md`](fprintf-closure.md) |
| A350 | `int _OS3K_fputc(int c, FILE *stream)` | mechanical A / published raw trap; public `fputc()` remains a separate BetaWise wrapper | [`fputc-closure.md`](fputc-closure.md), `../os3k/sys_a350.h` |
| A354 | `int fscanf(FILE *stream, const char *fmt, ...)` | mechanical A / published; same scan engine as A344 with stream adapters | [`fscanf-closure.md`](fscanf-closure.md) |
| A358 | `void *memchr(const void *ptr, int value, size_t num)` | mechanical A / published; bounded byte scan confirmed independently | [`memchr-closure.md`](memchr-closure.md) |

## Evidence discipline

A33C and A340 are mechanically identical decimal signed parsers; their nominal
`atoi`/`atol` distinction is retained from the independently preserved SDK
mapping rather than invented from firmware differences. A348 is a material ABI
correction: the firmware consumes one stream slot and returns `-1` or a byte
value, so the old three-argument pointer-return `fgets` declaration cannot be
retained as an alias.

A350 is the raw output-character service at index 212. Its handler consumes two
32-bit slots, returns the original full character value on success and `-1` on
failure, is called directly from A334, and is used as the output callback by
A34C's formatter wrapper. The official SmartApplet sweep is a validated negative
0/41 for direct A350 trap callers; OS-internal use supplies the positive ABI
correlation.

A354 is the stream-input counterpart of the already closed A344 scan wrapper.
Its 0x30-byte handler forms the first-vararg pointer, forwards the format slot,
installs A348 and A3B0 as stream adapters, passes the stream context with mode 0,
and calls the exact same common scan engine used by A344/`sscanf` with its
string adapters and mode 1. The engine's full D0 result is returned unchanged.
The official SmartApplet sweep is a validated negative 0/41; the same detector
reproduces the canonical A330 and A33C positive controls.

A358 is byte-identical across all three canonical ROMs. Its 0x26-byte handler
uses three physical slots, compares the low byte of the second against each byte
in the half-open range `[ptr, ptr + num)`, and returns the first matching address
or NULL. Four official NEO callers independently confirm the three-slot ABI and
consume D0.L as a pointer/NULL result.

Private static regressions for every mechanically closed entry in this segment
executed with **OVERALL PASS**. Dynamic emulator-first regression remains
specified but was not executed unless an individual closure says otherwise.
