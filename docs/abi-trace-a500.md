# SYS_A500 (index 320)

Status: mechanically closed; no callable ABI contract identified.

The canonical OS3K nominal slot at index 320 does not establish a callable firmware handler in any of the three canonical ROMs used by this project:

| Platform | File slot | Slot value | Disposition |
| --- | ---: | --- | --- |
| AlphaSmart 3000 (2005) | `+0x02B11C` | `0x81020800` | Not promoted to a firmware handler |
| NEO (2005) | `+0x031A46` | `0x82024000` | Not promoted to a firmware handler |
| NEO (2013) | `+0x03F65E` | `0x060A0044` | Not promoted to a firmware handler |

Raw `A500` occurrences in the three firmware images were classified individually rather than discarded by parity. Even-offset occurrences resolve to operands of `PEA`/`JSR` or to data/table regions; the remaining candidates are unaligned byte occurrences. None is promoted as an executable `A500` trap.

The official SmartApplet corpus was re-established from primary binaries as **16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41**. The 14 LEGACY binaries each contain one monotonic A-line table segment `A08C..A308`; no `A500` byte pair occurs before or inside that table in any of them, and all raw occurrences, when present, lie after its end. The 11 NO_TABLE binaries contain neither that table segment nor any raw `A500` bytes. The 16 EXTENDED binaries were already revalidated by exact SHA-256 and separately checked for executable `A500` candidates; no caller was promoted.

Adversarial regression is composite by design so already reproducible evidence is not rerun unnecessarily. The ROM+16 EXTENDED stage is **86/86 PASS**. The newly executed 14 LEGACY+11 NO_TABLE stage is **149/149 PASS**, exit 0 with empty stderr. Together they cover the official corpus 41/41.

This trace intentionally retains only the neutral identifier `SYS_A500`. It assigns no vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem semantics. The historical meaning of the nominal index-320 slot data remains unknown.

Private ROM material, exact per-file hashes and table offsets, raw-hit inventories, regression source/output and detailed reverse-engineering workpapers remain outside the repository.
