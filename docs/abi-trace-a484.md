# SYS_A484 / index 289 — ABI trace

Status: **mechanically reconstructed (confidence A)** as **no callable contract / ABI gap**.

## Canonical vector evidence

| Firmware | vector slot | raw value | disposition |
|---|---:|---:|---|
| AlphaSmart 3000 (2005) | file `+0x02B0A0` | `0x19012903` | not a valid runtime pointer |
| NEO (2005) | file `+0x0319CA` | `0x19012903` | not a valid runtime pointer |
| NEO (2013) | file `+0x03F5E2` | `0x004E7B7D` | odd address; not a valid 68k instruction target |

All three canonical ROMs were SHA-256 revalidated before reading index 289. None of the three slots yields a valid callable handler. The NEO 2013 value is numerically in the low firmware address region but fails the 68k alignment requirement because it is odd; it is therefore not promoted to a handler address.

## Caller/xref evidence

The 16 EXTENDED official SmartApplets were independently rehashed and their demonstrated late A-line import-table windows inspected. `A484` appears in **0/16**. Together with 14 LEGACY applets whose tables terminate before this late range and 11 applets without an A-line table, the official corpus is **41/41 negative for an A484 stub**. The three canonical firmware images contain **zero raw `A484` words**.

Raw applet matches were inventoried adversarially. Six occurrences in Thesaurus Large (`+0x6532`, `+0xB204`, `+0x10997`, `+0x498AD`, `+0x49CC5`, `+0x4AB35`) all occur after the demonstrated terminal executable boundary at RTS `+0x5414` and therefore lie in later payload/data. None is promoted to an executable caller.

## Regression

A block-specific static regression covering canonical ROM hashes and slots, alignment/range checks, firmware raw searches, all 16 EXTENDED hashes/table windows, complete A484 raw inventory, Thesaurus payload boundary, and the structural 41-item corpus accounting completed **82/82 PASS**. Dynamic regression is not applicable because no callable contract exists.

## Contract disposition

`SYS_A484` remains a neutral identifier only. No vendor name, prototype, arguments, return type, enum, flag, error code, or subsystem meaning is inferred from the marker values.

Private workpapers retain the exact corpus hashes, raw-hit inventory, boundary evidence, regression source/output, and reproducibility details; firmware and extended disassembly are intentionally not published here.
