# SYS_A420 / index 264 — blocked contractual return

Forward-sweep disposition for the OS3K A-line ABI.

- Status: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in `RETORNO_CONTRACTUAL=DESCONOCIDO`.
- AS3000 (2005): table value `0x5A06004E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2005): table value `0x7F08005E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2013): callable runtime `0x00433DAE`, file offset `0x023DAE`.
- Handler: `0x32` bytes, SHA-256 `b6eb5df5a0e11fad59e68e74dacac6d2bd1655faaf720dc5df82b67e59626392`.
- Physical external ABI: three 32-bit slots. Slot 1 is forwarded full-width to A3DC arg1; slot 2 is consumed through its low byte; slot 3 is a required readable pointer whose four consecutive longwords at offsets `+0,+4,+8,+0x0C` are expanded into A3DC arguments.
- A420 synthesizes A3DC arg7 as zero and calls A3DC with seven physical slots, then propagates A3DC `D0` unchanged.
- Direct NEO 2013 firmware xrefs to A420: 0 JSR / 0 JMP / 0 BSR.W / 0 BSR.B.
- Official SmartApplet exposure: structurally absent from the validated late-table corpus.
- No independent symbol/prototype or return consumer was recovered.
- Static regression: `OVERALL PASS`, 23/23 assertions.
- Dynamic/emulator-first regression: not executed.

Because the underlying A3DC return width/signedness is not established and A420 has no demonstrated return consumer, no callable C prototype is published. Private ROM bytes, disassembly and regression workpapers remain in Drive.
