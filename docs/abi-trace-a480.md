# SYS_A480 / index 288 — ABI trace

Status: **mechanically reconstructed (confidence A)** as **no callable contract / ABI gap**.

## Canonical vector evidence

| Firmware | vector slot | raw value | disposition |
|---|---:|---:|---|
| AlphaSmart 3000 (2005) | file `+0x02B09C` | `0x75010508` | not a valid runtime pointer |
| NEO (2005) | file `+0x0319C6` | `0x75010508` | not a valid runtime pointer |
| NEO (2013) | file `+0x03F5DE` | `0x89909192` | not a valid runtime pointer |

All three canonical ROMs were SHA-256 revalidated before reading index 288. No slot addresses a handler, so there are no handler bytes, boundaries, 68k disassembly, globals, helpers, arguments, or return semantics to publish.

## Caller/xref evidence

The 16 EXTENDED official SmartApplets were independently rehashed and their demonstrated late A-line import-table windows were inspected. `A480` appears in **0/16**. Together with 14 LEGACY applets whose tables terminate before this late range and 11 applets without an A-line table, the official corpus is **41/41 negative for an A480 stub**. The three canonical ROM images contain zero raw `A480` words.

Raw applet matches were retained and refuted rather than silently discarded. Six matches in SpellCheck Large occur after the terminal RTS of the executable tail shared with SpellCheck Small. One match in Thesaurus Large is after its demonstrated terminal code boundary. Two matches in WirelessUpdate occur after the executable code/import-table boundary; its demonstrated late table ends at `A470`, with no `A480` entry. None is promoted to an executable caller.

## Regression

A block-specific static regression covering canonical hashes/slots, firmware raw searches, all 16 EXTENDED hashes and table windows, complete A480 raw inventories, payload boundaries, and 41-item corpus accounting completed **88/88 PASS**. Dynamic regression is not applicable because no callable contract exists.

## Contract disposition

`SYS_A480` remains a neutral identifier only. No vendor name, prototype, arguments, return type, enum, flag, error code, or subsystem meaning is inferred from the marker values.

Private workpapers retain the exact corpus hashes, raw-hit inventory, boundary evidence, regression source/output, and reproducibility details; firmware and extended disassembly are intentionally not published here.
