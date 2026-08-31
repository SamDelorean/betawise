# SYS_A444 — bounded entity decoding behavior, source contract blocked

Status: **MECANICA_RECONSTRUIDA A / BLOQUEADO_SEMANTICA_DESCRIPTOR**.

`A444` is callable only in the NEO 2013 corpus examined here. The AS3000 2005 and NEO 2005 vector entries are non-pointer/unresolved values and are not treated as implementations of this service.

The handler has four 32-bit stack slots plus a contractual six-byte aggregate-result channel through `A1`.

Mechanically established argument roles are:

- slot 1: readable pointer to an object containing at least four 32-bit fields. Fields at offsets 0 and 4 are compared for an early-empty condition; fields at offsets 8 and 12 provide the source cursor and source-end pointer on the normal path;
- slot 2: writable byte-buffer destination;
- slot 3: physically a 32-bit stack slot but consumed as a signed 16-bit bound (`MOVE.W` followed by sign extension); the sole direct caller independently sign-extends the value before pushing it;
- slot 4: writable pointer receiving a 16-bit output-count-like delta. It is cleared on the early path and written on the normal path.

The transformation behavior is much less ambiguous than the source-level type names. A444 scans the bounded source interval, copies ordinary bytes, and treats byte `0x26` (`&`) specially. Its helper recognizes exactly the predefined entity names `amp`, `lt`, `gt`, `apos`, and `quot`, plus numeric references beginning with `#`. Numeric references support decimal and `x`-prefixed hexadecimal forms, require a terminating semicolon, and are emitted only when the decoded value fits below `0x100`. The destination is NUL-terminated. This is sufficient to describe A444 as implementing single-byte XML/entity-reference decoding behavior without inventing a vendor function name or claiming arbitrary HTML named-entity support.

The incoming `A1` is preserved and receives an exact six-byte copy at return. The sole recovered direct firmware caller prepares `A1` to point at a local six-byte object before calling A444, ignores `D0`, and later forwards that same six-byte object to its own saved `A1`. Thus the aggregate-result role of `A1` is confirmed at the binary ABI level.

The aggregate has a state byte at offset 0, an uninitialized/padding-compatible byte at offset 1, and a 32-bit conditional detail field at offset 2. The field at offset 2 must not be modeled as always defined: the early state-12 path explicitly sets it to zero, a state-13 path writes a source-cursor delta, while the normal success path sets state 0 without initializing that local 32-bit field.

`D0` is rejected as a coherent stable scalar return. Its value differs by control-flow path and the sole direct caller does not consume it; contractual outputs are instead the `A1` aggregate, destination-buffer side effects, and the 16-bit slot-4 output.

## Resolution clues for consolidation

The remaining debt is concentrated in the source-level layout/name of the four-long descriptor in slot 1 and the vendor semantics of observed result states such as 0, 12, and 13. Reconstructing A448 is especially relevant because the recovered caller chain uses A448 immediately before A444 and passes a local descriptor into A444. Other useful evidence would be a historical prototype, an external applet call site that types the descriptor and outputs, or dynamic probes covering empty input, valid named/numeric entities, invalid entities, and destination-capacity exhaustion.

Do not expose the second aggregate byte as a semantic field and do not expose the 32-bit detail as unconditionally initialized. No callable public header is emitted until the descriptor/result conditions can be represented without inventing source semantics.

Private ROM bytes, full correlated disassembly, helper/table analysis, hashes, and regression transcripts are retained outside the public repository.
