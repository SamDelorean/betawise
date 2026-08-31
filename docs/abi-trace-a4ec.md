# SYS_A4EC (index 315)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 315 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B108` | `0x09210001` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A32` | `0x09210001` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F64A` | `0x09F80044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA4EC` words.

The stable official corpus was reconciled as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41. The 16 EXTENDED binaries were rematerialized and revalidated against their canonical SHA-256 values; each retains exactly one demonstrated late tail `A450,A454,A468,A46C`, and no late-tail window contains `A4EC`.

Seven raw `A4EC` byte occurrences were found outside those tables, all in the Spanish-to-English thesaurus payload. They were not discarded merely by alignment: the three thesaurus variants share an executable prefix ending in a terminal `RTS` at file offset `+0x5414`, retain the same field at `+0x5416`, and diverge from `+0x5418`. The shared executable prefix contains no `A4EC`; all seven raw occurrences lie after that demonstrated payload boundary. No official executable caller was promoted.

Static adversarial regression: **86/86 PASS**. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4EC`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal slot data remains unknown.

Private ROM material, exact raw offsets, regression source/output and detailed reverse-engineering workpapers remain outside the repository.
