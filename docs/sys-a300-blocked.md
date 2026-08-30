# SYS_A300 — blocked contractual return

Status: **mechanics reconstructed; callable contract intentionally not published**.

`SYS_A300` has been reconstructed from the three canonical OS3K firmware generations used by this project. The handler has one physical 32-bit input slot used as a pointer. Its reachable body is 0x7A bytes in AS3000 2005, NEO 2005, and NEO 2013, and the three bodies are mechanically equivalent after normalizing call relocations.

The routine reads state-relative fields, performs a bounded transfer/update chain through private helpers, and terminates by invoking `SYS_A2F0` with the equivalent physical values `(zero_extend(state.word0), state, 1)`. The full D0 value returned by `SYS_A2F0` therefore reaches the `SYS_A300` RTS mechanically.

That mechanical D0 propagation is **not sufficient evidence of a public return contract**. The exhaustive official SmartApplet sweep covers 41 binaries: the only executable A300 callers are four AlphaWordPlus 2005 sites and four equivalent AlphaWordPlus NEO 2012 sites; the other 39 applets are negative. None of the eight callers consumes, tests, or preserves A300's D0 as a result before subsequent code makes that value irrelevant or clobbers it. No independent historical prototype or functional symbol has been recovered.

Accordingly, `void SYS_A300(void *state)` is a very strong inference but remains unconfirmed. Publishing it as a callable ABI would turn absence of observed consumption into a fact, contrary to the project methodology and to comparable blocked cases such as A2B0, A2E4, A2EC, and A2F8.

The private static regression passed and verifies canonical firmware hashes, handler boundaries and hashes, normalized cross-generation equivalence, the one-slot physical ABI, state-field accesses, the terminal A2F0 call shape, and the complete 41/41 caller accounting. Dynamic/emulator regression is specified but has not been executed.

Firmware bytes, extensive disassembly, caller manifests, and other primary workpapers remain private in Drive. This public note records only the neutral disposition and the reason a callable contract is deliberately withheld.
