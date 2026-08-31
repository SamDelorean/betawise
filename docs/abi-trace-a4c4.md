# SYS_A4C4 (index 305)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 305 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0E0` | `0x00010902` | Not a valid firmware runtime pointer |
| NEO (2005) | `+0x031A0A` | `0x00010902` | Not a valid firmware runtime pointer |
| NEO (2013) | `+0x03F622` | `0x0D0A0D0A` | CR/LF data, not a canonical runtime pointer |

A complete raw-word sweep of all three canonical ROMs finds no `0xA4C4` word. Consequently no handler bytes, bounds, arguments, return value, globals, helpers, or function prototype can be attributed to this slot.

The official SmartApplet corpus was checked structurally. Each of the 16 EXTENDED applets retains the demonstrated late table tail `A450,A454,A468,A46C`, and none contains an `A4C4` stub there. The remaining 14 LEGACY applets terminate before this late range and 11 corpus members have no A-line table, yielding 41/41 without an `A4C4` stub.

Seven raw `A4C4` byte pairs occur elsewhere in the 16 EXTENDED binaries. Five are at odd offsets and cannot begin aligned m68k word opcodes. AlphaWordPlus `+0x88EC` is the low word of the 32-bit immediate `0x0000A4C4` consumed by opcode `0x207C` (`MOVEA.L #imm32,A0`) beginning at `+0x88E8`; the following instruction begins at `+0x88EE`. Thesaurus Large USA `+0x12ACA` is even-aligned but lies well after the demonstrated terminal `RTS` at `+0x5414` and payload boundary at `+0x5418`. Neither is promoted to a caller.

Static adversarial regression: **86/86 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4C4`. It assigns no vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics. The historical meaning of the data occupying the nominal slot remains unknown.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
