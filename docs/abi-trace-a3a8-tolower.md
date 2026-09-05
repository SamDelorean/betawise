# ABI trace — A3A8 / `tolower`

- Trap/index: A3A8 / 234
- Reconstructed contract: `int tolower(int c)`
- Status: **CLOSED A / SOURCE_FIRST / PUBLISHED**
- Public closure: [`tolower-closure.md`](tolower-closure.md)
- Source anchors: historical `os3k.h`/`os3k.pdf`, BetaWise `syscall.c`, DebugTool caller; all treated as hypotheses before firmware verification
- Canonical ROM coverage: AS3000 2005, NEO 2005, NEO 2013; full canonical SHA-256 identities freshly reproduced on 2026-09-05
- Handler: 0x1A bytes, byte-identical cross-ROM, SHA-256 `467c88933f60a216b6234efaf42d20120662dd892d48470aacac33e4bb87f837`
- ABI: one full 32-bit argument; full `D0.L` result
- Mechanics: ASCII `A..Z` -> `a..z`; all other longwords unchanged; no helpers/globals/tables/writes
- Official SmartApplet corpus: 41/41; 39 executable calls in 13 applets
- Direct firmware xrefs: fresh targeted pass reproduced 6 absolute JSR per canonical ROM and 0 direct absolute JMP
- Full static regression: previously **EXECUTED / OVERALL PASS**
- Fresh targeted primary regression (2026-09-05): **EXECUTED / PASS** for ROM identity, handler identity and direct JSR/JMP xrefs
- Dynamic emulator-first regression: **SPECIFIED / NOT EXECUTED**
- Adversarial result: `toupper`, predicate, byte-only, locale/table-driven and A160-conflation alternatives rejected

The private Drive evidence retains exact ROM hashes, raw firmware bytes, separate instruction-level listings, the full caller manifest, direct firmware xrefs, regression source/output and adversarial notes. Public documentation intentionally omits raw firmware and extensive disassembly.
