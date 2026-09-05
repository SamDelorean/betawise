# ABI trace — A408 / index 258

- Disposition: `CERRADO A / SOURCE_FIRST / PUBLICADO`, with contractual return intent explicitly unknown.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: runtime `0x0043420A`, file offset `0x02420A`.
- Handler: `0x0E` bytes, three linear instructions, SHA-256 `7b1e6c2d6a6701cf77dba244b109ae1b9c362815012b33afa7c35fca4d1541e7`.
- Physical ABI: one 32-bit slot; only low 8 bits are consumed.
- Mechanical state effect: writes the byte at private state offset `+0x111` through the same firmware-owned pointer used by A404.
- Direct NEO 2013 firmware callers: exactly 1; argument observed as zero; D0 is not consumed after return.
- Official SmartApplet exposure: structurally absent (`0/41`).
- Independent symbol/prototype: no vendor semantic name recovered; BetaWise/ioma8-neo-re retain only `SYS_A408` at index 258.
- Static regression: historical `17/17 PASS`; fresh directed SOURCE_FIRST regression `19/19 PASS` on 2026-09-05.
- Dynamic/emulator-first: specified, not executed.
- Public callable header: intentionally omitted because return intent is not independently demonstrated.

## SOURCE_FIRST revalidation

Fresh extraction from all three canonical ROMs reproduced the generational split and the exact NEO 2013 handler. The routine loads the firmware-owned pointer at `0x0000F416` and stores the low byte of its sole physical argument into offset `+0x111`. It does not define D0. The only direct caller pushes zero and does not consume D0 afterward.

A404 and A408 therefore form a mechanically confirmed getter/store pair for the same private byte, but neither the byte's vendor meaning nor A408's contractual return type is recoverable from the available evidence. `void` remains a very strong inference rather than a confirmed ABI claim. Under the current audit methodology, `RETORNO_CONTRACTUAL=DESCONOCIDO` is an evidence limit, not a reason to leave the block open.

Private ROM extraction, disassembly, caller traces and regression workpapers remain outside the repository.
