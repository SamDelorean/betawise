# SYS_A4D8 (index 310)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 310 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0F4` | `0x28000904` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A1E` | `0xC8000904` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F636` | `0x07620044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA4D8` words.

The stable official corpus was revalidated as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41. The 16 EXTENDED binaries were rematerialized and matched their canonical SHA-256 manifest. Each retains exactly one demonstrated late tail `A450,A454,A468,A46C`, and no 64-byte late-tail window contains `A4D8`.

Across the 16 EXTENDED applets, 15 contain no raw `A4D8` word. `thesaurus_small_usa` contains exactly three byte-pair occurrences at `+0x14362`, `+0x19B80`, and `+0x1B7C8`. The same binary has a demonstrated terminal `RTS` at `+0x5414` and an executable/payload boundary at `+0x5418`; all three occurrences are therefore in later payload and are not promoted to executable 68000 A-line callers or stubs.

Static adversarial regression: **83/83 PASS**. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4D8`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal slot data remains unknown.

Private ROM material, complete raw inventories and detailed reverse-engineering workpapers remain outside the repository.
