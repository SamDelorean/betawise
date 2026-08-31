# SYS_A47C / index 287 — ABI trace

Status: **mechanically reconstructed (confidence A)** as **no callable contract / ABI gap**.

## Canonical vector evidence

| Firmware | vector slot | raw value | disposition |
|---|---:|---:|---|
| AlphaSmart 3000 (2005) | file `+0x02B098` | `0x81019503` | not a valid runtime pointer |
| NEO (2005) | file `+0x0319C2` | `0x81019503` | not a valid runtime pointer |
| NEO (2013) | file `+0x03F5DA` | `0x83848788` | not a valid runtime pointer |

The three canonical ROM images were SHA-256 revalidated before reading the slots. Since none of the three values addresses a callable handler, there are no handler bytes, boundaries, 68k disassembly, globals, helpers, arguments, or return semantics to publish for this slot.

## Caller/xref evidence

The official SmartApplet corpus was checked independently for `A47C`. The 16 EXTENDED applets were rehashed and their demonstrated late A-line table windows (`A450, A454, A468, A46C, ...`) were inspected: **0/16** contains an `A47C` table entry. Together with 14 LEGACY applets whose tables terminate before this range and 11 applets without an A-line table, coverage is **41/41 negative for an A47C stub**.

Raw-word searches were also treated adversarially rather than as callers. One `A47C` occurrence in MathFactsOffline is the low word of the immediate operand in `MOVEA.L #0x0000A47C,A0`, not an A-line opcode. Seventeen occurrences in Thesaurus Large are after the demonstrated terminal code boundary and lie in the later payload. The three canonical firmware images contain **zero raw `A47C` words**.

## Regression

A block-specific static regression covering ROM hashes and slot values, raw firmware search, all 16 EXTENDED hashes/table windows/raw inventories, the false-positive operand boundary, the Thesaurus payload boundary, and the structural 41-item corpus accounting completed **83/83 PASS**. No dynamic regression is applicable because no callable contract exists.

## Contract disposition

`SYS_A47C` remains a neutral identifier only. No vendor name, prototype, arguments, return type, enum, flag, error code, or subsystem meaning is inferred from the non-pointer marker values.

Private workpapers retain the ROM hashes, exact corpus hashes, raw-hit inventory, regression source/output, and reproducibility details; firmware and extended disassembly are intentionally not published here.
