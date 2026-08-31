# SYS_A4CC (index 307)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 307 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0E8` | `0x04A02800` | Not a valid firmware runtime pointer |
| NEO (2005) | `+0x031A12` | `0x04A0C800` | Not a valid firmware runtime pointer |
| NEO (2013) | `+0x03F62A` | `0x06120044` | Not a valid canonical runtime pointer |

The raw-word inventory requires special handling. AlphaSmart 3000 and NEO 2005 contain no `0xA4CC` word. NEO 2013 contains two raw pairs at file offsets `+0x9DE8` and `+0x9FE4`, but in both cases the exact bytes are `4E B9 00 41 A4 CC`: the instruction is an absolute-long `JSR` whose 32-bit operand is `0x0041A4CC`. The raw `A4CC` pair is therefore operand data, not an A-line opcode.

The official SmartApplet corpus was checked structurally. Each of the 16 EXTENDED applets retains exactly one demonstrated late table tail `A450,A454,A468,A46C`, and none contains an `A4CC` stub in that late-table window. The remaining 14 LEGACY applets terminate before the late range and 11 corpus members have no A-line table, yielding 41/41 without an `A4CC` stub.

Only one raw `A4CC` pair occurs in the 16 EXTENDED binaries: AlphaWordPlus `+0xBFF0`. Exact context is `20 7C FF FF A4 CC 4E BB 88 FE`: opcode `0x207C` begins at `+0xBFEC` and consumes the immediate long `0xFFFFA4CC`; the next opcode begins at `+0xBFF2`. This is operand data, not an A-line caller.

Static adversarial regression: **79/79 PASS**. Dynamic regression is not applicable because no callable contract is established.

This trace intentionally retains only the neutral identifier `SYS_A4CC`. It assigns no vendor name, prototype, arguments, return semantics, enums, flags, errors, or subsystem semantics. The historical meaning of the data occupying the nominal slot remains unknown.

Private ROM material and detailed reverse-engineering workpapers remain outside the repository.
