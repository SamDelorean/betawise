# SYS_A07C — 32-bit radix conversion

Status: mechanically closed from primary firmware evidence and revalidated source-first.

No vendor function name or public historical prototype has been recovered. BetaWise preserves only `SYS_A07C`, so that neutral name is retained.

## Reconstructed ABI

A conservative C representation of the mechanically observed contract is:

```c
uint8_t SYS_A07C(uint32_t value, uint32_t radix, char *out);
```

The three arguments are consumed as 32-bit stack slots in this order: `value`, `radix`, `out`.

`D0` is explicitly cleared and then loaded from an 8-bit digit counter immediately before return. It is therefore not merely a residual register value. The returned count excludes an optional leading minus sign.

## Confirmed behavior

The routine converts a 32-bit value to an uppercase radix representation and NUL-terminates the output.

- `value == 0` writes `"0"`, then NUL, and returns `1`.
- For nonzero values it repeatedly computes unsigned remainder and quotient by `radix`, stores digits in a temporary reverse-order buffer, then copies them forward to `out`.
- Remainders `0..9` become `0..9`.
- Remainders `10+` become `A + (remainder - 10)` without an upper-range check.
- If bit 31 of `value` is set **and** `radix == 10`, the routine writes `'-'` first and negates the 32-bit value before the unsigned conversion loop.
- For radices other than 10, bit-31 values are treated as unsigned 32-bit magnitudes.
- The return count is the number of converted digits only; the optional `'-'` is not counted.

This produces conventional uppercase hexadecimal for radix 16 and correct decimal text for values such as `INT32_MIN` under 32-bit two's-complement arithmetic.

## Division helpers

A07C invokes two internal helpers with `D0 = value` and `D1 = radix`.

Both helpers are byte-identical across the three canonical ROM generations and implement the same unsigned 32-bit division core. The first returns the remainder in `D0`; the second returns the quotient in `D0`. Each uses a `DIVU.W` fast path when possible and a 32-iteration restoring-division fallback otherwise.

These effects are confirmed mechanically; no vendor/compiler symbol is assigned to the helpers.

## Important preconditions and unsafe edge cases

The firmware does not validate the radix or output pointer.

- `out == NULL` is unsafe because output writes are unconditional.
- `radix == 0` with nonzero input reaches unsigned division by zero.
- `radix == 1` with nonzero input does not reduce the quotient, so the conversion loop does not terminate.
- `radix > 36` is accepted mechanically, but digits beyond remainder 35 become characters beyond `Z`; for example remainder 36 maps to `'['`.
- The handler reserves only **10 bytes** of local reverse-digit storage and performs no bounds check. Any value/radix combination requiring more than 10 digits writes beyond that local buffer before reversal. For example, `0xFFFFFFFF` requires 32 binary digits and 21 base-3 digits, while decimal and hexadecimal maxima fit in 10 and 8 digits respectively.

Therefore a safe caller must provide a writable output buffer, use `radix >= 2`, and ensure the magnitude requires no more than 10 converted digits. Conventional alphanumeric output additionally requires `radix <= 36`.

## Cross-generation verification

| Platform | Handler address | Handler size |
| --- | ---: | ---: |
| AlphaSmart 3000, 2005 | `0x004CBEA8` | `0xA0` |
| NEO, 2005 | `0x005CBA00` | `0xA0` |
| NEO, 2013 | `0x0041C420` | `0xA0` |

The only handler differences are the relocated absolute addresses of the two division helpers. Neutralizing those targets makes all three handler bodies identical; normalized SHA-256:

`89caab67c534f1555305bd6d40ecf84e72a5d7389a683f4b5f47438f10388b07`

Firmware bytes and full disassemblies remain private.

## Source-first reconciliation

`os3k/syscall.c` contains `DEFINE_SYSCALL(31, SYS_A07C)`, but `os3k.h` does not expose a corresponding public prototype. Searches of the historical source corpus and the project/public references did not recover a reliable vendor name or a `ltoa`/`ultoa`/`itoa` declaration attributable to this slot. The implementation is therefore documented by behavior under the neutral syscall name rather than renamed after a superficially similar C library function.

## Official caller evidence and correction

Structural resolution of AlphaWordPlus import calls found one executable A07C call in each of two official generations:

- AlphaWordPlus 2005: call sequence at file offset `0x11CCC`, resolving to the A07C veneer at `0x122A8`.
- AlphaWordPlus NEO 2012: call sequence at file offset `0x127CC`, resolving to the A07C veneer at `0x12DB0`.

Both callers push a writable output pointer, radix `10`, then a 32-bit computed value. This independently supports the three-slot argument order and ordinary decimal-conversion use. Neither caller consumes A07C's `D0` result before continuing, so caller usage alone does not establish the return; the explicit terminal `D0` construction in the handler does.

An earlier provisional xref interpretation near NEO 2012 file offset `0x8D0A` was refuted. Although that sequence loads the immediate value `0xA07C` into `A0`, the indexed PC-relative `JSR` resolves to file offset `0x12D8C`, whose veneer is `A010` (`PutChar`), not A07C. Raw/immediate appearances are therefore not counted as syscall callers.

## Regression status

A dedicated static regression was **executed and passed 77/77 checks**. It verifies the three canonical ROM hashes, A07C vectors and handler hashes, normalized handler identity, argument loads, both division-helper effects, explicit return-count construction, NUL termination, the two official executable caller resolutions, the refutation of the false `0x8D0A` xref, and adversarial models for radix 0, radix 1, radix >36, `INT32_MIN`, and the 10-byte local-buffer limit.

Regression source SHA-256:

`295d69bd98b1f1675bcb0797defc5e3823d58581a1f59901fdc9dc909de298ca`

Regression output SHA-256:

`3d378de05444e3f86b03b72a1c24444e03d2ddd21aaa7efd7a17f9f3e3e8adb3`

A dynamic hardware/emulator probe has not been executed; it remains additional validation rather than a prerequisite for this mechanical closure.

## Classification

- **CONFIRMED:** three 32-bit argument slots; output order and NUL termination; decimal-only sign handling; uppercase digit mapping; unsigned quotient/remainder helpers; explicit low-byte digit-count return; two official base-10 callers; 10-byte temporary storage; unsafe radix-0/radix-1 behavior; normalized cross-generation handler identity.
- **STRONG INFERENCE:** the C spelling `uint8_t SYS_A07C(uint32_t value, uint32_t radix, char *out)` as the narrowest portable expression of the observed ABI.
- **PROVISIONAL:** none beyond ordinary C type spelling where firmware encodes only widths/effects.
- **UNKNOWN:** original vendor function name and any stronger historical source-level prototype.
