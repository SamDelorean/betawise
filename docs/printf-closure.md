# A36C / `printf` closure

Status: **mechanical A / published**.

Reconstructed contract:

```c
int printf(const char *fmt, ...);
```

The three canonical OS3K ROMs resolve A36C to AS3000 2005 `0x004DD412` (`file+0x01D412`), NEO 2005 `0x005DFB30` (`file+0x01FB30`), and NEO 2013 `0x00435F2E` (`file+0x025F2E`). The wrapper is 0x28 bytes and terminates with `RTS` at +0x26. Its raw hashes differ only at three absolute operands; after neutralizing those relocations all three normalize to SHA-256 `150c204cd6a546022852997961ff098e37b06bf157dcb0f9518570af6e95f72d`.

Primary dataflow proves one fixed format argument plus a variadic tail. The wrapper forms a pointer to the first variadic argument, reloads the fixed `fmt` pointer, supplies the OS stream descriptor whose stored value is `1` (`stdout`) in all three ROMs, installs A350 / `_OS3K_fputc` as the output callback, and calls the exact same common formatter used by A34C / `fprintf`. The formatter's full `D0.L` result is returned unchanged.

The corrected PC-index detector was run over the complete official 41-SmartApplet corpus. It finds 99 executable A36C callers in 13 applets: 44 calls in the AS3000 cohort and 55 in the NEO cohort. Seventeen table-bearing applets are negative and eleven additional applets are structural negatives without the relevant A-line table. The same detector reproduces the established controls A330=12 and A33C=67. Direct firmware absolute-JSR counts are 7 / 7 / 11 for AS3000 2005 / NEO 2005 / NEO 2013.

Adversarial alternatives are mechanically excluded. A36C is not `fprintf`, because it consumes no external stream and inserts `stdout` internally; A34C is the distinct stream-plus-format wrapper. It is not `sprintf`, because its output callback is the stream writer A350 rather than a buffer writer. A formatless output helper is inconsistent with the explicit format pointer, first-vararg pointer and common formatter invocation.

Private static regression against the canonical ROMs and official caller corpus executed with **OVERALL PASS**. Dynamic/emulator regression remains specified but was not executed. Firmware bytes and detailed disassembly remain private in Drive.

## SOURCE-FIRST re-audit — 2026-09-04

Historical sources were consulted first: `os3k/syscall.c` maps index 219 to `printf`, while `os3k/os3k.h` preserves `int printf(const char *fmt, ...)`. They were used only as anchors. The independent firmware reconstruction then reconfirms the identity: A36C takes a fixed `fmt` plus a variadic tail, internally supplies the OS object whose stored value is `1`, routes output through A350/`_OS3K_fputc`, invokes the same common formatter as A34C/`fprintf`, and propagates the formatter's full `D0.L` result.

This also provides a source-first cross-check against neighboring wrappers: an external stream argument is absent (excluding `fprintf`), while the callback is stream-oriented rather than buffer-oriented (excluding `sprintf`). The complete 41-app corpus still yields 99 executable callers and ROM xrefs remain 7/7/11. No contradiction between historical sources and primary firmware evidence was found.

Classification after re-audit: **CLOSED A / SOURCE_FIRST / PUBLISHED**. Previously executed static regression remains `OVERALL PASS`; dynamic/emulator-first regression remains **NOT EXECUTED**.
