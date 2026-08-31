# SYS_A4A8 (index 298)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K vector slot at index 298 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | Slot value | Disposition |
| --- | --- | --- |
| AlphaSmart 3000 (2005) | `0x00081E08` | Not a valid firmware runtime pointer |
| NEO (2005) | `0x00401E08` | Not a valid firmware runtime pointer |
| NEO (2013) | `0xA5070006` | Not a valid canonical firmware runtime pointer |

No handler bytes, handler boundaries, callable arguments, return value, globals, helpers, or function prototype can therefore be attributed to this slot from the canonical firmware. A raw-word sweep of all three canonical ROMs finds no `0xA4A8` word.

The official SmartApplet corpus was checked structurally. None of the 16 EXTENDED applets contains an `A4A8` stub in the demonstrated late A-line table window; the remaining 14 LEGACY applets terminate before that late table and 11 corpus members have no A-line table. The corpus result is therefore 41/41 without an `A4A8` table stub.

Four raw applet occurrences were found, all in Thesaurus Large USA at file offsets `0x14A26`, `0x2AD47`, `0x2AF85`, and `0x2C8F2`. All four occur after the demonstrated terminal `RTS` at `0x5414` and payload boundary at `0x5418`. Two are additionally odd-aligned (`0x2AD47`, `0x2AF85`) and therefore cannot begin a 68k word opcode. None is promoted to a caller.

Static adversarial regression: **82/82 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4A8`. It does not assign a vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
