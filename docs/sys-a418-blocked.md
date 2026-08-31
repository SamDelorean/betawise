# SYS_A418 / index 262 — blocked contractual return

Forward-sweep disposition for the OS3K A-line ABI.

## Disposition

`MECÁNICA_RECONSTRUIDA A / BLOQUEADO` exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`.

- AS3000 (2005): table value `0x5A06004E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2005): table value `0x7F08005E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2013): callable at runtime `0x004342D4`, file offset `0x0242D4`.
- Handler length: `0x3A` bytes; 19 decoded instructions; one RTS; SHA-256 `69350dbfc4df1cee84a38cde7ae9fbba77912d061e994cf68d394cb0a6f8289b`.
- Physical ABI: one 32-bit stack slot, used as a mutable pointer.
- Mechanical effect for non-NULL input: read the 16-bit count at offset `+0`, reset that many private records at base `0xF43E` with stride `0x56` through helper `0x004427FC`, then set the input object's count to zero and byte `+2` to `5`. NULL performs no mutation/helper call.
- The helper itself mechanically clears the observed `0x56`-byte record state; semantic names for the records and state value are not assigned.
- Exactly one direct NEO 2013 firmware caller was found. It passes one pointer, cleans four stack bytes, and does not logically consume A418's D0 result.
- D0 has no uniform terminal contract: NULL leaves zero, non-NULL/count-zero leaves the input pointer residual, and count-positive paths leave helper residue. `void` is therefore a very strong intent inference, but no independent prototype/vendor symbol or positive caller return contract was recovered.
- Official SmartApplet exposure is structurally absent from the validated late-table corpus.
- Static regression: `OVERALL PASS`, 26/26 assertions after correcting a test-literal typo; the initial 25/26 run failed only because the expected SHA string omitted its final hex character.
- Dynamic/emulator-first regression: not executed.

No callable header is published for A418.
