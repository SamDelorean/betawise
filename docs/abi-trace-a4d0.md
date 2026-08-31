# SYS_A4D0 (index 308)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 308 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0EC` | `0x09022000` | Not a valid firmware runtime pointer |
| NEO (2005) | `+0x031A16` | `0x09022000` | Not a valid firmware runtime pointer |
| NEO (2013) | `+0x03F62E` | `0x06BA0044` | Not a valid canonical runtime pointer |

All three canonical firmware images contain zero raw `0xA4D0` words.

The official SmartApplet corpus was checked structurally. Each of the 16 EXTENDED applets retains exactly one demonstrated late table tail `A450,A454,A468,A46C`, and none contains an `A4D0` stub in the demonstrated late-table window. The remaining 14 LEGACY applets terminate before the late range and 11 corpus members have no A-line table, yielding 41/41 without an `A4D0` stub or caller.

The complete raw-word inventory across the 16 EXTENDED applets contains 16 `A4D0` byte pairs. Five occurrences outside SpellCheck Large and WirelessUpdate are at odd file offsets and therefore cannot begin an aligned 68000 opcode. The nine SpellCheck Large occurrences are all after its demonstrated terminal-RTS executable boundary at `+0x438A`. The two WirelessUpdate occurrences are after its demonstrated code/table boundary at `+0x1791` and lie in non-68000 payload/embedded-firmware material. None is promoted to an A-line caller.

Static adversarial regression: **82/82 PASS**: 12/12 canonical-ROM preflight plus 70/70 EXTENDED-corpus and adversarial checks. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4D0`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the data occupying the nominal slot remains unknown.

Private ROM material, complete raw inventories and detailed reverse-engineering workpapers remain outside the repository.
