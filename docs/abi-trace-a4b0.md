# SYS_A4B0 (index 300)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 300 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0CC` | `0x01020001` | Not a valid firmware runtime pointer |
| NEO (2005) | `+0x0319F6` | `0x01020001` | Not a valid firmware runtime pointer |
| NEO (2013) | `+0x03F60E` | `0x54502F31` | Data (`TP/1`), not a canonical runtime pointer |

A complete raw-word sweep of all three canonical ROMs finds no `0xA4B0` word. Consequently no handler bytes, bounds, arguments, return value, globals, helpers, or function prototype can be attributed to this slot.

The official SmartApplet corpus was checked structurally. Each of the 16 EXTENDED applets contains exactly one demonstrated late table tail `A450,A454,A468,A46C`, and none contains an `A4B0` stub there. The remaining 14 LEGACY applets terminate before this late range and 11 corpus members have no A-line table, yielding 41/41 without an `A4B0` stub.

Four raw `A4B0` byte pairs occur elsewhere in the 16 EXTENDED binaries, all refuted as stubs: SpellCheck Large USA `+0x1CF58` lies in a structured payload table; Thesaurus Small USA `+0x87C2` lies in a dense data table; Wireless Update `+0x960C` lies in an embedded update-code/payload region; Wireless Update `+0x1904B` is at an odd offset and cannot begin an aligned m68k opcode. None lies in the demonstrated late A-line table.

Static adversarial regression: **82/82 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4B0`. It assigns no vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
