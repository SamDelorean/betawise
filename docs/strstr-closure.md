# A3A4 / `strstr` closure

A3A4 (index 233) is reconstructed and independently revalidated source-first as:

```c
char *strstr(const char *haystack, const char *needle);
```

Status: **CLOSED A / SOURCE_FIRST / PUBLISHED**.

## Source-first anchors

Historical and current references were correlated before accepting the previous reconstruction:

- preserved BetaWise `syscall.c` maps index 233 to `strstr`;
- preserved `os3k.h` and `os3k.pdf` declare `char *strstr(const char *str1, const char *str2)`;
- the current `sdk/abi-automation` BetaWise sources retain both index233→`strstr` and the same two-pointer prototype;
- `master` independently retains index233→`strstr`;
- a targeted `debug.pdf` text search did not recover a direct `strstr` occurrence, so no positive source-level evidence is attributed to that document.

These references are treated only as identity/signature anchors. The contract below is based on the canonical firmware and concrete executable callers.

## Fresh primary verification

The three canonical private ROMs were freshly materialized and their complete SHA-256 values recomputed:

- AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

All three passed exactly.

Canonical A3A4 entry points remain:

- AS3000 2005: runtime `0x004DDCA0`, file `+0x01DCA0`
- NEO 2005: runtime `0x005E03BE`, file `+0x0203BE`
- NEO 2013: runtime `0x004367BC`, file `+0x0267BC`

The handler is exactly `0x48` bytes with its unique terminal `RTS` at `+0x46`. Fresh raw handler SHA-256 values are:

- AS3000: `5d13c5e42c32d1e9478dc755ef4f4ad144f50ade79a46a368b89131da264f634`
- NEO 2005: `0ee34148ce168d7c6cba4c5c34c6dc5be98e48882d3ceafec3435b42fe48d66c`
- NEO 2013: `64183aacd5ffd6c13c71ced1a2a88d346a2c286108bec13ac5af364b6530935e`

The only cross-generation changes are the two relocated absolute call operands. Neutralizing those operands gives common SHA-256 `5587602b25c9085b2a43cce4b05eefe7db0e93897142751f65ba78e3d53b2dbe`.

## Handler contract and helper

The handler consumes exactly two 32-bit string pointers. It passes `needle` to the already-closed A390/`strlen`, preserving the resulting 32-bit length. If that length is zero it returns `haystack` immediately.

For a nonempty needle, the handler scans `haystack` one byte at a time until NUL. A first-byte equality gates a call to a private bounded comparator with `(cursor, needle, needle_length)`. Comparator zero returns the current cursor; comparator nonzero advances the scan by one byte. Exhaustion returns zero/NULL. Neither input string is written, and the handler uses no globals or data tables.

The private comparator is `0x3A` bytes and was freshly checked byte-identical in all three ROMs, SHA-256 `494834eaa7a797f952e4b7612b64fe08759217596a56bf8cf12428d045740cb9`. It consumes pointer/pointer/count32, compares zero-extended bytes until count, mismatch or NUL, and returns zero or the byte difference. No vendor symbol is assigned to it.

This mechanics directly confirms:

```c
char *strstr(const char *haystack, const char *needle);
```

Valid readable NUL-terminated strings are caller preconditions; no NULL guard is present in the handler.

## Caller correlation

The previously completed 41/41 official-corpus audit found 11 executable A3A4 calls in exactly three NEO applets: eight in AcceleratedReader, two in KeywordsWireless and one in MathFactsInAFlash. The remaining 27 table-bearing applets and all 11 structural negatives had no executable A3A4 call.

As part of this source-first revalidation, those three positive binaries were freshly materialized, their complete SHA-256 values rechecked, and the A-line detector rerun. It reproduced the exact 11 sites with no drift:

- AcceleratedReader: 8/8 expected sites;
- KeywordsWireless: 2/2 expected sites;
- MathFactsInAFlash: 1/1 expected site.

All positive callers supply two pointer-sized slots. Ten test `D0.L` directly for NULL/non-NULL; the remaining caller performs pointer arithmetic on `D0.L` and moves it to an address register. The pointer/NULL return is therefore independently confirmed by consumers. The prior complete ROM-wide direct JSR/JMP/BSR search remains negative in all three canonical generations.

## Adversarial checks

The historical `strstr` hypothesis was challenged against alternatives:

- `strchr` / `strrchr` are incompatible because the second argument is a pointer and its string length is computed;
- `strncmp` corresponds to the three-argument bounded comparison mechanics, not the outer two-argument pointer-return search;
- copy/concatenation routines are incompatible with the absence of input writes;
- boolean/status-return search is incompatible with callers that use `D0.L` as an address;
- a custom substring-search contract does not explain the observed standard edge semantics better than `strstr`: empty needle returns haystack, success returns the first matching cursor, and failure returns NULL.

No source-versus-firmware contradiction was found. The historical name and prototype survive primary verification unchanged.

## Regression status

The preserved full static regression over all three canonical ROMs and the complete 41-applet accounting is **EXECUTED / OVERALL PASS**. It covers canonical ROM hashes, exact handler lengths/hashes, relocation-normalized identity, exact helper identity, negative direct ROM xrefs, the 11-call/3-applet corpus, return consumption, mechanics and neighboring detector controls.

This source-first pass additionally re-executed the canonical ROM/hash/handler/helper checks and the complete positive-caller set: **PASS** with exact prior fingerprints and site offsets.

Dynamic/emulator-first regression remains **SPECIFIED / NOT EXECUTED**. Minimum cases remain: empty needle; empty haystack with nonempty needle; match at beginning/middle/end; no match; needle longer than remaining suffix; high-bit bytes in the comparator; and repeated first-byte false candidates before the real match.

The existing `os3k.h` declaration already matches the verified ABI; no header or wrapper change is required.
