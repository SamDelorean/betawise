# A3A4 / `strstr` closure

A3A4 (index 233) is mechanically reconstructed as:

```c
char *strstr(const char *haystack, const char *needle);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004DDCA0`, file `+0x01DCA0`
- NEO 2005: runtime `0x005E03BE`, file `+0x0203BE`
- NEO 2013: runtime `0x004367BC`, file `+0x0267BC`

The handler is 0x48 bytes in all three ROMs. The raw images differ only in the two absolute JSR operands; after neutralizing those relocations the common SHA-256 is `5587602b25c9085b2a43cce4b05eefe7db0e93897142751f65ba78e3d53b2dbe`.

It consumes two string pointers. The second string is passed to the already reconstructed A390/`strlen`. An empty second string returns the first pointer immediately. Otherwise the handler scans the first NUL-terminated string. When the current byte equals the first byte of the second string, it calls a private bounded comparator with `(cursor, needle, strlen(needle))`; comparator zero returns the current cursor, while a mismatch advances by one byte. Exhaustion returns NULL. The handler does not modify either string and uses no globals or data tables.

The private comparator is 0x3A bytes and byte-identical across AS3000 2005, NEO 2005 and NEO 2013 (SHA-256 `494834eaa7a797f952e4b7612b64fe08759217596a56bf8cf12428d045740cb9`). It consumes two pointers plus a 32-bit count and compares zero-extended bytes until count, mismatch or NUL, returning zero or the byte difference. No vendor symbol is assigned to that helper.

## Caller correlation

The validated official corpus is complete at 41/41. The physical A-line target (`A378 + 0x16`) finds 11 executable A3A4 calls in three NEO applets: eight in AcceleratedReader, two in KeywordsWireless and one in MathFactsInAFlash. The other 27 table-bearing applets and all 11 structural negatives have no executable A3A4 call.

All positive sites supply two pointer-sized slots. Ten immediately test `D0.L` for NULL/non-NULL; the remaining caller performs pointer arithmetic on `D0.L` and moves it to an address register. This independently confirms the pointer/NULL result contract. No direct firmware JSR/JMP/BSR to A3A4 was found in any canonical ROM.

## Adversarial alternatives

- `strchr`/`strrchr` are rejected because the second external argument is a pointer and the handler computes its string length.
- `strncmp` is the three-argument private comparator used by the search, not the outer two-argument pointer-return service.
- Copy/concatenation routines are rejected because A3A4 does not write either input.
- A generic substring-search label adds no explanatory power beyond the exact standard `strstr` edge behavior: empty needle returns haystack, successful search returns the first matching cursor and failure returns NULL.

The historical BetaWise `strstr` declaration is secondary corroboration only. The contract follows from the ROM mechanics and official callers.

## Validation status

Static regression over all three canonical ROMs and the complete official SmartApplet corpus executed with **OVERALL PASS**. It validates canonical hashes, exact handler lengths and normalized identity, exact helper identity, negative direct ROM xrefs, the exact 11-call/3-applet corpus, pointer-return consumption, and neighboring detector controls A388=244, A38C=302, A390=477 and A3A0=2.

Dynamic emulator-first regression is specified for empty needle, empty haystack, beginning/middle/end matches, no match and repeated first-byte false candidates, but was not executed for this mechanically determined entry.

Status: **MECÁNICA_CERRADA A / PUBLICADO**. The existing `os3k.h` declaration already matches the reconstructed contract, so no header change is required.
