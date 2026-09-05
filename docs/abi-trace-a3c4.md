# Source-first trace — A3C4 / index 241

- Status: **SOURCE_FIRST / MECHANICALLY_RECONSTRUCTED A / contractual return UNKNOWN**. No callable prototype is published.
- Correlation first: BetaWise and `ioma8/neo-re` independently retain only the neutral mapping `index 241 = SYS_A3C4`; the available `os3k.h` standard-library declarations stop at `ungetc`, and the checked DebugTool source provides no A3C4 prototype or semantic name. These are anchors/negative evidence, not primary proof.
- Fresh primary revalidation reproduced all three canonical ROM SHA-256 values before extraction.
- AS3000 2005 raw slot `0x004E004E` maps into the extension word of an existing instruction, not a handler boundary.
- NEO 2005 raw slot `0x004E005E` is below the mapped ROM runtime base.
- NEO 2013 has a real 26-byte handler; its previously recorded handler SHA-256 was reproduced exactly.
- Physical input: one 32-bit pointer-like slot, freshly rechecked in seven official executable callers.
- Side effects: if the shared state byte is zero, the handler stores that longword and changes the state to literal `5`; if state is already non-zero, it returns without mutation. Semantic names remain unknown.
- D0 is never defined by the handler. All seven callers overwrite D0 before use. `void` remains **INFERENCIA MUY FUERTE**, not CONFIRMADO; return intent is **DESCONOCIDO**.
- Fresh direct firmware search again found no absolute JSR/JMP or BSR.W reference to the NEO 2013 handler.
- Static regression: prior integral regression **EJECUTADA / OVERALL PASS**; directed source-first primary revalidation **EJECUTADA / PASS**. Dynamic regression remains **ESPECIFICADA / NO EJECUTADA**.

Public disposition: [`sys-a3c4-blocked.md`](sys-a3c4-blocked.md).

Detailed firmware bytes, addresses of private globals, caller contexts, and workpapers remain private in Drive.
