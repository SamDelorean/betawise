# SYS_A4A0 (index 296)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K vector slot at index 296 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | Slot value | Disposition |
| --- | --- | --- |
| AlphaSmart 3000 (2005) | `0xC04E1201` | Not a valid firmware runtime pointer |
| NEO (2005) | `0xC04E1201` | Not a valid firmware runtime pointer |
| NEO (2013) | `0x2E00A507` | Not a valid canonical firmware runtime pointer |

No handler bytes, handler boundaries, callable arguments, return value, globals, helpers, or function prototype can therefore be attributed to this slot from the canonical firmware. A raw-word sweep of all three canonical ROMs finds no `0xA4A0` word.

The official SmartApplet corpus was checked structurally. None of the 16 EXTENDED applets contains an `A4A0` stub in the demonstrated late A-line table window; the remaining 14 LEGACY applets terminate before that late table and 11 corpus members have no A-line table. The corpus result is therefore 41/41 without an `A4A0` table stub.

Every EXTENDED applet does contain one raw byte-pair `A4 A0` elsewhere, but this was classified adversarially as a cross-word artifact. In each file it occurs inside the same aligned word sequence `A08C, A090, A094, A098, A09C, A0A0, A0A4, A0A8, A0AC, A0B0, A0B4, A0B8, A0BC`: the raw byte-pair is formed by the low byte of aligned word `A0A4` followed by the high byte of aligned word `A0A8`, so it is not an `A4A0` opcode boundary. The few additional raw occurrences are either odd-aligned or lie beyond demonstrated executable/table boundaries.

Static adversarial regression: **115/115 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4A0`. It does not assign a vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
