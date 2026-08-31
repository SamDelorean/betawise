# ABI trace — A448

Public trace for the mechanically reconstructed but source-semantics-blocked OS3K A-line entry `A448`.

- Vector ordinal/index: 274.
- AS3000 2005: non-pointer/unresolved vector value; no callable handler attributed.
- NEO 2005: non-pointer/unresolved vector value; no callable handler attributed.
- NEO 2013: callable handler; exact private bounds and hashes are retained in the correlated workpaper.
- External stack evidence: five 32-bit slots plus a six-byte aggregate result through incoming `A1`.
- Slot roles: input descriptor span; exact NUL-string selector/filter; writable array of 16-byte four-long descriptor records; effective signed-8 record capacity; writable one-byte accepted-record count.
- Parser correlation: common core explicitly recognizes XML/markup syntax characters and shares the entity-reference helper independently established in A444.
- Predicate correlation: A448 installs a callback that compares the supplied selector string against a parser-maintained NUL string with a strcmp-like helper and accepts only exact equality.
- Descriptor correlation: callbacks write records at `base + count*16`, populate fields at offsets 0/4/8/12, and increment count only after the predicate accepts the candidate.
- Direct firmware xrefs in NEO 2013: exactly two recovered `BSR.W` callers, one leading to the A440 wrapper path and one to the A444 wrapper path.
- Caller evidence: both callers prepare a local descriptor array, pass literal record capacity 2, pass a local count byte, prepare `A1` to a local six-byte aggregate, call A448, clean 20 bytes, inspect aggregate state and count, ignore `D0`, and then pass the first produced descriptor to A440 or A444.
- State code 8 is mechanically tied to record-capacity exhaustion/limit: the descriptor callback sets it when produced count is no longer less than capacity; both callers treat state 0 or 8 as usable when count is positive.
- `D0.L` duplicates the aggregate detail delta at return, but no recovered caller consumes it independently; a separate scalar-return contract is not established.
- Static regression: 70/70 PASS against canonical NEO 2013 ROM, callbacks, strcmp-like helper, both callers, common XML parser syntax, and official AlphaWordPlus corpus presence.
- Dynamic/emulator validation: not executed for semantic promotion.

Disposition: **MECANICA_RECONSTRUIDA A / BLOQUEADO_SEMANTICA_SELECTOR**. The binary ABI and parser/filter/descriptor mechanics are established, but no callable C header is emitted until the descriptor-field and selector-string source semantics are independently resolved.

See `docs/sys-a448-blocked.md` for adversarial analysis and consolidation clues.
