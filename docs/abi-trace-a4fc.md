# SYS_A4FC (index 319)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 319 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B118` | `0x0A000705` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A42` | `0x0A000705` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F65A` | `0x060A0044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA4FC` words.

The official corpus was reconciled as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41. The 16 EXTENDED binaries were revalidated against their canonical SHA-256 values and the demonstrated late-tail prefix `A450,A454,A468,A46C`; no late-tail window contains `A4FC`.

Raw `A4FC` byte occurrences outside those tables were confined to payload-bearing regions: one in `thesaurusspanishtoeng` and two in `wirelessupdate`. The Spanish-thesaurus occurrence is after the variant-payload boundary at `+0x5418`; both WirelessUpdate occurrences are after the code/table boundary at `+0x1791`. The corresponding executable prefixes contain no `A4FC`, so no official executable caller was promoted.

Static adversarial regression: **81/81 PASS**. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4FC`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal slot data remains unknown.

Private ROM material, exact raw inventories, regression source/output and detailed reverse-engineering workpapers remain outside the repository.
