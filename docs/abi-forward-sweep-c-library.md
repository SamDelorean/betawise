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
| A35C | `int memcmp(const void *ptr1, const void *ptr2, size_t num)` | mechanical A / published; ordered unsigned-byte comparison | [`memcmp-closure.md`](memcmp-closure.md) |
| A360 | `void *memcpy(void *dst, const void *src, size_t num)` | mechanical A / published; forward bounded copy with explicit destination return | [`memcpy-closure.md`](memcpy-closure.md) |
| A364 | `void *memmove(void *dst, const void *src, size_t num)` | mechanical A / published; forward/backward overlap-safe move | [`memmove-closure.md`](memmove-closure.md) |
| A368 | `void *memset(void *ptr, int value, size_t num)` | mechanical A / published; exact byte fill and explicit destination return | [`memset-closure.md`](memset-closure.md) |
| A36C | `int printf(const char *fmt, ...)` | mechanical A / published; stdout wrapper over the common formatter | [`printf-closure.md`](printf-closure.md) |
| A370 | `int rand(void)` | mechanical A / published; 32-bit LCG and 15-bit result confirmed cross-ROM | [`rand-closure.md`](rand-closure.md) |
| A374 | `int scanf(const char *fmt, ...)` | mechanical A / published; stdin wrapper over the common scan engine | [`scanf-closure.md`](scanf-closure.md) |
| A378 | `int sprintf(char *str, const char *fmt, ...)` | mechanical A / published; string-output wrapper over the common formatter | [`sprintf-closure.md`](sprintf-closure.md) |
| A37C | `void srand(unsigned int seed)` | mechanical A / published; verbatim setter for the A370/`rand` 32-bit state | [`srand-closure.md`](srand-closure.md) |
| A380 | `char *strcat(char *dst, const char *src)` | mechanical A / published; NUL-terminated append with explicit destination return | [`strcat-closure.md`](strcat-closure.md) |
| A384 | `char *strchr(const char *str, int c)` | mechanical A / published; NUL-inclusive byte search with pointer/NULL return | [`strchr-closure.md`](strchr-closure.md) |
| A388 | `int strcmp(const char *str1, const char *str2)` | mechanical A / published; exact unsigned-byte difference return | [`strcmp-closure.md`](strcmp-closure.md) |
| A38C | `char *strcpy(char *dst, const char *src)` | mechanical A / published; NUL-terminated copy with explicit destination return | [`strcpy-closure.md`](strcpy-closure.md) |

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

A35C is also byte-identical across the three canonical ROMs. Its 0x30-byte
handler consumes two pointers and a full 32-bit count, compares bytes in unsigned
order, and returns normalized `-1`, `0`, or `+1`. Five direct ROM callers per
generation and three executable Wireless Update callers independently confirm
the three-slot ABI and consume the full comparison result.

A360's contractual entry point is the first 0x4A bytes of the larger code region
before A364. Those bytes are identical across all three canonical ROMs. The
handler consumes destination, source, and a full 32-bit byte count, copies
forward using aligned longword moves where possible and byte moves otherwise,
and explicitly returns the original destination in D0.L. No overlap test or
backward-copy path exists, distinguishing this service from `memmove`. The
validated official sweep finds 233 callers in 26 table-bearing applets, while
direct firmware JSR counts are 19/22/33 for AS3000 2005 / NEO 2005 / NEO 2013.

A364 is the overlap-safe counterpart of A360. Its byte-identical 0x3C-byte
handler consumes the same three-slot destination/source/count ABI but compares
the pointers and chooses either a forward byte loop or a backward byte loop from
the range ends. It explicitly returns the original destination in D0.L. The
validated official sweep finds 95 callers in 13 table-bearing applets; direct
firmware JSR counts are 27/31/32 across AS3000 2005 / NEO 2005 / NEO 2013.

A368 is a byte-identical 0x28-byte fill primitive across all three canonical
ROMs. It consumes a destination pointer, the low byte of the second 32-bit slot,
and a full 32-bit byte count; `num == 0` performs no store, while nonzero counts
write exactly the requested number of bytes. The original destination is
explicitly returned in D0.L. The complete official sweep finds 100 executable
callers in 21 table-bearing applets, and direct firmware JSR counts are 22/30/34
for AS3000 2005 / NEO 2005 / NEO 2013.

A36C is a thin stdout formatter wrapper. It has one fixed external argument,
forms a pointer to the variadic tail, supplies the OS stream descriptor whose
stored value is `1`, installs A350 / `_OS3K_fputc` as the output callback, and
calls the same common formatter used by A34C / `fprintf`. The formatter's
`D0.L` result is returned unchanged. The validated official sweep finds 99
callers in 13 applets; direct firmware JSR counts are 7/7/11 across AS3000 2005,
NEO 2005 and NEO 2013.

A370 is the standard no-argument PRNG entry. All three canonical ROMs update a
32-bit persistent state with `state = state * 0x41C64E6D + 0x3039` modulo 2^32
and return `(state >> 16) & 0x7FFF`. The adjacent A37C/index223 handler writes
its single 32-bit argument to the same state global, providing independent
mechanical seed-state correlation without relying on the historical symbol map.
The official SmartApplet sweep is 0/41 and direct ROM xrefs are negative; the
same corrected detector finds 99 A36C calls and 598 A378 calls as positive
controls.

A374 is the stdin-specific member of the common scan-wrapper family. It treats
entry `S+4` as `fmt`, forms `&S+8` for the variadic tail, installs the same
A348/fgetc and A3B0 unget callbacks used by A354/fscanf, supplies the stream
descriptor whose stored value is `0`, sets scan mode 0, and calls the exact same
scan engine as A354. A354 differs only by accepting the stream externally and
therefore shifting `fmt`/varargs one slot. A344/sscanf uses string adapters and
mode 1. A374 preserves the engine's D0.L return. The official sweep is 0/41;
positive controls from the same detector are A36C=99 and A378=598.

A378 is the string-output member of the common formatter family. It consumes a
destination string and format pointer, forms `&S+12` for the variadic tail,
initializes the destination to the empty string, and installs a private callback
that appends the low byte of each formatted character, advances the cursor, and
maintains a trailing NUL. No capacity argument or bounds check exists. The
formatter's full D0.L result is preserved. The official sweep finds 598
executable callers in 25 table-bearing applets, and direct firmware JSR counts
are 18/18/35 across AS3000 2005 / NEO 2005 / NEO 2013.

A37C is the seed setter paired with A370. Its 0x0A-byte handler copies the full
longword at entry `SP+4` verbatim into the exact persistent state global used by
`rand`. It does not write `D0`, so preserved incoming register contents are not
interpreted as a return value. A fresh 30-file table-bearing SmartApplet sweep
finds no executable A37C callers, and the eleven remaining official applets are
structural negatives; the same run reproduces A36C=99 and A378=598 exactly as
positive detector controls. Direct firmware JSR/JMP/BSR xrefs are negative in
all three canonical ROMs. The existing independent BetaWise mapping and header
already identify the contract as `void srand(unsigned int seed)`.

A380 is a byte-identical 0x1C-byte NUL-terminated append primitive across the
three canonical ROMs. It consumes two pointer slots, walks the first string to
its terminator, copies the second string including its terminator at that point,
and deliberately returns the original first pointer in `D0.L`. No capacity
argument or bounds check is present. The complete official corpus finds 130
executable callers in 16 applets; 14 table-bearing applets and the 11 applets
without an A-line table are negative. Direct firmware JSR counts are 9/9/9.
`strcpy`, `strncat` and `memcpy` are mechanically incompatible alternatives.

A384 is a byte-identical 0x1A-byte NUL-terminated search primitive across all
three canonical ROMs. It consumes a string pointer and a 32-bit character slot,
compares only the low byte, and performs the character comparison before testing
the current byte for NUL. Consequently `c == 0` returns the terminator address;
a missing nonzero byte returns NULL; and a match returns the current string
pointer in `D0.L`. The complete official corpus finds 24 executable callers in
three NEO applets, with concrete promoted character arguments and pointer/NULL
return consumption. Direct firmware JSR counts are 5/5/11 for AS3000 2005 /
NEO 2005 / NEO 2013. `memchr`, boolean-only search and NUL-excluding search are
mechanically incompatible alternatives.

A388 is a byte-identical 0x24-byte string comparator across all three canonical
ROMs. It advances two pointers while current bytes are equal and nonzero. At the
first mismatch or shared terminator it zero-extends both current bytes and
returns their exact difference in `D0.L`. Therefore the implementation has the
standard `strcmp` zero/sign semantics with unsigned-byte lexical ordering, plus
a reproducible implementation-specific magnitude in the range -255..255. The
complete official corpus finds 244 executable callers in 16 applets. Most use
equality/inequality, while an AcceleratedReader caller consumes the sign with
`BLE`, independently proving an ordering contract. Direct firmware JSR counts
are 7/6/7 for AS3000 2005 / NEO 2005 / NEO 2013. `strncmp`, `memcmp`, a
case-insensitive comparator and a boolean-only predicate are incompatible.

A38C is a byte-identical 0x16-byte NUL-terminated copy primitive across all three
canonical ROMs. It consumes destination and source pointers, copies source bytes
through and including the first NUL byte, deliberately preserves the original
destination and returns it in `D0.L`. No count, capacity, overlap handling,
helper or global state is present. The complete official corpus finds 302
executable callers in 24 table-bearing applets; six table-bearing and 11
structural applets are negative. Direct firmware JSR counts are 25/25/27 for
AS3000 2005 / NEO 2005 / NEO 2013. `memcpy`, `strncpy`, `memmove`, `strcat` and
a void-only copy are mechanically incompatible alternatives.

Private static regressions for every mechanically closed entry in this segment
executed with **OVERALL PASS**. Dynamic emulator-first regression remains
specified but was not executed unless an individual closure says otherwise.
