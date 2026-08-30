# A390 / `strlen` closure

A390 (index 228) is mechanically reconstructed as:

```c
size_t strlen(const char *str);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004DDE24`, file `+0x01DE24`
- NEO 2005: runtime `0x005E0784`, file `+0x020784`
- NEO 2013: runtime `0x00436B82`, file `+0x026B82`

The contractual handler is 0x12 bytes and byte-identical across all three ROMs (SHA-256 `46193c2eda4c7b66d5a1fd34b8e73773c52f87f49d0e60439c29c9daa95798f5`). It consumes one string pointer, preserves the starting address, advances once per byte through the terminating NUL, and returns `cursor_after_nul - start - 1` in `D0.L`.

An empty string therefore returns zero. The handler has no global state, helper calls, tables or memory writes.

## Caller correlation

The validated official corpus is complete at 41/41. The corrected physical A-line target (`A308 + 0x34`, after omission of A30C–A328) finds 477 executable A390 calls in 26 table-bearing applets. Four table-bearing applets are negative (AS KeyWords; NEO keywords, Text2SpeechUpdate and WirelessUpdate), and all 11 structural negatives lack the table.

The same detector reproduces the established controls A36C=99, A378=598, A380=130, A384=24, A388=244 and A38C=302. Representative callers remove one 32-bit argument slot and then consume `D0.L` with full-long operations such as `TST.L D0` and `MOVE.L D0,D6`, independently confirming the 32-bit length result.

Direct absolute firmware JSR counts are 36/37/53 for AS3000 2005 / NEO 2005 / NEO 2013; no direct JMP entry use was found.

## Adversarial alternatives

- A character search is rejected because there is no second character argument and the result is arithmetic length, not an address.
- `strnlen` is rejected because there is no count argument or bound.
- A void scanner is rejected because the handler explicitly computes `D0.L` and callers consume it.
- A 16-bit length contract is rejected by both the handler arithmetic and full-long caller consumption.

The historical BetaWise index-228 `strlen` mapping and existing `os3k.h` declaration are secondary corroboration only. `size_t` is 32 bits on this ABI.

## Validation status

Static regression over the canonical ROMs and official SmartApplet manifest executed with **OVERALL PASS**. The first run intentionally failed closed because one positive-control offset was mis-entered as `+0x24` for A36C; the physically compacted table places A36C at `+0x22` and A370 at `+0x24`. After correcting the test input, a fresh run validated ROM hashes, exact handler bytes/hash, direct xrefs, the 477-call/26-applet corpus, all positive controls and full-long return consumers.

Dynamic emulator-first regression was not executed for this mechanically determined entry.

Status: **MECÁNICA_CERRADA A / PUBLICADO**. The existing `os3k.h` declaration already matches the reconstructed contract, so no header modification is required.
