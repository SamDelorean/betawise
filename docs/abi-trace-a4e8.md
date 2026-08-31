# SYS_A4E8 (index 314)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K slot at index 314 does not contain a valid firmware runtime pointer in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B104` | `0x02010003` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A2E` | `0x02010003` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F646` | `0x060A0044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA4E8` words.

The stable official corpus was revalidated as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41. The 16 EXTENDED binaries retain their canonical SHA-256 values and exactly one demonstrated late tail `A450,A454,A468,A46C`; no late-tail window contains `A4E8`.

Four raw `A4E8` byte occurrences were found outside those tables, all in `thesaurus_small_usa`. Every occurrence is at an odd file offset and is therefore incompatible with an aligned m68k/A-line opcode start. No official executable caller was promoted.

Static adversarial regression: **82/82 PASS**. The regression harness also failed closed on an intentionally retained first run when a copied expected raw-count assertion still expected three hits rather than the observed four; after correcting only that expected count, the complete run passed. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A4E8`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal slot data remains unknown.

Private ROM material, complete raw inventories and detailed reverse-engineering workpapers remain outside the repository.
