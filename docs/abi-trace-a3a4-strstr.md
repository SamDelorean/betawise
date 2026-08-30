# ABI trace — A3A4 / `strstr`

This trace links the public forward-sweep record for A3A4 to the private evidence set without publishing firmware bytes or extensive disassembly.

- Trap/index: A3A4 / 233
- Reconstructed contract: `char *strstr(const char *haystack, const char *needle)`
- Mechanical status: A / published
- Public closure: [`strstr-closure.md`](strstr-closure.md)
- Canonical ROM coverage: AS3000 2005, NEO 2005, NEO 2013
- Handler: 0x48 bytes in each ROM; relocation-normalized SHA-256 `5587602b25c9085b2a43cce4b05eefe7db0e93897142751f65ba78e3d53b2dbe`
- Private bounded-comparison helper: 0x3A bytes, byte-identical cross-ROM, SHA-256 `494834eaa7a797f952e4b7612b64fe08759217596a56bf8cf12428d045740cb9`
- Official SmartApplet corpus: 41/41; 11 executable calls in three NEO applets
- Direct firmware JSR/JMP/BSR xrefs: none in all three canonical ROMs
- Static regression: OVERALL PASS
- Dynamic emulator-first regression: specified, not executed

The private Drive evidence retains exact ROM hashes, raw handler hashes, separate instruction-level listings for all three ROMs, helper listing, all caller offsets and machine-level argument/return use, adversarial analysis, and regression source/output. Public material intentionally omits firmware bytes and extensive disassembly.
