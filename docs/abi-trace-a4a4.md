# SYS_A4A4 (index 297)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K vector slot at index 297 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | Slot value | Disposition |
| --- | --- | --- |
| AlphaSmart 3000 (2005) | `0x00010000` | Not a valid firmware runtime pointer |
| NEO (2005) | `0x00010000` | Not a valid firmware runtime pointer |
| NEO (2013) | `0x0006014D` | Not a valid canonical firmware runtime pointer |

No handler bytes, handler boundaries, callable arguments, return value, globals, helpers, or function prototype can therefore be attributed to this slot from the canonical firmware. A raw-word sweep of all three canonical ROMs finds no `0xA4A4` word.

The official SmartApplet corpus was checked structurally. None of the 16 EXTENDED applets contains an `A4A4` stub in the demonstrated late A-line table window; the remaining 14 LEGACY applets terminate before that late table and 11 corpus members have no A-line table. The corpus result is therefore 41/41 without an `A4A4` table stub.

Three raw applet occurrences were classified adversarially. `keywords.os3kapp` contains an even-aligned `A4A4` value at file offset `0xCF60`, but the surrounding bytes form a monotonic table of 4-byte `(value16, attribute16)` records: `A433/0001, A44F/0001, A46C/0001, A486/0001, A4A4/0001, A4C1/0001, A4DB/0001, A4FA/0000`. These values are valid offsets into the applet; the `A4A4` record points to bytes at file offset `0xA4A4`, confirming that the raw word at `0xCF60` is a table field rather than an A-line opcode. Two additional occurrences in Thesaurus Large USA (`0x2069B`, `0x28BE9`) are odd-aligned and lie after the demonstrated executable/payload boundary.

Static adversarial regression: **85/85 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4A4`. It does not assign a vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
