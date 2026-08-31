# ABI trace — A408 / index 258

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` on contractual return intent only.
- AS3000 2005: non-callable/non-pointer table value.
- NEO 2005: non-callable/non-pointer table value.
- NEO 2013: runtime `0x0043420A`, file offset `0x02420A`.
- Handler: `0x0E` bytes, three linear instructions, SHA-256 `7b1e6c2d6a6701cf77dba244b109ae1b9c362815012b33afa7c35fca4d1541e7`.
- Physical ABI: one 32-bit slot; only low 8 bits are consumed.
- Mechanical state effect: writes the byte at private state offset `+0x111` through the same firmware-owned pointer used by A404.
- Direct NEO 2013 firmware callers: exactly 1; argument observed as zero; D0 not consumed after return.
- Official SmartApplet exposure: structurally absent (`0/41`).
- Independent symbol/prototype: not recovered.
- Static regression: `17/17 PASS / OVERALL PASS`.
- Dynamic/emulator-first: not executed.
- Public callable header: intentionally not published until return intent is independently demonstrated.
