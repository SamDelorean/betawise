# SYS_A424 / index 265 — blocked contractual return

Forward-sweep disposition for the OS3K A-line ABI.

- Status: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in `RETORNO_CONTRACTUAL=DESCONOCIDO`.
- AS3000 (2005): table value `0x5A06004E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2005): table value `0x7F08005E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2013): callable runtime `0x00434348`, file offset `0x024348`.
- Handler: `0x5A` bytes / 29 instructions / one terminal RTS; SHA-256 `cfd73e3dd4facd3f85122c02e7149d739a9370891e219c25a308deae80232bda`.
- Physical ABI: two 32-bit slots. Slot 1 is consumed full-width; slot 2 only through its low byte.
- The handler uses three private globals at `0xF412`, `0xF416`, and `0xF41A`, plus private helpers at `0x004346C8` and `0x0044254E`; no semantic names are asserted.
- Direct NEO 2013 firmware xrefs: exactly one BSR.W from A3DC, no direct JSR/JMP/BSR.B.
- The A3DC caller pushes exactly the two physical slots and overwrites `D0` immediately after A424 returns.
- `D0` is not uniformly defined across A424 paths: one path leaves it untouched, another leaves an internal arithmetic value, and another propagates the private helper result.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- No independent symbol/prototype was recovered.
- Static regression: `OVERALL PASS`, 34/34 assertions.
- Dynamic/emulator-first regression: not executed.

`void` is strongly suggested by the sole caller and non-uniform `D0`, but absence of return use is not positive ABI proof. A callable C prototype is therefore intentionally not published. Private ROM bytes, disassembly and regression workpapers remain in Drive.
