# ABI trace — A3AC / `toupper`

- Index: 235 / A3AC
- Status: MECÁNICA_CERRADA A / PUBLICADO / COMPLETADO
- Contract: `int toupper(int c)`
- Handler: 0x1A bytes, byte-identical AS3000 2005 / NEO 2005 / NEO 2013
- Handler SHA-256: `517641aebad0e001ab132b7fa7064ebfe864a3d2549e25dc042c8777d1ead5e3`
- Canonical runtime/file entries: AS `0x004DE7D8/+0x01E7D8`; NEO05 `0x005E1138/+0x021138`; NEO13 `0x00437536/+0x027536`
- Official SmartApplet corpus: 41/41; 57 executable calls in 20 applets; 10 table-bearing negatives + 11 structural negatives
- Firmware direct JSR/JMP/BSR: 14/0/0, 14/0/0, 15/0/0
- Static regression: OVERALL PASS
- Dynamic emulator-first regression: specified / not executed
- Existing `os3k.h` prototype already matches; no header change
- Private ROM/disassembly/caller/regression evidence remains in Drive and is not published here

See `docs/toupper-closure.md` for the public functional closure.
