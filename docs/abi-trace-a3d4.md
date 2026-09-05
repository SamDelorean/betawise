# Source-first trace — A3D4 / index 245

- Status: **SOURCE_FIRST / MECHANICALLY_RECONSTRUCTED A / contractual return UNKNOWN**.
- Correlation first: BetaWise and `ioma8/neo-re` preserve only the neutral mapping `index 245 = SYS_A3D4`; checked header/debug sources provide no independent vendor prototype or semantic name.
- Fresh primary pass reproduced all three canonical ROM hashes and the three generation-specific slot values.
- AS3000 2005 and NEO 2005 remain non-pointer/unresolved.
- NEO 2013 has a real 0x3A-byte handler; its archived SHA-256 and the independent function boundary immediately following it were reproduced exactly.
- Physical input ABI: one full 32-bit source-pointer slot. The copy count is internal state, not another caller argument.
- Direct dependency: A360/`memcpy`; destination is an internal buffer and the count is assembled from two state bytes.
- Mechanical terminal D0.L retains the upper word from `memcpy` while replacing the low word with count. This residual dataflow is **CONFIRMADO** but is not evidence for a C return width or intent.
- Fresh NEO 2013 direct xrefs: JSR/JMP/BSR.W = 0/0/0. The previously executed official 41/41 structural survey remains negative because applet tables stop before A3D4.
- `uint16_t`, residual `uint32_t`, and `void` return hypotheses all remain unconfirmed; contractual return is **DESCONOCIDO**.
- Static regression: prior integral **EJECUTADA / OVERALL PASS**; directed source-first primary revalidation **EJECUTADA / PASS**. Dynamic regression remains **ESPECIFICADA / NO EJECUTADA**.

Public disposition: [`sys-a3d4-blocked.md`](sys-a3d4-blocked.md).

Detailed firmware/disassembly and private-state addresses remain private in Drive.
