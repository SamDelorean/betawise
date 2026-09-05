# Source-first trace — A3CC / index 243

- Status: **SOURCE_FIRST / MECHANICALLY_RECONSTRUCTED A / contractual return UNKNOWN**.
- Correlation first: BetaWise and `ioma8/neo-re` retain only the neutral mapping `index 243 = SYS_A3CC`; checked public/header/debug sources provide no independent vendor prototype or semantic name.
- Fresh primary pass reproduced all three canonical ROM SHA-256 values.
- AS3000 2005 raw slot `0x5AB6004E` and NEO 2005 raw slot `0x7FC0005E` remain non-pointer/unresolved.
- NEO 2013 has a real 0xD6-byte handler; its archived SHA-256 was reproduced exactly.
- Physical input ABI: zero caller arguments.
- Fresh handler correlation reproduces two BSR calls to A3B8 and two BSR calls to A3B4.
- The implementation deliberately constructs a terminal D0.B domain `{0,1}`, but all three direct firmware callers ignore/overwrite D0. A contractual `uint8_t` return therefore remains **INFERENCIA FUERTE**, not CONFIRMADO.
- Fresh firmware xrefs: exactly three absolute JSR callers, zero absolute JMP and zero BSR.W callers.
- The archived 41/41 SmartApplet survey remains structurally negative because A3CC is not physically exposed as an applet A-line slot.
- Static regression: prior integral regression **EJECUTADA / OVERALL PASS**; directed source-first primary revalidation **EJECUTADA / PASS**. Dynamic regression remains **ESPECIFICADA / NO EJECUTADA**.

Public disposition: [`sys-a3cc-blocked.md`](sys-a3cc-blocked.md).

No callable prototype or invented vendor name is published. Detailed firmware/disassembly evidence remains private in Drive.
