# ABI trace — A440

Public trace for the mechanically reconstructed but semantically blocked OS3K A-line entry `A440`.

- Vector ordinal/index: 272.
- AS3000 2005: non-pointer/unresolved vector value; no callable handler attributed.
- NEO 2005: non-pointer/unresolved vector value; no callable handler attributed.
- NEO 2013: callable handler; exact private bounds and hashes are retained in the correlated workpaper.
- External stack evidence: five 32-bit slots.
- Effective widths/roles: slot 1 readable two-long object; slot 2 pointer-like byte/NUL input; slot 3 writable byte buffer; slot 4 signed low-16 value; slot 5 writable 16-bit conditional output pointer.
- Additional result channel: incoming `A1` is preserved and used as the destination for a terminal six-byte aggregate copy.
- Aggregate layout evidence: byte at offset 0, one uninitialized/padding-compatible byte at offset 1, 32-bit field at offset 2.
- Direct firmware xrefs to the handler in NEO 2013: exactly one recovered `BSR.W`, from the A44C-family handler.
- Caller evidence: the caller prepares `A1` to a local six-byte object, pushes exactly five stack arguments, calls A440, discards 20 bytes of stack arguments, does not consume `D0`, and later forwards the same six-byte object to its own saved `A1`.
- `D0` scalar-return hypothesis: refuted as a coherent stable contract because the early and normal paths leave different internal values in `D0` while the contractual aggregate is independently formed through `A1`.
- Static regression: 48/48 PASS against the canonical NEO 2013 ROM, the recovered A44C caller, callback evidence, and the official AlphaWordPlus corpus artifact.
- Dynamic/emulator validation: not executed for semantic promotion.

Disposition: **MECANICA_RECONSTRUIDA A / BLOQUEADO_SEMANTICA**. The binary ABI shape is substantially established, including the contractual aggregate result through `A1`, but no callable C header is emitted until the source-level semantics of the five stack arguments and observed state/output fields are independently resolved.

See `docs/sys-a440-blocked.md` for adversarial analysis and consolidation clues.
