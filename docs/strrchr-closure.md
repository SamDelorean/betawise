# A3A0 / `strrchr` closure

A3A0 (index 232) is mechanically reconstructed as:

```c
char *strrchr(const char *str, int c);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004E722A`, file `+0x02722A`
- NEO 2005: runtime `0x005E9572`, file `+0x029572`
- NEO 2013: runtime `0x00444446`, file `+0x034446`

The contractual handler is 0x1A bytes and byte-identical across all three ROMs (SHA-256 `d894d16615ad73b1c65943ff49774f59a4c003be341726fb2159551636fa22eb`). It consumes a string pointer and a 32-bit character slot, comparing only the low byte.

The result pointer starts as NULL. For every byte in the NUL-terminated string, the handler compares the current byte with `c`; on equality it replaces the saved result with the current address but continues scanning. The same current byte is then tested for NUL and the cursor advances. Consequently the final result is the address of the last occurrence or NULL. Because the comparison precedes the NUL test, `c == 0` returns the address of the terminating NUL.

The saved pointer is returned in `D0.L`. The handler has no globals, helper calls, tables or memory writes.

## Caller correlation

The validated official corpus is complete at 41/41. The corrected physical A-line target (`A308 + 0x3C`, equivalently `A378 + 0x14`) finds exactly two executable calls, both in the official NEO `alphaquiz` applet; the remaining 29 table-bearing applets and all 11 structural negatives have no executable A3A0 call.

Both callsites push `c = 0x17` followed by the string pointer, then clean exactly eight bytes. One caller immediately moves `D0.L` into an address register and writes through that returned pointer; the other immediately executes `TST.L D0`. These sites independently confirm the two-slot ABI and pointer/NULL return contract.

No direct absolute JSR/JMP or PC-relative BSR to A3A0 was found in any of the three canonical ROMs.

## Adversarial alternatives

- `strchr` is rejected because a match does not terminate the scan; later matches replace the saved result.
- `memrchr` is rejected because there is no count argument and scanning terminates on the string NUL.
- A boolean last-match predicate is rejected because official callers consume `D0.L` as an address/NULL value.

The historical BetaWise index-232 `strrchr` mapping is secondary corroboration only; the contract above follows from firmware mechanics and official callers.

## Validation status

Static regression over the three canonical ROMs and the complete official SmartApplet corpus executed with **OVERALL PASS**. It validates ROM hashes, exact handler bytes/hash, negative direct firmware xrefs, the 2-call/1-applet/41 corpus, the two-slot caller ABI, return-pointer consumption, and the last-match control-flow pattern.

Dynamic emulator-first regression is specified for no-match, multiple-match, and `c == 0` cases but was not executed for this mechanically determined entry.

## SOURCE-FIRST re-audit — 2026-09-04

The re-audit first established independent API anchors. BetaWise master maps index 232 to `strrchr`; the public `ioma8/neo-re` snapshot independently preserves both `DEFINE_SYSCALL(232, strrchr)` and the matching `char *strrchr(const char *str, int c)` declaration. No stronger competing identity was recovered from the auxiliary source searches, so these labels were retained only as hypotheses until firmware revalidation.

The three canonical ROMs were then re-read in the current pass. Full ROM hashes matched the canonical manifest; A3A0 re-extracted at the offsets above as the same exact 0x1A-byte handler with SHA-256 `d894d16615ad73b1c65943ff49774f59a4c003be341726fb2159551636fa22eb`. Primary control flow independently confirms a NULL-initialized saved result, low-byte comparison, replacement of that result on every match without early return, continued scanning through the terminating NUL, and final pointer/NULL return in `D0.L`. This also proves the `c == 0` terminator case. No helpers, globals, tables, writes, or generational differences were found.

The fresh 30-binary SmartApplet materialization from the immediately preceding audits was rescanned for physical slot A3A0. All 30 table-bearing applets contained the expected C-library sequence exactly once. The result reproduced exactly two executable A3A0 calls, both in NEO `alphaquiz`, plus 29 table-bearing negatives and 11 structural negatives. Both callsites pass `c = 0x17`, clean eight argument bytes, and independently consume `D0.L` as a pointer/NULL value. Direct firmware JSR/JMP searches remain zero in all three ROMs.

A fresh SOURCE-FIRST static regression covering ROM hashes, exact handler bytes/hash, direct xrefs, corpus coverage, both caller patterns and last-match mechanics executed with **OVERALL PASS**. Dynamic/emulator-first regression remains **SPECIFIED / NOT EXECUTED**.

Classification: **CONFIRMED / CLOSED A / SOURCE_FIRST / PUBLISHED**. A3A4/index233 is released as the next sequential audit block.
