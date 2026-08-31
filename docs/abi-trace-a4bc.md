# SYS_A4BC (index 303)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 303 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0D8` | `0x1E0801BD` | Not a valid firmware runtime pointer |
| NEO (2005) | `+0x031A02` | `0x1E0801BD` | Not a valid firmware runtime pointer |
| NEO (2013) | `+0x03F61A` | `0x0D0A7269` | Text/data (`\r\nri`), not a canonical runtime pointer |

A complete raw-word sweep of all three canonical ROMs finds no `0xA4BC` word. Consequently no handler bytes, bounds, arguments, return value, globals, helpers, or function prototype can be attributed to this slot.

The official SmartApplet corpus was checked structurally. Each of the 16 EXTENDED applets retains its demonstrated late table tail and none contains an `A4BC` stub there; 14 LEGACY applets terminate before that late range and 11 corpus members have no A-line table, yielding 41/41 without an `A4BC` stub.

Only one raw `A4BC` byte pair exists in the 16 EXTENDED binaries: AcceleratedReader at `+0x74D8`. It is even-aligned and therefore was not discarded by parity. Exact instruction context is `2F00 207C FFFF A4BC 4EBB 88FE`; opcode `0x207C` begins at `+0x74D4` and consumes the 32-bit immediate `0xFFFFA4BC` at `+0x74D6..+0x74D9`. The raw `A4BC` pair is thus operand data inside a `MOVEA.L #imm32,A0`, not an A-line opcode or caller.

Static adversarial regression: **85/85 PASS** after correcting a preflight-only slice-length assertion and rerunning the full regression cleanly. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4BC`. It assigns no vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics. The historical meaning of the data occupying the nominal slot remains unknown.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
