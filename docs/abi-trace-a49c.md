# SYS_A49C (index 295)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K vector slot at index 295 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | Slot value | Disposition |
| --- | --- | --- |
| AlphaSmart 3000 (2005) | `0x29658100` | Not a valid firmware runtime pointer |
| NEO (2005) | `0x29658100` | Not a valid firmware runtime pointer |
| NEO (2013) | `0x72652E2E` | Not a valid canonical firmware runtime pointer |

Consequently, no handler bytes, handler boundaries, callable arguments, return value, globals, helpers, or function prototype can be attributed to this slot from the canonical firmware.

One raw `0xA49C` word occurs in the AlphaSmart 3000 firmware, but instruction-boundary analysis refutes it as a caller: bytes at file offset `0xEC78` encode an `ADDI.L` immediate whose 32-bit immediate value is `0x004EA49C`; the `A49C` word is the low half of that immediate, not an A-line opcode. NEO 2005 and NEO 2013 contain no raw `A49C` words.

The official SmartApplet corpus was checked structurally: no `A49C` stub occurs in the demonstrated late A-line table window of any of the 16 EXTENDED applets; the remaining 14 LEGACY applets terminate before that late table and 11 corpus members have no A-line table. Thus the corpus result is 41/41 without an `A49C` table stub.

Seven raw applet occurrences were classified adversarially. Five are at odd file offsets and therefore cannot be word-aligned 68k A-line opcodes. The two even occurrences are in Thesaurus Small USA after the demonstrated common terminal `RTS`/payload boundary (`0x5418`). An additional repeated 402-byte block comparison places three of the odd occurrences at the same relative byte offset in shared payload across unrelated applets.

Static adversarial regression: **89/89 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A49C`. It does not assign a vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
