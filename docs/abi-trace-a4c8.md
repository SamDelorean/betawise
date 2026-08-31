# SYS_A4C8 (index 306)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 306 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0E4` | `0x22000101` | Not a valid firmware runtime pointer |
| NEO (2005) | `+0x031A0E` | `0x22000101` | Not a valid firmware runtime pointer |
| NEO (2013) | `+0x03F626` | `0x004E0044` | Data, not a canonical runtime pointer |

A complete raw-word sweep of all three canonical ROMs finds no `0xA4C8` word. Consequently no handler bytes, bounds, arguments, return value, globals, helpers, or function prototype can be attributed to this slot.

The official SmartApplet corpus was checked structurally. Each of the 16 EXTENDED applets retains exactly one demonstrated late table tail `A450,A454,A468,A46C`, and none contains an `A4C8` stub in that late-table window. The remaining 14 LEGACY applets terminate before the late range and 11 corpus members have no A-line table, yielding 41/41 without an `A4C8` stub.

Eleven raw `A4C8` byte pairs occur elsewhere in the 16 EXTENDED binaries, all confined to Thesaurus Large USA and Thesaurus Small USA. Every occurrence lies after the demonstrated executable/payload boundary `+0x5418`; both binaries retain the terminal `RTS` at `+0x5414`. Four of the eleven occurrences are additionally odd-aligned. These raw pairs are payload data, not A-line caller/stub entries.

Static adversarial regression: **80/80 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4C8`. It assigns no vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics. The historical meaning of the data occupying the nominal slot remains unknown.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
