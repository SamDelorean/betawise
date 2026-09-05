# Source-first trace — A3C8 / index 242

- Status: **SOURCE_FIRST / MECHANICALLY_RECONSTRUCTED A / callable contract unresolved**.
- Correlation first: BetaWise and `ioma8/neo-re` expose only the neutral mapping `index 242 = SYS_A3C8`; the checked `os3k.h` API declarations and DebugTool source provide no independent A3C8 prototype or semantic name.
- Fresh primary pass reproduced the canonical ROM SHA-256 values before using the slots.
- AS3000 2005 raw slot `0x5A0E004E` and NEO 2005 raw slot `0x7F10005E` remain non-pointer/unresolved. No NEO 2013 contract is projected backward.
- NEO 2013 has a real 32-byte handler; the archived handler SHA-256 was reproduced exactly.
- One full 32-bit stack slot is read and stored verbatim on the active path. Its semantic C type remains **DESCONOCIDO** because no executable caller has been recovered.
- The private helper is 0x76 bytes with a closed CFG; its archived SHA-256 was reproduced exactly. Fresh firmware search again found exactly one absolute JSR to that helper, from A3C8.
- D0 is not uniform across handler paths: the state-nonzero path leaves incoming D0 untouched; the active path preserves the final helper-chain D0. No return contract is inferred.
- Fresh direct NEO 2013 search: A3C8 absolute JSR/JMP = 0/0 and BSR.W = 0. The archived 41/41 official-corpus regression remains the authoritative negative caller survey: executable callers = 0.
- Static regression: prior integral regression **EJECUTADA / OVERALL PASS**; directed source-first primary revalidation **EJECUTADA / PASS**. Dynamic regression remains **ESPECIFICADA / NO EJECUTADA**.

Public disposition: [`sys-a3c8-blocked.md`](sys-a3c8-blocked.md).

Detailed firmware/helper bytes and private-state addresses remain private in Drive.
