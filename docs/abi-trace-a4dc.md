# SYS_A4DC (index 311)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 311 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0F8` | `0x00000103` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A22` | `0x00000103` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F63A` | `0x060A0044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA4DC` words.

The stable official corpus was revalidated as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41. The 16 EXTENDED binaries were revalidated against their canonical SHA-256 manifest. Each retains exactly one demonstrated late tail `A450,A454,A468,A46C`; no 64-byte late-tail window contains `A4DC`, and the complete raw `A4DC` inventory across all 16 EXTENDED applets is empty.

Static adversarial regression: **82/82 PASS**. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4DC`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal slot data remains unknown.

Private ROM material, complete raw inventories and detailed reverse-engineering workpapers remain outside the repository.
