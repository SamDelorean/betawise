# ABI trace — A3A0 / `strrchr`

This trace links the public forward-sweep record for A3A0 to the private evidence set without publishing firmware bytes or extensive disassembly.

- Trap/index: A3A0 / 232
- Reconstructed contract: `char *strrchr(const char *str, int c)`
- Mechanical status: A / published
- Public closure: [`strrchr-closure.md`](strrchr-closure.md)
- Canonical ROM coverage: AS3000 2005, NEO 2005, NEO 2013
- Handler: 0x1A bytes, byte-identical across the three canonical ROMs
- Handler SHA-256: `d894d16615ad73b1c65943ff49774f59a4c003be341726fb2159551636fa22eb`
- Official SmartApplet corpus: 41/41; two executable calls, both in NEO AlphaQuiz
- Direct firmware JSR/JMP/BSR xrefs: none in all three canonical ROMs
- Static regression: OVERALL PASS
- Dynamic emulator-first regression: specified, not executed

The private Drive workpaper retains the exact ROM hashes, offsets, instruction-level listing, caller offsets and arguments, adversarial analysis, regression source/output, and chronological reasoning. The public material intentionally omits firmware bytes and extended disassembly.
