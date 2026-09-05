# A378 / `sprintf` closure

Status: **mechanical A / published**.

Reconstructed contract:

```c
int sprintf(char *str, const char *fmt, ...);
```

The three canonical OS3K ROMs resolve A378 to AS3000 2005 `0x004DD43A` (`file+0x01D43A`), NEO 2005 `0x005DFB58` (`file+0x01FB58`), and NEO 2013 `0x00435F56` (`file+0x025F56`). The wrapper is 0x2C bytes, contains 13 instructions, and terminates with `RTS` at +0x2A. Raw hashes differ only at the formatter and callback relocation operands; after neutralizing those fields all three normalize to SHA-256 `fb7e4124ee4a57e6b360fb2432ffad46059208522aa9488e8ce783e744e930d8`.

Primary stack/dataflow reconstructs two fixed pointer arguments followed by a variadic tail: `str` at `S+4`, `fmt` at `S+8`, and the first variadic slot at `S+12`. The wrapper first writes an empty terminator to `str[0]`, forms a pointer to the variadic tail, passes `fmt`, maintains a cursor initialized from `str`, installs a string-output callback, and calls the same common formatter used by A34C / `fprintf` and A36C / `printf`. The formatter's full `D0.L` result is returned unchanged.

The string callback is byte-identical across the three ROMs and is 0x18 bytes long. It writes the low byte of the supplied character through the current cursor, advances the cursor, and writes a new NUL terminator. There is no capacity argument or bounds check in either the wrapper or callback.

The corrected PC-index detector was run over the complete official 41-SmartApplet corpus. It finds 598 executable A378 callers in 25 table-bearing applets; five additional table-bearing applets are negative and eleven applets are structural negatives without the relevant A-line table. Representative callers consume `D0.L` as the scalar formatter result. Direct firmware absolute-JSR counts are 18 / 18 / 35 for AS3000 2005 / NEO 2005 / NEO 2013.

Adversarial alternatives are mechanically excluded. A378 is not `printf` or `fprintf`, because it receives an explicit destination string and no stream. It is not `snprintf`, because no capacity parameter exists and the output callback has no bounds check. A fixed-arity helper is inconsistent with the explicit first-vararg pointer, and `void` is inconsistent with preservation and caller consumption of the formatter result. Simple string-copy/concatenation interpretations are incompatible with the explicit format pointer and common formatter invocation.

Private static regression against the canonical ROMs and official caller corpus executed with **OVERALL PASS**. Dynamic/emulator regression remains specified but was not executed. Firmware bytes and detailed disassembly remain private in Drive.

## SOURCE-FIRST re-audit — 2026-09-04

The sequential re-audit began from the historical API layer rather than from the binary. `os3k.h` and `os3k.pdf` preserve `int sprintf(char *str, const char *fmt, ...)`, while the generated syscall representations map `sprintf` to index 222 / A378. `debug.pdf` supplies independent real source callers that format hexadecimal bytes and addresses into caller-owned buffers before printing them. These references were treated only as hypotheses and behavioral anchors.

The three canonical ROM images were then re-extracted independently and their SHA-256 identities revalidated. A378 again resolves to the same 0x2C handler in each generation. The vector targets, raw handler hashes, relocation-normalized hash, and byte-identical 0x18 string callback all reproduce the archived workpaper. Stack/dataflow confirms `str`, `fmt`, and the variadic tail; the wrapper clears `str[0]`, passes a mutable destination cursor cell and the string callback to the same formatter engine already revalidated through A34C/`fprintf` and A36C/`printf`, and preserves the formatter result in `D0.L`.

The adversarial pass again rejects `printf`, `fprintf`, `snprintf`, fixed-arity, `void`, and simple copy/concatenation interpretations. The previously executed complete caller sweep remains consistent with the reconstructed contract: 598 executable calls in 25 table-bearing applets, plus controlled negative cohorts, with representative callers consuming `D0.L`. No source/firmware contradiction was found.

Classification after re-audit: **CLOSED A / SOURCE_FIRST / PUBLISHED**. Static regression remains **OVERALL PASS**; dynamic/emulator-first regression remains **SPECIFIED / NOT EXECUTED**.
