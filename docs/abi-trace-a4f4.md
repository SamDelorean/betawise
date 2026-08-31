# SYS_A4F4 (index 317)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 317 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B110` | `0x00000705` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A3A` | `0x00000705` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F652` | `0x0AAA0044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA4F4` words.

The official corpus was reconciled as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41. The 16 EXTENDED binaries were revalidated against their canonical SHA-256 values and the demonstrated late-tail prefix `A450,A454,A468,A46C`; no late-tail window contains `A4F4`.

Raw `A4F4` byte occurrences outside those tables were confined to payload-bearing regions: two in `spellcheck_large_usa`, two in `thesaurus_large_usa`, one in `thesaurus_small_usa`, and three in `thesaurusspanishtoeng`. They were classified using demonstrated structural boundaries rather than alignment alone. Both SpellCheck occurrences are after the executable boundary at `+0x438A`. All six Thesaurus occurrences are after the shared executable prefix / variant-payload boundary at `+0x5418`. The corresponding executable prefixes contain no `A4F4`, so no official executable caller was promoted.

Static adversarial regression: **85/85 PASS**. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4F4`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal slot data remains unknown.

Private ROM material, exact raw inventories, regression source/output and detailed reverse-engineering workpapers remain outside the repository.
