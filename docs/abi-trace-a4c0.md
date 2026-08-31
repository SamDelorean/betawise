# SYS_A4C0 (index 304)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 304 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0DC` | `0x01000103` | Not a valid firmware runtime pointer |
| NEO (2005) | `+0x031A06` | `0x02000103` | Not a valid firmware runtime pointer |
| NEO (2013) | `+0x03F61E` | `0x3A323334` | ASCII data (`:234`), not a canonical runtime pointer |

A complete raw-word sweep of all three canonical ROMs finds no `0xA4C0` word. Consequently no handler bytes, bounds, arguments, return value, globals, helpers, or function prototype can be attributed to this slot.

The official SmartApplet corpus was checked structurally. Each of the 16 EXTENDED applets contains exactly one demonstrated late table tail `A450,A454,A468,A46C`, and none contains an `A4C0` stub there. The remaining 14 LEGACY applets terminate before this late range and 11 corpus members have no A-line table, yielding 41/41 without an `A4C0` stub.

Six raw `A4C0` byte pairs occur elsewhere in the 16 EXTENDED binaries. Five are at odd offsets and cannot begin aligned m68k opcodes. The only even hit, Wireless Update `+0x1905E`, lies inside a dense 8051-style opcode/data stream (`... D0 00 A4 C0 83 D0 82 ...`) and is outside the demonstrated late A-line table.

Static adversarial regression: **85/85 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4C0`. It assigns no vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
