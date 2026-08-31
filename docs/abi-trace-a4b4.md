# SYS_A4B4 (index 301)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 301 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0D0` | `0x12010001` | Not a valid firmware runtime pointer |
| NEO (2005) | `+0x0319FA` | `0x12010001` | Not a valid firmware runtime pointer |
| NEO (2013) | `+0x03F612` | `0x2E30202F` | Data (`.0 /`), not a canonical runtime pointer |

A complete raw-word sweep of all three canonical ROMs finds no `0xA4B4` word. Consequently no handler bytes, bounds, arguments, return value, globals, helpers, or function prototype can be attributed to this slot.

The official SmartApplet corpus was checked structurally. Each of the 16 EXTENDED applets contains exactly one demonstrated late table tail `A450,A454,A468,A46C`, and none contains an `A4B4` stub there. The remaining 14 LEGACY applets terminate before this late range and 11 corpus members have no A-line table, yielding 41/41 without an `A4B4` stub.

Eight raw `A4B4` byte pairs occur elsewhere in the 16 EXTENDED binaries, all refuted as stubs. SpellCheck Large USA `+0x75A3` is at an odd offset and cannot begin an aligned m68k opcode. Thesaurus Large USA has hits at `+0x14F16`, `+0x17CC5`, `+0x2723D`, and `+0x27C86`; Thesaurus Small USA has hits at `+0x544E`, `+0x7A6A`, and `+0x111E7`. Both Thesaurus binaries share a demonstrated terminal `RTS` at `+0x5414` with transition to payload by `+0x5418`, so all seven Thesaurus matches are post-code payload; three are additionally at odd offsets. None of the eight hits lies in the demonstrated late A-line table.

Static adversarial regression: **91/91 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4B4`. It assigns no vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics. The historical meaning of the data occupying the nominal slot remains unknown.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
