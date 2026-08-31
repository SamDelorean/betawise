# ABI trace — A41C / index 263

- Disposition: `MECÁNICA_CERRADA A / PUBLICADO`.
- Neutral contract: `void *SYS_A41C(uint8_t selector)`.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: runtime `0x00433B86`, file offset `0x023B86`.
- Handler: `0x92` bytes, 52 instructions, one RTS; SHA-256 `ab0942b2bd308ec22364abbd82bd419f0c5b6bebe884cbb4702e70828d25f255`.
- Physical ABI: one slot, effective low-byte selector.
- Return: NULL on initial allocation/selection failure; otherwise an explicitly returned firmware-owned structure pointer in D0.L.
- Selector helper demonstrates a raw valid byte range `5..21`; no semantic enum name is assigned.
- Success path builds at most five destination records (`0x12C` stride) from private descriptors (`0x0C` stride).
- Direct firmware callers: four JSRs; concrete selector values `7`, `8`, `10`; all provide positive pointer-return evidence.
- Direct JMP/BSR references: zero.
- Official SmartApplet exposure: structurally absent (`0/41`) because this late slot is beyond the validated applet table.
- Independent vendor symbol/prototype: not recovered.
- Static regression: `42/42 PASS / OVERALL PASS`.
- Dynamic/emulator-first: not executed.
- Public header: `os3k/sys_a41c.h`.

Private ROM extraction, disassembly, helper correlation, caller traces and regression workpapers remain outside the repository.
