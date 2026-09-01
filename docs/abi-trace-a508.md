# SYS_A508 (index 322)

Status: mechanically closed; no callable ABI contract identified.

The nominal index-322 slots in the canonical ROMs are `0x01020800` (AlphaSmart 3000), `0x01024000` (NEO 2005), and `0x060A0044` (NEO 2013), all outside the demonstrated firmware runtime ranges. The three ROMs contain zero raw `A508` byte pairs.

The official SmartApplet corpus was checked specifically for A508 as **16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41**. No executable caller was promoted. In the EXTENDED set, non-Thesaurus raw occurrences are unaligned byte matches; Thesaurus occurrences are after the demonstrated terminal `RTS` at `+0x5414`. In the LEGACY set, dictionary occurrences are after the legacy table. The one material pre-table even occurrence, `AlphaWordPlus+0x7D7C`, was inspected directly: bytes `20 7C 00 00 A5 08 4E BB` frame `A508` as the immediate operand of a `MOVEA.L` instruction, not as an A-line opcode. The 11 NO_TABLE binaries contain no raw A508.

Static adversarial regression: **186/186 PASS**, exit 0 with empty stderr. Dynamic regression is not applicable because no callable entrypoint is established.

Only the neutral identifier `SYS_A508` is retained. No vendor name, prototype, arguments, return semantics, globals, helpers, enums, flags, errors, structures, or subsystem meaning is asserted. The historical meaning of the nominal slot data remains unknown.

Private ROM material, exact hashes/raw inventories, regression source/output and detailed reverse-engineering workpapers remain outside the repository.
