# ABI trace — A3AC / `toupper`

- Index: 235 / A3AC
- Status: **CLOSED A / SOURCE_FIRST / PUBLISHED**
- Contract: `int toupper(int c)`
- Source anchors: BetaWise `syscall.c`, `os3k.h` / `os3k.pdf`, plus earlier A2CC helper correlation; firmware remains authoritative
- Handler: 0x1A bytes, byte-identical AS3000 2005 / NEO 2005 / NEO 2013
- Handler SHA-256: `517641aebad0e001ab132b7fa7064ebfe864a3d2549e25dc042c8777d1ead5e3`
- Canonical runtime/file entries: AS `0x004DE7D8/+0x01E7D8`; NEO05 `0x005E1138/+0x021138`; NEO13 `0x00437536/+0x027536`
- ABI: one full 32-bit argument; full `D0.L` result; ASCII `a..z` -> `A..Z`; all other longwords unchanged
- Side effects: none; no helpers, globals, tables or writes
- Official SmartApplet corpus: 41/41; 57 executable calls in 20 applets; 10 table-bearing negatives + 11 structural negatives
- Firmware direct xrefs: fresh pass reproduced JSR 14 / 14 / 15 and direct JMP 0 / 0 / 0
- Full static regression: previously **EXECUTED / OVERALL PASS**
- Fresh targeted primary regression (2026-09-05): **EXECUTED / PASS** for ROM identity, handler identity and direct JSR/JMP references
- Dynamic emulator-first regression: **SPECIFIED / NOT EXECUTED**
- Adversarial alternatives rejected: `tolower`, predicate, byte-only, locale/table-driven, and proof-by-A2CC projection
- Existing `os3k.h` prototype already matches; no header change

Private ROM/disassembly/caller/regression evidence remains in Drive and is not published here. See `docs/toupper-closure.md` for the public functional closure.
