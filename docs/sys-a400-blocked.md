# SYS_A400 / index 256 — blocked contractual return

Forward-sweep disposition for the OS3K A-line ABI.

## Disposition

`MECÁNICA_RECONSTRUIDA A / BLOQUEADO` exclusively in `RETORNO_CONTRACTUAL=DESCONOCIDO`.

- AS3000 (2005): table value is non-pointer / unresolved; no callable contract assigned.
- NEO (2005): table value is non-pointer / unresolved; no callable contract assigned.
- NEO (2013): callable at runtime `0x004343A2` (file offset `0x0243A2`).
- Handler length: `0x2DA` bytes; 244 decoded instructions; one RTS; SHA-256 `407349319b3fbf0c0f5245126a20ed31a45649267c79e6a218655dbfb6a29e7d`.
- Physical ABI: one 32-bit stack slot; only its low 16 bits are consumed.
- One direct NEO 2013 firmware caller was identified. It supplies a zero-extended 16-bit value, cleans four bytes, and does not consume D0 after the call.
- The handler performs side effects through private state, record structures and helpers. D0 is not uniformly normalized on all exits; some paths reach the common epilogue after private helper calls.
- `void` is therefore a strong inference from observed use, but it is not promoted to a confirmed contractual return type without an independent prototype/symbol or a positive caller contract.
- Official SmartApplet exposure is structurally absent from the validated late-table corpus.
- Static regression: `OVERALL PASS`, 29/29 assertions.
- Dynamic/emulator-first regression: not executed.

No callable header is published for A400. Semantic names for the argument, globals, records, selectors and helpers remain unknown unless independently demonstrated.

Private ROM bytes, disassembly and detailed workpapers remain outside the repository.
