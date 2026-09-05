# Source-first trace — A3D8 / index 246

- Disposition: **SOURCE_FIRST / MECÁNICA_RECONSTRUIDA A / RETORNO_CONTRACTUAL=DESCONOCIDO**.
- Correlation first: BetaWise and `ioma8/neo-re` preserve only `index 246 = SYS_A3D8`; checked header/debug material yields no independent prototype, vendor symbol, or semantic argument names.
- Fresh primary pass reproduced the canonical ROM hashes and generation-specific slots. AS3000 2005 and NEO 2005 remain non-pointer/unresolved; only NEO 2013 has a demonstrated handler.
- NEO 2013 handler length/hash were freshly reproduced: 0x178 bytes with the archived SHA-256 unchanged.
- Physical ABI is **CONFIRMADO** as four 32-bit slots with effective consumptions low16, low8, full32, and full32/pointer-like. Nominal meanings remain **DESCONOCIDO**.
- Reachable exits deliberately produce `D0.B` in `{0,3,5,8}`. The 3/5/8 paths define clean longword values via MOVEQ, while the normal zero path clears only D0.B, leaving upper bits potentially residual. A byte-sized return remains **INFERENCIA FUERTE**, not a confirmed source contract; `void` is also unsupported because explicit status bytes are constructed.
- Fresh NEO 2013 direct xrefs: JSR/JMP/BSR.W = 0/0/0. The previously executed official table-tail survey remains structurally negative because applet exposure ends before A3D8.
- Static regression: prior integral **EJECUTADA / OVERALL PASS (29/29)**; directed source-first primary revalidation **EJECUTADA / PASS**. Dynamic regression remains **ESPECIFICADA / NO EJECUTADA**.
- Public callable header remains intentionally unpublished.

Private ROM bytes, disassembly, helper/global correlation and regression workpapers remain outside the repository.
