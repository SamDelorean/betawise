# SYS_A490 (index 292)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K vector slot at index 292 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | Slot value | Disposition |
| --- | --- | --- |
| AlphaSmart 3000 (2005) | `0x95067508` | Not a valid firmware runtime pointer |
| NEO (2005) | `0x95067508` | Not a valid firmware runtime pointer |
| NEO (2013) | `0x00000044` | Not a valid canonical firmware runtime pointer |

Consequently, no handler bytes, handler boundaries, callable arguments, return value, globals, helpers, or function prototype can be attributed to this slot from the canonical firmware. A raw-word sweep of all three canonical firmware images also found no `0xA490` words.

The official SmartApplet corpus was checked structurally: the demonstrated late A-line table window contains no `A490` stub in any of the 16 EXTENDED applets; the remaining 14 LEGACY applets terminate before that late table and 11 corpus members have no A-line table. Thus the corpus result is 41/41 without an `A490` table stub.

Eight raw `0xA490` occurrences were examined adversarially instead of being promoted automatically to calls. Six occur in SpellCheck Large USA, all after its demonstrated terminal `RTS`; two occur in Wireless Update, both after its demonstrated executable tail and late import/table region. There is no raw `A490` occurrence in the demonstrated executable prefix of either applet, so these eight occurrences are payload/data rather than executable A-line call sites.

Static adversarial regression: **96/96 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A490`. It does not assign a vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
