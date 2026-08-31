# ABI trace — A444

Public trace for the mechanically reconstructed but source-semantics-blocked OS3K A-line entry `A444`.

- Vector ordinal/index: 273.
- AS3000 2005: non-pointer/unresolved vector value; no callable handler attributed.
- NEO 2005: non-pointer/unresolved vector value; no callable handler attributed.
- NEO 2013: callable handler; exact private bounds and hashes are retained in the correlated workpaper.
- External stack evidence: four 32-bit slots.
- Effective roles: slot 1 points to a readable descriptor-like object with at least four 32-bit fields; slot 2 is a writable byte-buffer destination; slot 3 is consumed as a signed 16-bit bound; slot 4 is a writable pointer receiving a 16-bit output-count-like delta.
- Additional result channel: incoming `A1` is preserved and used as the destination for a terminal six-byte aggregate copy.
- Aggregate layout evidence: state byte at offset 0, one uninitialized/padding-compatible byte at offset 1, and a conditional 32-bit detail field at offset 2.
- Direct firmware xrefs in NEO 2013: exactly one recovered `BSR.W`, from the private wrapper beginning at `0x0044331C`.
- Caller evidence: the caller prepares `A1` to a local six-byte object, pushes exactly four stack arguments, sign-extends the effective 16-bit bound before pushing it, calls A444, discards 16 bytes of stack arguments, does not consume `D0`, and later forwards the same six-byte object to its own saved `A1`.
- Functional behavior: bounded single-byte XML/entity-reference decoding is confirmed from primary firmware evidence. Ordinary bytes are copied directly; `&...;` references are decoded for the predefined names `amp`, `lt`, `gt`, `apos`, `quot`, and numeric references beginning with `#`, including decimal and `x`-prefixed hexadecimal forms. Numeric results are emitted only when below `0x100`.
- The destination is NUL-terminated. Slot 4 receives a 16-bit produced-length-like delta on the normal path and is cleared on the early path.
- `D0` scalar-return hypothesis: refuted as a coherent stable contract because path-dependent internal values remain in `D0` while the recovered caller ignores it.
- Static regression: 61/61 PASS against canonical NEO 2013 ROM, caller, entity helper/table, output behavior, and official AlphaWordPlus corpus presence.
- Dynamic/emulator validation: not executed for semantic promotion.

Disposition: **MECANICA_RECONSTRUIDA A / BLOQUEADO_SEMANTICA_DESCRIPTOR**. The transformation behavior and binary ABI shape are substantially established, but no callable C header is emitted until the descriptor/result-state semantics can be represented without inventing source-level names or assuming the conditional aggregate detail field is always initialized.

See `docs/sys-a444-blocked.md` for adversarial analysis and consolidation clues.
