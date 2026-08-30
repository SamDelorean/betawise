# Forward-sweep trace — A3C4 / index 241

- Status: mechanically reconstructed A; blocked only on contractual return intent.
- AS3000 2005 raw slot `0x004E004E` lands inside an existing instruction extension word, not at a handler boundary.
- NEO 2005 raw slot `0x004E005E` is outside the mapped ROM runtime range.
- NEO 2013 handler: runtime `0x0043DAE2`, file `0x02DAE2`, length `0x1A`, SHA-256 `7619bfc511b2f496744dc3cc0db4f3b7a27095e604f30824fc5d5d006f6b29d9`.
- Physical input: one 32-bit pointer-like slot, confirmed by seven official executable callers.
- Side effects: conditional storage of that longword plus state-byte update; semantic names remain unknown.
- D0: never defined by the handler; all seven official callers clobber it before use. `void` is very strongly inferred but not independently confirmed, so no callable prototype is published.
- Official corpus: 41/41; seven callers in seven extended applets.
- Static regression: OVERALL PASS. Dynamic regression: not executed.

Public disposition: [`sys-a3c4-blocked.md`](sys-a3c4-blocked.md).

Detailed firmware and caller evidence remains private in Drive.
