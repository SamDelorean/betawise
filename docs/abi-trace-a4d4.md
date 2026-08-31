# SYS_A4D4 (index 309)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 309 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0F0` | `0x010104A0` | Not a valid firmware runtime pointer |
| NEO (2005) | `+0x031A1A` | `0x010104A0` | Not a valid firmware runtime pointer |
| NEO (2013) | `+0x03F632` | `0x060A0044` | Not a valid canonical runtime pointer |

All three canonical firmware images contain zero raw `0xA4D4` words.

The official SmartApplet corpus was checked structurally. The 16 EXTENDED applets were revalidated against the stable manifest, while 14 LEGACY applets terminate before the relevant late range and 11 corpus members have no A-line table, yielding 41/41 without an `A4D4` stub or caller.

Across the 16 EXTENDED applets, 15 contain no raw `A4D4` word. SpellCheck Large USA contains one byte-pair occurrence at `+0x8EB3`; it is at an odd file offset and lies after the demonstrated terminal-RTS executable boundary at `+0x438A`, so it is not promoted to an executable 68000 A-line opcode or caller. The demonstrated late table tail `A450,A454,A468,A46C` occurs once in each EXTENDED binary, and no 64-byte window around that tail contains `A4D4`.

Static adversarial regression: **82/82 PASS**. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4D4`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the data occupying the nominal slot remains unknown.

Private ROM material, complete raw inventories and detailed reverse-engineering workpapers remain outside the repository.
