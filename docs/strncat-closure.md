# A394 / `strncat` closure

A394 (index 229) is mechanically reconstructed as:

```c
char *strncat(char *dst, const char *src, size_t num);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004DDC24`, file `+0x01DC24`
- NEO 2005: runtime `0x005E0342`, file `+0x020342`
- NEO 2013: runtime `0x00436740`, file `+0x026740`

The contractual handler is 0x3E bytes and byte-identical across all three ROMs (SHA-256 `008f46227a1084fc0146cca469531528795d78242917a6ff8f8165debb4a67ce`). It consumes destination, source and a full 32-bit count. It finds the destination terminator, copies at most `num` source bytes, stops early when a source NUL is copied, adds a NUL if the count is exhausted while the last copied byte is nonzero, and returns the original destination pointer in `D0.L`.

`size_t` is 32 bits on this ABI. There are no helper calls or global-state dependencies.

## Implementation quirk for `num == 0`

The zero-count gate prevents any source copy, so the visible result remains the unchanged destination as required by the normal `strncat` contract. However, the common epilogue still executes a byte test at `-1(A1)` after A1 has been positioned on the destination terminator.

For a nonempty destination this merely rereads its last character and may redundantly rewrite the already-present terminator. For an empty destination, A1 equals `dst`, so the handler reads `dst[-1]` before returning the unchanged destination. This is a potential one-byte read before the destination object. It is documented as an implementation quirk/bug, not as desirable API behavior.

## Caller correlation

The validated official corpus is complete at 41/41. The corrected physical A-line target (`A308 + 0x36`) finds five executable A394 calls in three applets: AS AlphaWordPlus (1), NEO AlphaWordPlus (1) and NEO AlphaQuiz (3). The other 27 table-bearing applets and all 11 structural negatives have no direct A394 call.

AlphaWordPlus computes a dynamic count, pushes that count first, then the source and destination pointers, matching the three-argument 68k C convention. AlphaQuiz has an especially strong pattern: after a preceding A390/`strlen` call it negates the returned length and uses `PEA 60(A0)` as a compact arithmetic push, producing a `60 - strlen(...)` count before pushing source and destination. This independently identifies the third slot as a remaining-capacity count rather than another pointer.

Direct absolute firmware JSR counts are 1/1/1 for AS3000 2005 / NEO 2005 / NEO 2013; no direct JMP use was found.

## Adversarial alternatives

- `strcat` is rejected because A394 consumes and enforces a third count argument.
- `strncpy` is rejected because A394 first scans the existing destination to its terminator and appends there.
- `memcpy`/`memmove` are rejected by the NUL-aware append semantics.
- Formatting functions are rejected by the fixed three-slot ABI and lack of formatter machinery.

The historical BetaWise index-229 `strncat` mapping and existing header declaration are secondary corroboration only.

## Validation status

Static regression executed with **OVERALL PASS**. It validates canonical ROM hashes, exact handler bytes/hash, 1/1/1 firmware JSR counts, the 5-call/3-applet official corpus, all preceding detector controls, the AlphaWordPlus and AlphaQuiz argument patterns, and the exact opcode sequence responsible for the zero-count gate and `-1(A1)` epilogue read.

Dynamic emulator-first regression was not executed. A future protected-memory probe should specifically exercise `dst=""` with `num==0` to observe the documented read-before-buffer behavior.

Status: **MECÁNICA_CERRADA A / PUBLICADO**, with the zero-count implementation quirk explicitly documented. The existing `os3k.h` prototype is ABI-compatible and requires no change.
