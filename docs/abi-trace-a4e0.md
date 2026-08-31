# SYS_A4E0 (index 312)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 312 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B0FC` | `0x01010409` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A26` | `0x01010409` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F63E` | `0x080E0044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA4E0` words.

The stable official corpus was revalidated as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41. The 16 EXTENDED binaries were rematerialized and revalidated against their canonical SHA-256 manifest. Each retains exactly one demonstrated late tail `A450,A454,A468,A46C`; no late-tail window contains `A4E0`.

Ten raw `A4E0` byte occurrences were found outside those tables: one in `thesaurus_small_usa` and nine in `wirelessupdate`. Each lies beyond a previously demonstrated executable/code-table frontier and is therefore classified as non-executable payload rather than a stub or caller. No official executable caller was promoted.

Static adversarial regression: **82/82 PASS**. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4E0`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal slot data remains unknown.

Private ROM material, complete raw inventories and detailed reverse-engineering workpapers remain outside the repository.
