# Source-first trace — A3D0 / index 244

Status: **SOURCE_FIRST / MECÁNICA_RECONSTRUIDA A / RETORNO_CONTRACTUAL=DESCONOCIDO**. No callable prototype is asserted.

- Correlation first: BetaWise and `ioma8/neo-re` preserve only the neutral mapping `index 244 = SYS_A3D0`; checked header/debug sources provide no independent vendor prototype or semantic name.
- Fresh primary pass reproduced all three canonical ROM SHA-256 values and the generation-specific slots.
- AS3000 2005 and NEO 2005 remain non-pointer/unresolved; no NEO 2013 contract is projected backward.
- NEO 2013: the real handler length/hash and its private helper length/hash were freshly reproduced.
- Physical ABI is **CONFIRMADO** as two 32-bit stack slots: slot 1 is consumed as a full pointer/source; only low16 of slot 2 participates as count/length.
- The implementation defines full D0 on every exit path, including literals 8/9/10 and zero-extended successful length. Those values are mechanical observations, not a recovered enum/error contract.
- Fresh firmware xrefs reproduce zero absolute JSR/JMP and exactly one direct BSR.W caller. That caller removes 8 bytes of arguments and immediately branches without consuming D0.
- Official SmartApplets remain a structural negative from the previously executed 41/41 table-tail survey: no official applet physically exposes A3D0.
- Return type, width, signedness and semantic intent therefore remain **DESCONOCIDO** despite uniform machine-level D0 definition.
- Static regression: prior integral **EJECUTADA / OVERALL PASS (30/30)**; directed source-first primary revalidation **EJECUTADA / PASS**. Dynamic regression remains **ESPECIFICADA / NO EJECUTADA**.

Detailed ROM mappings, CFG/disassembly, helper/global analysis and caller bytes remain private in Drive.
