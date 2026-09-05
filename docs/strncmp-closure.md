# A398 / `strncmp` closure

A398 (index 230) is mechanically reconstructed as:

```c
int strncmp(const char *str1, const char *str2, size_t num);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004DDE36`, file `+0x01DE36`
- NEO 2005: runtime `0x005E0796`, file `+0x020796`
- NEO 2013: runtime `0x00436B94`, file `+0x026B94`

The contractual handler is 0x3A bytes and byte-identical across all three ROMs (SHA-256 `494834eaa7a797f952e4b7612b64fe08759217596a56bf8cf12428d045740cb9`). It consumes two string pointers and a full 32-bit count.

The count gate executes before the first string read. Therefore `num == 0` returns zero without dereferencing either string. For a positive count the handler compares bytes in parallel, stopping on the first mismatch, a NUL byte, or exhaustion of the count. If the count is exhausted after equal bytes, it returns zero without reading the next byte. At a mismatch it zero-extends both current bytes and returns exactly:

```text
(unsigned char)str1[i] - (unsigned char)str2[i]
```

in `D0.L`.

## Caller correlation

The validated official corpus is complete at 41/41. The corrected physical A-line target (`A308 + 0x38`) finds 48 executable A398 calls in eight applets: AS Beamer (7); NEO AcceleratedReader (6), Beamer (7), KeywordsWireless (2), MathFactsInAFlash (6), MathFactsOffline (7), Responder (7), and WirelessFileTransfer (6). The other 22 table-bearing applets and all 11 structural negatives have no A398 call.

Representative Beamer callers supply bounded counts, push the count and two pointers, invoke A398, remove exactly 12 bytes and test the full `D0.L` result. Fresh source-first revalidation observed immediate counts including 3 and 6. Other observed callers use fixed and dynamic counts. The observed applet callsites mainly use equality/inequality; the ordered unsigned-byte result is established directly by the handler arithmetic.

Direct absolute firmware JSR counts are 2/2/2 for AS3000 2005 / NEO 2005 / NEO 2013; no direct JMP use was found.

## Adversarial alternatives

- `strcmp` is rejected because A398 consumes and enforces a third count argument.
- `memcmp` is rejected because comparison stops at NUL.
- A boolean-only comparator is rejected because the mismatch path explicitly returns a byte difference.
- A signed-char comparator is rejected because both bytes are zero-extended before subtraction.

The historical BetaWise index-230 `strncmp` mapping and existing `os3k.h` declaration are secondary corroboration only. `size_t` is 32 bits on this ABI.

## Validation status

Static regression executed with **OVERALL PASS**. It validates canonical ROM hashes, exact handler bytes/hash, 2/2/2 firmware JSR counts, the 48-call/8-applet corpus, stable preceding detector controls, representative Beamer callers with 12-byte cleanup and full-long result use, the zero-count gate before the first string read, and the explicit unsigned-byte subtraction pattern.

Dynamic emulator-first regression was not executed for this mechanically determined entry.

## SOURCE-FIRST re-audit — 2026-09-04

The current audit began from independent API and historical anchors rather than the firmware label. BetaWise master and the preserved BetaWise 0.2 variant both map index 230 to `strncmp` and declare `int strncmp(const char *str1, const char *str2, size_t num)`; `os3k.pdf` preserves the same declaration. The public `ioma8/neo-re` tree independently reproduces both the `os3k.h` prototype and `DEFINE_SYSCALL(230, strncmp)` mapping. `debug.pdf` and the searched `alphasmart-research` tree supplied no direct `strncmp` occurrence; these are contextual negative searches, not contradictory evidence.

After those anchors were established, all three canonical ROMs were re-read in the current pass. Their full SHA-256 values matched the canonical manifest, the A398 handlers re-extracted at the offsets above, and all three were again exactly 0x3A bytes with the same handler SHA-256. The primary dataflow independently confirms the complete contract: three 32-bit slots, a count gate before any string dereference, NUL-sensitive bounded comparison, and an unsigned-byte difference in `D0.L` at mismatch. No helper, global-state dependency, wrapper-specific side effect, or generational divergence was found.

The official SmartApplet caller corpus was also re-materialized and rescanned in this pass rather than inherited from the earlier closure. All 30 table-bearing applets contained the expected C-library sequence exactly once; A398 reproduced exactly 48 executable calls in eight applets, plus 22 table-bearing negatives and 11 structural negatives. Stable same-run detector controls reproduced A36C=99, A378=598, A380=130, A384=24, A388=244 and A390=477. All seven AS Beamer A398 callsites clean 12 argument bytes and test `D0.L` immediately after the call.

One regression assertion initially searched for a previously described `MOVEQ #3` encoding at a representative caller. Fresh caller bytes instead showed immediate argument construction using `PEA`, including values 3 and 6. The harness was corrected to assert the actual encoding; this was a test-specification correction, not a firmware or ABI contradiction. The corrected SOURCE-FIRST static regression executed with **OVERALL PASS**. Dynamic/emulator-first regression remains **SPECIFIED / NOT EXECUTED**.

Classification: **CONFIRMED / CLOSED A / SOURCE_FIRST / PUBLISHED**. No source-versus-firmware contradiction remains, and A39C/index231 is released as the next sequential audit block.
