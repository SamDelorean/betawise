# SYS_A4AC (index 299)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K vector slot at index 299 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | Slot value | Disposition |
| --- | --- | --- |
| AlphaSmart 3000 (2005) | `0x04BD0100` | Not a valid firmware runtime pointer |
| NEO (2005) | `0x04BD0200` | Not a valid firmware runtime pointer |
| NEO (2013) | `0x004E5344` | Not a valid canonical firmware runtime pointer |

No handler bytes, handler boundaries, callable arguments, return value, globals, helpers, or function prototype can therefore be attributed to this slot from the canonical firmware. A raw-word sweep of all three canonical ROMs finds no `0xA4AC` word.

The official SmartApplet corpus was checked structurally. None of the 16 EXTENDED applets contains an `A4AC` stub in the demonstrated late A-line table window; the remaining 14 LEGACY applets terminate before that late table and 11 corpus members have no A-line table. The corpus result is therefore 41/41 without an `A4AC` table stub. A complete raw-word inventory of the 16 EXTENDED applets also finds no `0xA4AC` occurrence.

Static adversarial regression: **77/77 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4AC`. It does not assign a vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
