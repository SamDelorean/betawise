# A050-A068 null-vector range — source-first closure

Status: `CLOSED_MECHANICAL / REVALIDATED_SOURCE_FIRST`.

The canonical A-line tables were re-read directly from the three canonical ROMs and confirm that indexes 20 through 26 are all null in every generation:

- index 20 / A050 = `0x00000000`
- index 21 / A054 = `0x00000000`
- index 22 / A058 = `0x00000000`
- index 23 / A05C = `0x00000000`
- index 24 / A060 = `0x00000000`
- index 25 / A064 = `0x00000000`
- index 26 / A068 = `0x00000000`

This holds for AlphaSmart 3000 2005, NEO 2005, and NEO 2013.

Source-first correlation agrees with the primary evidence. `os3k/syscall.c` jumps from index 19 (`RasterOp`) directly to index 27 (`SYS_A06C`), and the SDK/header material provides no names, wrappers, or prototypes for indexes 20-26. The validated AlphaWordPlus A-line veneer blocks likewise skip this range: the initial block ends at A03C and the next imported block begins at A06C.

Boundary controls rule out a table-stride or alignment mistake: index 18/A048 is non-null in all three ROMs, index 19/A04C is implemented only in NEO 2013, and index 27/A06C is non-null in all three ROMs.

Therefore these seven entries are documented as **null vectors**, not as callable `SYS_Axxx()` functions. Their historical/vendor intent remains unknown; no vendor names or synthetic prototypes are invented.

The static regression was executed and passed, checking all three canonical ROM hashes, all seven slots in all three ROMs, and neighboring boundary entries. Firmware bytes, ROM images, and extended disassembly remain private.
