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

Representative Beamer callers prepare a fixed count of 3, push the count and two pointers, invoke A398, remove exactly 12 bytes and test the full `D0.L` result. Other observed callers use fixed counts such as 3 or 4 and dynamic counts. The observed applet callsites mainly use equality/inequality; the ordered unsigned-byte result is established directly by the handler arithmetic.

Direct absolute firmware JSR counts are 2/2/2 for AS3000 2005 / NEO 2005 / NEO 2013; no direct JMP use was found.

## Adversarial alternatives

- `strcmp` is rejected because A398 consumes and enforces a third count argument.
- `memcmp` is rejected because comparison stops at NUL.
- A boolean-only comparator is rejected because the mismatch path explicitly returns a byte difference.
- A signed-char comparator is rejected because both bytes are zero-extended before subtraction.

The historical BetaWise index-230 `strncmp` mapping and existing `os3k.h` declaration are secondary corroboration only. `size_t` is 32 bits on this ABI.

## Validation status

Static regression executed with **OVERALL PASS**. It validates canonical ROM hashes, exact handler bytes/hash, 2/2/2 firmware JSR counts, the 48-call/8-applet corpus, all preceding detector controls, a fixed-count Beamer caller with 12-byte cleanup and full-long result use, the zero-count gate before the first string read, and the explicit unsigned-byte subtraction pattern.

Dynamic emulator-first regression was not executed for this mechanically determined entry.

Status: **MECÁNICA_CERRADA A / PUBLICADO**. The existing `os3k.h` prototype is ABI-compatible and requires no change.
