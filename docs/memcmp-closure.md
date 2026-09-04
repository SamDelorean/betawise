# A35C / index 215 — `memcmp`

Status: **mechanically closed (confidence A)**.

Reconstructed contract:

```c
int memcmp(const void *ptr1, const void *ptr2, size_t num);
```

## Source-first re-audit

The 2026-09-04 source-first pass began from the preserved SDK layer: `os3k/syscall.c`
maps index 215 to `memcmp`, and `os3k/os3k.h` carries the same two-pointer/full-count
prototype. Those references were treated as hypotheses rather than proof. The archived
firmware reconstruction and callers independently reconfirm the three-slot ABI,
full-width count, unsigned-byte ordering, and full `D0.L` comparison result. No ABI
correction was required.

The A35C handler is byte-identical in the AS3000 2005, NEO 2005, and NEO 2013 canonical ROMs. It consumes exactly three 32-bit slots: two pointers and a full-width count. It compares bytes sequentially over the bounded range and returns a deliberately defined full `D0.L` result of `-1`, `0`, or `+1`.

The ordering is unsigned-byte ordering. The implementation normalizes the nonzero result to sign rather than returning the arithmetic byte difference; this is compatible with the C `memcmp` contract, which specifies only the sign of a nonzero result. A zero count returns zero without requiring a byte comparison.

Independent call evidence closes the ABI. The canonical ROMs contain five direct callers per generation; the observed call groups construct three slots, clean 12 bytes, and immediately test `D0.L`. The official 41-SmartApplet sweep finds three executable A35C callers, all in the NEO Wireless Update applet, with observed counts 5, 5, and `0x800`; each consumes the comparison result. The remaining official corpus is negative under the validated PC-index detector.

Adversarial checks reject `strcmp`/`strncmp` (there is no NUL termination rule), an equality-only predicate (the handler emits ordered `-1/+1`), a 16-bit count (the third slot is loaded and used as a longword), and a signed-byte comparison. The historical BetaWise index mapping to `memcmp` is retained only as corroboration; the contract above is supported by ROM mechanics and callers.

Private workpapers retain ROM hashes, exact runtime/file offsets, three separate 68k listings, the caller manifest, correlated analysis, and the reproducible static regression. Static regression executed **OVERALL PASS**. Dynamic/emulator-first regression remains specified but was not executed.
