# A394 / `strncat` closure

A394 (index 229) is reconstructed and revalidated source-first as:

```c
char *strncat(char *dst, const char *src, size_t num);
```

Status: **CLOSED A / SOURCE_FIRST / PUBLISHED**.

## Source-first anchors

References were correlated before reopening the firmware:

- preserved BetaWise `syscall.c` maps index 229 to `strncat`;
- preserved `os3k.h` / `os3k.pdf` declare `char *strncat(char *dst, const char *src, size_t num)`;
- the current public BetaWise header retains that prototype;
- a targeted `debug.pdf` search did not recover a direct `strncat` source-level caller, so no caller evidence is attributed to that source;
- A390/`strlen` was already source-first closed immediately before this block and supplies a mechanically independent anchor for the AlphaQuiz call sequence that derives the third A394 argument from `60 - strlen(...)`.

These sources establish a strong historical hypothesis only. Identity and contract below are taken from a fresh read of the canonical firmware images and concrete callers.

## Fresh primary verification

The same three private canonical ROM files freshly fetched for the preceding A390 audit were used without substitution; their complete SHA-256 values had just been recomputed and passed:

- AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Canonical A394 entry points remain:

- AS3000 2005: runtime `0x004DDC24`, file `+0x01DC24`
- NEO 2005: runtime `0x005E0342`, file `+0x020342`
- NEO 2013: runtime `0x00436740`, file `+0x026740`

A fresh extraction of 0x3E bytes at each entry is byte-identical across all three ROMs, with SHA-256 `008f46227a1084fc0146cca469531528795d78242917a6ff8f8165debb4a67ce`.

The 68k handler saves `D7/A2`, loads `dst`, `src`, and a full 32-bit `num` from the three caller slots, preserves the original destination in `A2`, scans `dst` to its NUL, then copies from `src` while the remaining count gate is nonzero. Copying stops early when the copied source byte is NUL. If the count is exhausted with the last copied byte nonzero, the handler writes a terminating NUL. It finally returns the original `dst` in `D0.L` and restores its saved registers.

This directly confirms `char *strncat(char *dst, const char *src, size_t num)` with 32-bit `size_t`. No helper calls, globals, tables or generation-specific branches exist in the contractual handler.

Valid readable/writable NUL-terminated destination storage, a readable source string, and sufficient destination capacity for the requested append plus terminator are caller preconditions. The firmware does not know the destination object's capacity and therefore cannot enforce it.

## Zero-count implementation quirk

The `num == 0` gate prevents any source copy. However, after locating the destination terminator, the common epilogue still tests byte `-1(A1)`.

For a nonempty destination this rereads its final character and may redundantly clear the already-existing NUL at `(A1)`. For an empty destination, `A1 == dst`, so the test reads `dst[-1]`. The visible destination remains empty, but the read itself is outside the destination object. This is **CONFIRMED implementation behavior** in all three ROMs, not a requirement clients should reproduce or rely on.

## Caller correlation

The previously executed complete official-corpus regression found five executable A394 calls in three applets: AS AlphaWordPlus (1), NEO AlphaWordPlus (1), and NEO AlphaQuiz (3). The other 27 table-bearing applets and all 11 structural negatives have no direct A394 call. Direct absolute firmware JSR counts are 1/1/1, with no direct JMP use found.

AlphaWordPlus computes a dynamic count and pushes count, source, destination in 68k C right-to-left order. AlphaQuiz provides the stronger semantic xref: after A390/`strlen`, it negates the full returned length and uses an effective-address push equivalent to `60 - strlen(...)` before pushing source and destination. That is independently consistent with a remaining-capacity/count argument and inconsistent with a third pointer or formatting operand.

## Adversarial checks

The historical `strncat` hypothesis was challenged against alternatives:

- `strcat` is incompatible because A394 consumes and gates on a third 32-bit count;
- `strncpy` is incompatible because A394 first scans the existing destination to its terminator and appends there;
- `memcpy` / `memmove` are incompatible with the NUL-aware append and terminator logic;
- a formatting function is incompatible with the fixed three-slot ABI and absence of formatter machinery;
- a 16-bit count is incompatible with the full-long load/decrement/test and the caller-derived longword arithmetic;
- returning the end pointer is incompatible with the explicit preservation of original `dst` in `A2` and final `MOVE.L A2,D0`.

No source-versus-firmware contradiction was found. The historical name and prototype survive primary verification unchanged.

## Regression status

The original static regression is **EXECUTED / OVERALL PASS** and covers canonical ROM hashes, exact handler bytes/hash, direct xrefs, the 5-call/3-applet corpus, preceding detector controls, AlphaWordPlus/AlphaQuiz argument patterns, and the exact zero-count gate plus `-1(A1)` epilogue sequence.

For this source-first revalidation the full canonical ROM hashes were freshly recomputed immediately before A394, and the 0x3E A394 handler was freshly re-extracted from each of those same three validated images: **3/3 PASS** with the exact recorded handler hash.

Dynamic/emulator-first regression remains **SPECIFIED / NOT EXECUTED**. A future protected-memory probe should specifically exercise an empty destination with `num == 0` to observe the confirmed static read-before-buffer quirk dynamically; this document does not claim that probe has run.

The existing `os3k.h` prototype already matches the verified ABI and requires no change.
