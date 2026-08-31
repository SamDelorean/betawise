# SYS_A488 / index 290 — ABI trace

Status: **mechanically reconstructed (confidence A)** as **no callable contract / ABI gap**.

## Canonical vector evidence

| Firmware | vector slot | raw value | disposition |
|---|---:|---:|---|
| AlphaSmart 3000 (2005) | file `+0x02B0A4` | `0x91029501` | not a valid runtime pointer |
| NEO (2005) | file `+0x0319CE` | `0x91029501` | not a valid runtime pointer |
| NEO (2013) | file `+0x03F5E6` | `0x5C000044` | not a valid firmware handler pointer |

All three canonical ROMs were SHA-256 revalidated before reading index 290. No slot addresses a callable handler, so handler bytes, boundaries, disassembly, globals, helpers, arguments and return semantics are N/A by primary evidence.

## Caller/xref evidence

The 16 EXTENDED official SmartApplets were independently rehashed and their demonstrated late A-line import-table windows inspected. `A488` appears in **0/16**. Together with 14 LEGACY applets whose tables terminate before this late range and 11 applets without an A-line table, the official corpus is **41/41 negative for an A488 stub**. The three canonical ROM images contain zero raw `A488` words.

Raw applet occurrences were classified by instruction/data boundary instead of being counted as callers. Three occurrences lie inside executable regions, but in every case the bytes are `20 7C 00 00 A4 88`: `A488` is the low word of the immediate operand of `MOVEA.L #0x0000A488,A0`, not an A-line opcode. As an adversarial control, the compiler/linker idiom `20 7C 00 00 xxxx 4E BB 88 FE` occurs 8,732 times with 6,668 distinct immediates across the 16 EXTENDED applets, so such an immediate value is not evidence of an OS3K syscall. The remaining raw matches are after demonstrated terminal executable boundaries in SpellCheck Large and Thesaurus Large and are payload/data.

## Regression

A block-specific static regression covering ROM hashes/slots, firmware raw search, all 16 EXTENDED hashes/table windows/raw inventories, instruction-boundary classification of the three code-region occurrences, payload boundaries, generic compiler-idiom controls and structural 41-item corpus accounting completed **81/81 PASS**. Dynamic regression is not applicable because no callable contract exists.

## Contract disposition

`SYS_A488` remains a neutral identifier only. No vendor name, prototype, arguments, return type, enum, flag, error code, or subsystem meaning is inferred from marker or immediate values.

Private workpapers retain exact hashes, raw-hit inventory, boundary evidence, regression source/output and reproducibility details; firmware and extended disassembly are intentionally not published here.
