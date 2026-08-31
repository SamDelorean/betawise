# SYS_A4F0 (index 316)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 316 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B10C` | `0x0101223F` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A36` | `0x0101223F` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F64E` | `0x0A240044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA4F0` words.

The official corpus was reconciled as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41. The 16 EXTENDED binaries were revalidated against their canonical SHA-256 values, retain the demonstrated late-tail prefix `A450,A454,A468,A46C`, and contain no `A4F0` in the late-tail windows.

Raw `A4F0` byte occurrences outside those tables were confined to three payload-bearing binaries: 41 in `spellcheck_large_usa`, two in `thesaurusspanishtoeng`, and seven in `wirelessupdate`. They were classified using demonstrated structural boundaries rather than alignment alone: all SpellCheck occurrences are after its executable boundary at `+0x438A`; both Spanish thesaurus occurrences are after its variant-payload boundary at `+0x5418`; and all WirelessUpdate occurrences are after its code/table boundary at `+0x1791`. The corresponding executable prefixes contain no `A4F0`, so no official executable caller was promoted.

Static adversarial regression: **83/83 PASS**. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4F0`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal slot data remains unknown.

Private ROM material, exact raw inventories, regression source/output and detailed reverse-engineering workpapers remain outside the repository.
