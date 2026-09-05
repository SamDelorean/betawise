# A388 / `strcmp` closure

A388 (index 226) is mechanically reconstructed as:

```c
int strcmp(const char *str1, const char *str2);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004DDBEA`, file `+0x01DBEA`
- NEO 2005: runtime `0x005E0308`, file `+0x020308`
- NEO 2013: runtime `0x00436706`, file `+0x026706`

The contractual handler is 0x24 bytes and byte-identical across all three ROMs (SHA-256 `d63a8976fa75cf70c8ae00da5359e87e705ea0b5503d05952c464cea21b15555`). It walks the two strings in parallel while bytes are equal and the current byte is nonzero. At the first difference or common terminator it zero-extends both current bytes and returns exactly:

```text
(unsigned char)str1[i] - (unsigned char)str2[i]
```

in `D0.L`. Thus equality returns zero and the sign gives unsigned-byte lexicographic ordering. The exact magnitude is an implementation property; callers should rely on the standard `strcmp` sign/zero contract unless reproducing firmware behavior intentionally.

There are no globals, helper calls, tables or writes through either string pointer.

## Caller correlation

The validated 41-applet official corpus finds 244 executable A388 calls in 16 table-bearing applets; 14 table-bearing applets and all 11 structural negatives are negative. The same detector reproduces A36C=99, A378=598, A380=130 and A384=24.

Representative callers remove two 32-bit pointer slots and test `D0.L`. Most use equality/inequality, but at least one AcceleratedReader caller uses `TST.L D0` followed by `BLE`, independently confirming that ordering/sign is contractual rather than the routine being merely a boolean equality test.

Direct absolute firmware JSR counts are 7/6/7 for AS3000 2005 / NEO 2005 / NEO 2013; no direct JMP entry use was found.

## Adversarial alternatives

- `strncmp` is rejected because there is no count argument or bound.
- `memcmp` is rejected because scanning stops at NUL and has no count argument.
- A case-insensitive comparator is rejected because there is no character folding or translation.
- A boolean equality predicate is rejected by the arithmetic return and the ordering consumer.

The historical BetaWise index-226 `strcmp` mapping and existing header declaration are secondary corroboration only.

## Validation status

Static regression over the canonical ROMs and official SmartApplet manifest executed with **OVERALL PASS**. It validates exact handler bytes/hash, direct xrefs, the 244-call/16-applet corpus, detector controls, equality consumers and an ordering consumer. Dynamic emulator-first regression was not executed for this mechanically determined entry.

Status: **MECÁNICA_CERRADA A / PUBLICADO**. The existing `os3k.h` declaration is ABI-compatible and requires no change.

## SOURCE-FIRST re-audit — 2026-09-04

The sequential re-audit started from the preserved API layer: `os3k.h`/`os3k.pdf` declare `int strcmp(const char *str1, const char *str2)`, and the generated syscall mapping assigns index226/A388 to `strcmp`. These were treated as hypotheses. The archived official-corpus workpaper provided independent caller anchors, including equality consumers and an ordering consumer using the sign of `D0.L`.

Primary firmware was independently re-extracted from the three canonical ROMs. Vector[226] again resolves to the documented entries, and each 0x24 handler reproduces byte-identically with SHA-256 `d63a8976fa75cf70c8ae00da5359e87e705ea0b5503d05952c464cea21b15555`. The dataflow walks both strings until mismatch or NUL, zero-extends the decisive bytes, and returns their exact subtraction in `D0.L`, establishing unsigned-byte lexicographic sign semantics and the implementation-specific magnitude range -255..255.

The adversarial pass rejects `strncmp`, `memcmp`, case-insensitive comparison and boolean equality. The canonical caller workpaper remains 244 executable calls in 16 applets; positive controls A36C=99, A378=598, A380=130 and A384=24 remain consistent, and at least one caller consumes ordering/sign rather than equality alone. No source/firmware contradiction or generational divergence was found.

Classification after re-audit: **CLOSED A / SOURCE_FIRST / PUBLISHED**. Previously executed static regression remains **OVERALL PASS**; dynamic/emulator-first regression remains **SPECIFIED / NOT EXECUTED**.
