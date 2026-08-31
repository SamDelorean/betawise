# ABI trace — A424 / index 265

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in contractual return intent.
- AS3000 2005 / NEO 2005: non-pointer/unresolved table values; only NEO 2013 is callable.
- NEO 2013 handler: runtime `0x00434348`, file `+0x024348`, length `0x5A`, 29 instructions, SHA-256 `cfd73e3dd4facd3f85122c02e7149d739a9370891e219c25a308deae80232bda`.
- Physical ABI: two 32-bit slots, effective widths full32 and low8.
- CFG closes at the first coherent RTS; the much larger A424→A428 vector interval contains later independent functions and is not the handler length.
- Direct firmware caller: exactly one BSR.W from A3DC. The caller constructs the two slots and overwrites `D0` immediately after return.
- A424 `D0` is path-dependent and non-uniform; no independent consumer or prototype establishes a contractual return.
- Private global/helper interactions are mechanically documented but intentionally unnamed.
- Official SmartApplet exposure: structurally absent.
- Static regression: `OVERALL PASS`, 34/34 assertions. Dynamic regression: not executed.

No callable header is published. Private instruction listings, ROM extraction and regression details remain in Drive.
