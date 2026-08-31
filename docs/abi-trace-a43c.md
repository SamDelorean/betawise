# ABI trace — A43C

Public trace for the mechanically reconstructed but contract-blocked OS3K A-line entry `A43C`.

- Vector ordinal/index: 271.
- AS3000 2005: non-pointer/unresolved vector value; no callable handler attributed.
- NEO 2005: non-pointer/unresolved vector value; no callable handler attributed.
- NEO 2013: callable handler; exact private bounds and hashes are retained in the correlated workpaper.
- External stack evidence: five 32-bit slots; slot 4 is used through its low byte.
- Additional entry channel: incoming `A1` is preserved and used as the destination for a terminal six-byte copy.
- Aggregate layout evidence: byte at offset 0, one uninitialized/padding-compatible byte at offset 1, 32-bit value at offset 2.
- Return-register evidence: `D0.L` holds the 32-bit delta at exit, but contractual scalar-return status is unproven.
- Direct firmware xrefs to the handler in the NEO 2013 image: none found by the reproducible direct JSR/JMP/BSR sweep.
- Related contiguous family: A440, A444, A448, A44C reproduce the preserved-`A1` plus terminal-six-byte-copy pattern.
- Static regression: primary A43C checks and a helper-core supplement both passed; dynamic/emulator validation has not been used to promote the unresolved ABI interpretation.

Disposition: **MECANICA_RECONSTRUIDA A / BLOQUEADO**. No callable header is emitted until independent caller/prototype evidence discriminates hidden structure return from a proprietary `A1` output convention and clarifies `D0`.

See `docs/sys-a43c-blocked.md` for the public adversarial analysis and consolidation clues.
