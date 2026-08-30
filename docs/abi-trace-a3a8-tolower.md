# ABI trace — A3A8 / `tolower`

- Trap/index: A3A8 / 234
- Reconstructed contract: `int tolower(int c)`
- Mechanical status: A / published
- Public closure: [`tolower-closure.md`](tolower-closure.md)
- Canonical ROM coverage: AS3000 2005, NEO 2005, NEO 2013
- Handler: 0x1A bytes, byte-identical cross-ROM, SHA-256 `467c88933f60a216b6234efaf42d20120662dd892d48470aacac33e4bb87f837`
- ABI: one 32-bit argument; full `D0.L` result
- Mechanics: ASCII `A..Z` -> `a..z`; all other longwords unchanged
- Official SmartApplet corpus: 41/41; 39 executable calls in 13 applets
- Direct firmware xrefs: 6 JSR per canonical ROM; no JMP/BSR entry use
- Static regression: OVERALL PASS
- Dynamic emulator-first regression: specified, not executed

The private Drive evidence retains exact ROM hashes, separate instruction-level listings for each ROM, full caller manifest with machine-level use, direct firmware xrefs, adversarial analysis, and regression source/output. Public documentation intentionally omits firmware bytes and extensive disassembly.
