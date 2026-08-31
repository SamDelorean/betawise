# SYS_A44C — blocked semantic closure

`SYS_A44C` (index 275) is mechanically reconstructed for the canonical NEO 2013 ROM but is intentionally **not exposed as a callable public SDK contract**.

## What is proven

A44C is a two-stage wrapper/combinator around `SYS_A448` and `SYS_A440`:

- it accepts six 32-bit external stack slots plus a 6-byte aggregate-output channel in `A1`;
- stage 1 invokes A448 with external slots 1 and 2, a local two-entry descriptor array, fixed capacity `2`, and a local count byte;
- stage 2 executes only for aggregate state `0` or `8` with signed count greater than zero;
- stage 2 passes the first descriptor, external slots 3 and 4, fixed literal `18`, and external slot 6 to A440;
- external slot 5 is not consumed;
- the resulting 6-byte aggregate is copied back through the original `A1` channel.

Canonical NEO 2013 mapping: runtime `0x004432B0`, file offset `0x0332B0`, length `0x6C`, handler SHA-256 `ac24442e5758ff6fdc9bd12ab8c9dde51565fe1ad13eea4ef3c5af2e11d34ec0`.

The same vector position is not callable in the canonical 2005 ROMs: AS3000 2005 and NEO 2005 both contain `0x02040307` at slot 275, followed by `0xF0F1F2F3` at slot 276.

## Why it stays blocked

The mechanical ABI is sufficient to reproduce the wrapper behavior, but the following source-level semantics are not demonstrated:

1. the exact conceptual identities of the first and second selector-like strings;
2. the meaning, if any, of the unused fifth external stack slot in the original source declaration/calling convention;
3. whether the 6-byte `A1` channel corresponds to a compiler hidden structure-return convention or another OS3K ABI convention;
4. a stable scalar `D0` return contract;
5. vendor names for the aggregate states, descriptor structure, or service itself.

A nested structured-markup lookup/extraction interpretation is supported by the A448→descriptor→A440 pipeline, but assigning a specific XML/HTML attribute-getter name would exceed the evidence.

## Caller evidence

No direct NEO 2013 ROM caller targets A44C. The official NEO AlphaWordPlus image contains the A44C A-line stub, but exhaustive resolution of all 2,011 indexed-PC JSR calls produces zero A44C calls. Nine calls resolve to neighboring A46C, validating the resolution method. This is a documented negative caller result, not an unperformed search.

## Test status

Static canonical regression: **33/33 PASS**. Dynamic regression: **NOT EXECUTED**.

Use `SYS_A44C` as the neutral identity in documentation. Do not add a typed public prototype until the remaining semantic/return convention questions are independently resolved.
