# SYS_A1F8 local-index reverse lookup — source-first closure

Status: mechanically and semantically closed from canonical firmware evidence. No reliable modern vendor symbol was recovered, so the neutral `SYS_A1F8` name is retained.

```c
int32_t SYS_A1F8(uint16_t local_index);
```

## Source-first correlation

BetaWise maps vector index 126 to `SYS_A1F8` without a vendor name. The prior A1E0–A1FC reconstruction supplied a reverse-lookup hypothesis only. A1F4 has now independently closed descriptor `+0x46` as the local-index field and shown that descriptor `+0x44` is a distinct canonical token.

A254 provides a second independent anchor: its current-group selector derives the low seven bits of `current_group_flags` as `flags & 0x7F`. A1F8 reads that same runtime byte but gives bit 7 a separate filtering role. The original vendor meaning of bit 7 remains unknown and is not named speculatively.

## Canonical firmware confirmation

A1F8 is vector index 126. Entry points:

- AS3000 2005: `0x004E2902`;
- NEO 2005: `0x005E525E`;
- NEO 2013: `0x0043B660`.

The handler is exactly `0x90` bytes in all three images and contains no JSR or BSR calls. It walks a descriptor interval described by the active File API runtime context: a context record supplies the first descriptor pointer and descriptor count, and the end pointer is calculated as `first + count * 0x48`.

## Confirmed lookup rule

For each descriptor candidate, the routine ultimately compares `local_index` with descriptor `+0x46` and, on match, returns the canonical token from descriptor `+0x44`, zero-extended.

There is an important conditional high-byte filter controlled by `current_group_flags.bit7`:

- bit 7 **clear**: no token-high-byte restriction is applied before the local-index comparison;
- bit 7 **set**: a candidate is considered only when

```text
(descriptor_token & 0xFF00) == (current_group_flags << 8)
```

If no candidate matches, A1F8 returns raw `-11`.

The routine is read-only: it does not invoke the File API resolver, does not call helpers, and does not mutate descriptors or global state.

## Correction to the older closure

The previous shorthand that A1F8 "searches the current file group/class" was too strong. The firmware implements an asymmetric condition: when bit 7 is clear there is no high-byte filter at all; only when bit 7 is set is the full `current_group_flags` byte required to match the token high byte. The exact condition is preserved here while the vendor semantics of bit 7 remain unknown.

## Callers

Exactly two direct absolute A1F8 calls exist in each canonical ROM:

- AS3000: `0x004C1D3A`, `0x004C2260`;
- NEO 2005: `0x005C1BE0`, `0x005C2106`;
- NEO 2013: `0x00411D4A`, `0x00412284`.

All six call sites zero-extend a byte-valued local index into the single 32-bit ABI slot and remove one slot after the call.

## Refutation

- Refuted dependence on the common File API resolver: A1F8 performs a direct bounded descriptor scan and contains no calls.
- Refuted an unconditional current-group high-byte restriction: the high-byte filter is enabled only when `current_group_flags.bit7` is set.
- Refuted returning the matched local index: success returns descriptor token `+0x44`.
- Refuted token/local-index identity: the routine compares `+0x46` and returns `+0x44`.
- No speculative vendor name is introduced for either the syscall or bit-7 mode.

## Classification

**CONFIRMED:** one `uint16_t` argument; bounded 0x48-byte descriptor scan; conditional bit-7 high-byte filter; local-index comparison at `+0x46`; canonical-token return from `+0x44`; raw `-11` on no match; two direct callers per ROM; no mutation; cross-generation equivalence.

**INFERENCIA FUERTE:** conceptual role as local-index-to-canonical-token reverse lookup.

**DESCONOCIDO:** original modern vendor symbol and vendor semantics/name of `current_group_flags.bit7`.

## Regression

Static primary-byte regression was **EXECUTED: 87/87 PASS** against the three canonical ROMs. It verifies hashes, handler span, context-record traversal, `count * 0x48`, bit-7 branching, exact `0xFF00` mask and shifted expected high byte, `+0x46` compare, `+0x44` return, `-11`, absence of calls, both direct callers, A254 shared-flag correlation, and generation-normalized stability.

Dynamic regression is **SPECIFIED / NOT EXECUTED**: known/absent indexes, bit7 clear/set, matching/nonmatching token high byte, duplicate local indexes in different token-high domains, and read-only verification.

Firmware bytes and complete disassembly remain private.
