# A384 / `strchr` closure

A384 (index 225) is mechanically reconstructed as:

```c
char *strchr(const char *str, int c);
```

## Primary mechanics

The canonical AS3000 2005, NEO 2005 and NEO 2013 ROMs resolve A384 to:

- AS3000 2005: runtime `0x004DE7AC`, file `+0x01E7AC`
- NEO 2005: runtime `0x005E110C`, file `+0x02110C`
- NEO 2013: runtime `0x0043750A`, file `+0x02750A`

The contractual handler is 0x1A bytes and byte-identical in all three ROMs (SHA-256 `3633a5c6151a9bbcfed16b7a1159646302b89e1f3cc7a10fd9b96ded665292f6`). It consumes a string pointer and one 32-bit argument slot whose low byte is compared against successive string bytes.

The comparison occurs before the terminator test. Therefore a search for `c == 0` returns the address of the terminating NUL, while a nonzero byte absent from the string returns NULL. A match returns the address of the matching byte in `D0.L`.

There are no global accesses, helper calls, tables, string writes or other side effects in the handler.

## Caller correlation

The validated official corpus is complete at 41/41. The corrected physical A-line table target (`A308 + 0x2E`, after omission of A30C–A328) finds 24 executable A384 calls in three NEO applets: AcceleratedReader (1), MathFactsInAFlash (14) and MathFactsOffline (9). The other 27 table-bearing applets and all 11 structural negatives have no A384 call.

The same detector reproduces A36C=99, A378=598 and A380=130 as positive controls. Representative callers explicitly supply concrete character values such as `0x22`, `0x20`, `0x25`, `0x2F` and `0x42`, remove two 32-bit argument slots, and consume `D0.L` as a pointer/NULL result. For example, AcceleratedReader pushes `0x22` and a string pointer and immediately moves returned `D0` to an address register; a MathFacts caller pushes `0x2F` and a string pointer, cleans 8 bytes, moves `D0` to A4 and tests it for NULL.

Direct absolute firmware JSR counts are 5/5/11 for AS3000 2005 / NEO 2005 / NEO 2013; no direct JMP entry use was found. Representative firmware callers likewise pass a byte promoted through a 32-bit slot plus a string pointer and test the returned pointer.

## Adversarial alternatives

- `memchr` is rejected because there is no count argument and scanning terminates at NUL.
- A search primitive that cannot find the terminating NUL is rejected by the compare-before-terminator-test control flow.
- A boolean search primitive is rejected because callers consume `D0.L` as an address and the handler returns the current string pointer on match.

The historical BetaWise index-225 `strchr` mapping and the existing `os3k.h` declaration are corroborative rather than the basis of the identification.

## Validation status

Static regression over the canonical ROMs and official SmartApplet manifest executed with **OVERALL PASS**. It revalidates ROM hashes, exact handler bytes/hash, direct JSR/JMP counts, the A384 corpus total (24), the three positive applets, detector controls, and representative caller argument/return patterns. Dynamic emulator-first regression was not executed for this mechanically trivial entry.

Status: **MECÁNICA_CERRADA A / PUBLICADO**. The existing `os3k.h` declaration already matches the reconstructed two-argument contract, so no header modification is required.

## SOURCE-FIRST re-audit — 2026-09-04

The sequential re-audit started from the preserved API anchors: `os3k.h`/`os3k.pdf` declare `char *strchr(const char *str, int c)`, and the generated syscall mapping places `strchr` at index225/A384. Those references were treated only as hypotheses. The archived official-corpus workpaper supplied caller anchors with concrete promoted character values and explicit pointer/NULL consumption, plus positive controls for the compact table layout.

Primary firmware was independently re-extracted from all three canonical ROMs. Vector[225] again resolves to the documented entries, and all three 0x1A handlers reproduce byte-identically with SHA-256 `3633a5c6151a9bbcfed16b7a1159646302b89e1f3cc7a10fd9b96ded665292f6`. Dataflow confirms a string pointer plus a 32-bit argument slot whose low byte alone is compared, pointer return on match, NULL on exhaustion, and compare-before-NUL-test ordering; consequently `c == 0` returns the terminator address exactly as standard `strchr` requires.

The adversarial pass rejects `memchr`, boolean-search and terminator-excluding variants. The canonical caller workpaper remains 24 executable calls in three NEO applets, with explicit two-slot cleanup and `D0.L` consumed as a pointer/NULL; controls A36C=99, A378=598 and A380=130 remain consistent. No source/firmware contradiction or generational divergence was found.

Classification after re-audit: **CLOSED A / SOURCE_FIRST / PUBLISHED**. Previously executed static regression remains **OVERALL PASS**; dynamic/emulator-first regression remains **SPECIFIED / NOT EXECUTED**.
