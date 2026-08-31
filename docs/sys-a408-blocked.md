# SYS_A408 / index 258 — blocked contractual return

Forward-sweep disposition for the OS3K A-line ABI.

## Disposition

`MECÁNICA_RECONSTRUIDA A / BLOQUEADO` exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`.

- AS3000 (2005): table value is non-pointer/unresolved; no callable contract assigned.
- NEO (2005): table value is non-pointer/unresolved; no callable contract assigned.
- NEO (2013): callable at runtime `0x0043420A`, file offset `0x02420A`.
- Handler length: `0x0E` bytes; three instructions; SHA-256 `7b1e6c2d6a6701cf77dba244b109ae1b9c362815012b33afa7c35fca4d1541e7`.
- Physical ABI: one 32-bit stack slot, consumed only through its low byte.
- Mechanical effect: store that low byte to a private firmware-owned byte field also read by A404. No semantic field name is assigned.
- Exactly one direct NEO 2013 firmware caller was found. It passes zero and does not consume D0 after the call.
- The handler does not define D0. `void` is therefore a very strong behavioral inference, but no independent prototype/vendor symbol or positive caller contract was recovered.
- Official SmartApplet exposure is structurally absent from the validated late-table corpus.
- Static regression: `OVERALL PASS`, 17/17 assertions.
- Dynamic/emulator-first regression: not executed.

No callable header is published for A408.
