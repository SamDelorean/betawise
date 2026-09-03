# SYS_A1FC canonical-token build/validate — source-first closure

Status: mechanically and semantically closed from canonical firmware evidence. The original modern vendor symbol was not recovered, so `SYS_A1FC` is retained.

```c
int32_t SYS_A1FC(uint8_t token_low, uint8_t token_high);
```

## Source-first correlation

BetaWise maps vector index 127 to `SYS_A1FC`. The earlier A1E0–A1FC reconstruction supplied the two-byte token hypothesis only. A254 independently establishes a token-group byte and official/internal callers use its return immediately as the high byte supplied to A1FC. A1F4/A1F8 separately establish the modern 16-bit canonical token stored in descriptor `+0x44`.

## Canonical firmware confirmation

Entry points:

- AS3000 2005: `0x004E2992`;
- NEO 2005: `0x005E52EE`;
- NEO 2013: `0x0043B6F0`.

The closed handler control-flow span is `0x40` bytes in all three canonical ROMs. NEO 2005 and NEO 2013 are byte-for-byte identical; AS3000 differs only in the low byte of the BSR displacement to the common File API resolver.

## Confirmed behavior

The handler reads the low byte of ABI slot 2, shifts it left eight bits, reads the low byte of ABI slot 1, and combines them as:

```text
token = ((uint16_t)token_high << 8) | token_low
```

It then validates that token through the common File API resolver.

- resolver success: return the assembled token, zero-extended to `int32_t`;
- resolver failure: return the resolver status unchanged.

A1FC itself does not mask or remap either byte and does not mutate descriptor fields.

## Callers

Exactly ten direct A1FC call sites exist in each canonical ROM. All thirty are immediately preceded by A254; its returned effective token-group byte is zero-extended and supplied as A1FC's `token_high`, while a separate byte supplies `token_low`. This independently confirms both argument order and the build-plus-validate role.

## Refutation

- Refuted `(high, low)` argument order: stack accesses prove slot 1 is low and slot 2 is high.
- Refuted a pure concatenation helper: the assembled token is passed through the common resolver and failures propagate its status.
- Refuted a boolean/status-only return: successful validation returns the full token.
- Refuted applying `& 0x7F` inside A1FC: that policy belongs to A254; A1FC uses the supplied high byte literally.
- No speculative vendor symbol is introduced.

## Classification

**CONFIRMED:** two-byte low/high ABI; `high<<8 | low`; common File API resolver; success returns token; failure returns resolver status; ten direct callers per ROM; A254→A1FC correlation; cross-generation equivalence.

**INFERENCIA FUERTE:** conceptual role as canonical file-token builder/validator.

**DESCONOCIDO:** original modern vendor symbol.

## Regression

Static primary-byte regression was **EXECUTED: 59/59 PASS** against the three canonical ROMs. It verifies hashes, handler span, both byte-slot accesses, high-byte shift, token assembly, resolver target, status propagation, token success return, caller counts/addresses, universal A254 predecessor correlation, and cross-generation byte stability.

Dynamic regression is **SPECIFIED / NOT EXECUTED**: valid low/high pairs, high byte returned by A254, invalid combinations, known special tokens, and exact token/status return.

Firmware bytes and complete disassembly remain private.
