# SYS_A414 / index 261 — blocked contractual return

Forward-sweep disposition for the OS3K A-line ABI.

## Disposition

`MECÁNICA_RECONSTRUIDA A / BLOQUEADO` exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`.

- AS3000 (2005): table value `0x5F70004E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2005): table value `0x847C005E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2013): callable at runtime `0x0043425C` (file offset `0x02425C`).
- The A414→A418 vector interval is `0x78` bytes but contains two functions. A414 ends at the first RTS and is exactly `0x3C` bytes / 23 instructions; SHA-256 `cdeb3c3d86e7e425dc8bc1c327c1d75e23c24b1ae1626b5099cf1217bb71e473`. The following independent `0x3C`-byte routine is excluded.
- Physical ABI: two 32-bit slots. `arg1` is consumed full-width; only the low byte of `arg2` is consumed.
- A414 compares `arg2.low8` with private byte state at `0x0000F41A`, normalizing equality to longword 0/1. It also computes `arg1 + 4 + sign_extend_8(arg2.low8) * 0x12C`.
- It passes the computed record pointer and equality flag to private helper `0x004349BE`, then propagates that helper's D0 without local normalization.
- Direct NEO 2013 firmware xrefs to A414: zero JSR/JMP/BSR. Official SmartApplet exposure is structurally absent from the validated late-table corpus.
- No independent symbol or prototype was recovered.
- No contractual return width/signedness or `void` intent can be justified from the available evidence.
- Static regression: `17/17 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.

No callable header is published for A414. Semantic names for the arguments, private byte state, record structure and helper remain unknown unless independently demonstrated.

Private ROM bytes, disassembly and regression workpapers remain outside the repository.
