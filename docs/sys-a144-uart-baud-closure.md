# A144 / index81 — SYS_A144 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

The original vendor symbol is not recovered, so the neutral `SYS_A144` name is retained.

## Recovered contract

```c
int32_t SYS_A144(uint32_t baud_rate);
```

`A144` consumes one 32-bit baud-rate argument, selects the corresponding DragonBall `UBAUD` programming word, writes it to the UART baud register, and returns status.

## Supported rates

The audited AS3000 2005, NEO 2005 and NEO 2013 firmware accepts exactly:

- 9600
- 19200
- 38400
- 57600
- 115200

Success returns `0`. Any other rate returns `-502`.

The implementation does not merely use a loose numeric range: it transforms the input and checks for residual bits before matching the five rate values, so nearby unsupported rates are rejected rather than rounded.

## Platform-specific UBAUD values

The public ABI and accepted rates are common, but the hardware programming words differ.

| Baud | AS3000 | NEO 2005 / NEO 2013 |
|---:|---:|---:|
| 9600 | `0x0226` | `0x0326` |
| 19200 | `0x0126` | `0x0226` |
| 38400 | `0x0026` | `0x0126` |
| 57600 | `0x0138` | `0x0238` |
| 115200 | `0x0038` | `0x0138` |

This is preserved as a genuine platform/clock difference rather than normalized away.

## Relationship with A140

`SYS_A140` / index80 delegates its `baud_rate` argument directly to the exact vector entry for `A144` and propagates `A144`'s `-502` error. This proves that `A144` is a separate syscall rather than a private helper embedded in the `A140` implementation.

Additional direct firmware callers invoke `A144` independently. One AS3000 caller explicitly selects among 9600, 57600 and 115200 at runtime; other callers pass a runtime baud value.

## Generation comparison

NEO 2005 and NEO 2013 have the same baud programming table. AS3000 accepts the same five public rates but uses different `UBAUD` words.

## Regression status

Static regression: **EXECUTED — 69/69 PASS**.

It verifies canonical firmware identities, index81/index82 vector boundaries, one-argument ABI, exact-rate discrimination, all five accepted rates, `-502`, `UBAUD` writes, platform-specific programming words, direct caller counts and the PC-relative call from `A140` to the exact `A144` vector entry.

Dynamic baud/loopback verification: **SPECIFIED / NOT EXECUTED**.

## Confidence

- **CONFIRMED:** index81 identity, one-argument ABI, five exact rates, `UBAUD` programming, status `0/-502`, platform tables, independent callers and relationship with `A140`.
- **UNKNOWN:** original vendor function name or symbolic baud enum/constant names, if any.

Raw firmware, ROM offsets, extensive disassembly and private reverse-engineering corpus remain outside the public repository.