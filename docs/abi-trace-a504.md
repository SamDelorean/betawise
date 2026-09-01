# SYS_A504 (index 321)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K nominal slot at index 321 does not establish a callable firmware handler in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B120` | `0x0A000705` | Outside demonstrated firmware runtime range |
| NEO (2005) | `+0x031A4A` | `0x0A000705` | Outside demonstrated firmware runtime range |
| NEO (2013) | `+0x03F662` | `0x060A0044` | Outside demonstrated firmware runtime range |

All three canonical firmware images contain zero raw `0xA504` byte pairs.

The official SmartApplet corpus was verified specifically for A504 as **16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41**. The 16 EXTENDED binaries were revalidated by exact SHA-256 and by their demonstrated late executable prefix `A450,A454,A468,A46C`; no late executable window contains A504. Raw A504 occurrences are confined to SpellCheck and Thesaurus dictionary/payload regions after their demonstrated terminal `RTS` boundaries (`+0x438A` for SpellCheck, `+0x5414` for Thesaurus). The 14 LEGACY binaries preserve their exact `A08C..A308` A-line sequence and contain no A504 before or inside that table; the 11 NO_TABLE binaries contain no raw A504 at all.

Static adversarial regression: **185/185 PASS**, exit 0 with empty stderr. Dynamic regression is not applicable because no callable entrypoint is established.

This trace intentionally retains only the neutral identifier `SYS_A504`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal index-321 slot data remains unknown.

Private ROM material, exact per-file hashes and raw inventories, regression source/output and detailed reverse-engineering workpapers remain outside the repository.
