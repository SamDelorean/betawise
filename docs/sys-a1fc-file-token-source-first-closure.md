# SYS_A1FC — source-first closure

Status: **CONFIRMED / mechanical confidence A**.

```c
int32_t SYS_A1FC(uint8_t token_low, uint8_t token_high);
```

`SYS_A1FC` constructs a 16-bit file token from two byte-valued ABI slots, validates the resulting token through the common File API resolver, and returns either the validated token or the resolver status.

## Contract

The handler computes:

```c
uint16_t token = ((uint16_t)token_high << 8) | token_low;
```

It then calls the shared File API resolver with that token. If resolution succeeds, the token is returned zero-extended in `D0.L`. If resolution fails, the resolver status is propagated unchanged.

The routine does not mask, remap, normalize, or otherwise reinterpret either input byte and does not mutate the resolved descriptor.

## Evidence

The canonical handlers are stable across the three audited ROMs:

| ROM | handler entry | length |
| --- | ---: | ---: |
| AlphaSmart 3000 (2005) | `0x004E2992` | `0x40` |
| NEO (2005) | `0x005E52EE` | `0x40` |
| NEO/System 3.15 (2013) | `0x0043B6F0` | `0x40` |

The NEO 2005 and NEO 2013 handlers are byte-identical; the AlphaSmart 3000 form differs only in relocation-sensitive resolver call bytes.

Direct-call correlation gives **10/10/10 callers**. At every observed call site, `SYS_A254` is called immediately beforehand; its returned effective token-group byte is passed as `token_high`, while the companion byte is passed as `token_low`. This independently confirms both argument order and the build-and-validate role.

## Refutation checks

The firmware rejects several plausible alternatives:

- argument order is **low, high**, not high, low;
- this is not mere byte concatenation, because the constructed token is passed through the common resolver;
- success does not return a boolean or status-only value; it returns the complete token;
- `SYS_A1FC` does not apply `& 0x7F` to the high byte. Any such group policy belongs upstream (notably `SYS_A254`).

## Classification

- **CONFIRMED:** two-byte ABI, `high << 8 | low`, resolver validation, token-on-success, resolver-status-on-failure, no descriptor mutation, cross-ROM equivalence.
- **STRONG INFERENCE:** public semantic description as “build and validate canonical file token.”
- **UNKNOWN:** original vendor symbol/name; therefore the neutral `SYS_A1FC` identifier is retained.

## Regression

Static regression: **59/59 PASS**.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**. It should cover valid low/high pairs, high-byte values obtained from `SYS_A254`, invalid combinations, known special tokens, and verification of token-vs-status return behavior.

No ROM bytes, firmware image, or extended disassembly is published here.
