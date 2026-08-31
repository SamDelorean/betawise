# SYS_A4E4 (index 313)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 313 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B100` | `0x04000002` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A2A` | `0x04000002` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F642` | `0x083A0044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA4E4` words.

The stable official corpus was revalidated as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41. The 16 EXTENDED binaries retain their canonical SHA-256 values and exactly one demonstrated late tail `A450,A454,A468,A46C`; no late-tail window contains `A4E4`.

Three raw `A4E4` byte occurrences were found outside those tables: two in `thesaurus_large_usa` and one in `thesaurus_small_usa`. All lie beyond the previously demonstrated Thesaurus executable/payload frontier at `+0x5418` and are classified as non-executable payload rather than stubs or callers. No official executable caller was promoted.

Static adversarial regression: **82/82 PASS**. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4E4`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal slot data remains unknown.

Private ROM material, complete raw inventories and detailed reverse-engineering workpapers remain outside the repository.
