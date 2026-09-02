# A0B4 / index 45 — SYS_A0B4 closure

## Status

**MECÁNICA_CERRADA A / REVALIDADO_SOURCE_FIRST / 121/121 PASS / COMPLETADO**

No historical/vendor function name has been recovered for this ABI slot. The neutral name `SYS_A0B4` is therefore retained.

Working contract:

```c
bool SYS_A0B4(uint8_t packed_matrix_coordinate);
```

The argument is consumed only through its low byte. The public typedef/original symbolic encoding remains unknown.

## Source-first result

BetaWise preserves only `DEFINE_SYSCALL(45, SYS_A0B4)` and the public `os3k.h` material does not expose a named prototype for index 45. Directed searches of the available historical material did not recover an independent vendor name or typedef.

The neighboring keyboard reconstruction supplies the semantic anchor: A0A4 / `ScanKeyboard` scans a 16-position matrix, reads the active-low keyboard byte at `0xFFFFF419`, and translates physical matrix coordinates to logical keys. Direct firmware analysis shows that A0B4 calls exactly the same matrix-position selector used by `ScanKeyboard`.

## Primary firmware evidence

| Generation | Handler | File offset | Length |
|---|---:|---:|---:|
| AS3000 2005 | `0x004CE9C8` | `0x00E9C8` | `0x46` |
| NEO 2005 | `0x005D2AC6` | `0x012AC6` | `0x46` |
| NEO 2013 | `0x00423AB6` | `0x013AB6` | `0x46` |

Raw handler SHA-256:

- AS3000: `27f0ecd2fc29250e693c45bd1d836caf2e0e61ea509c23f1426269e0acd90b8f`
- NEO 2005: `038c7d8552ff8c7e80eaf90a9442dff570083bfb19a3b9ee4610a9afa5e5f9b8`
- NEO 2013: `038c7d8552ff8c7e80eaf90a9442dff570083bfb19a3b9ee4610a9afa5e5f9b8`

The only cross-generation byte difference is the relative displacement of the BSR to the relocated matrix-position helper. Neutralizing that displacement makes all three handlers byte-identical; normalized SHA-256 is `d244a809d313801c03fb3c0bf164fd83a04f6017f34b31554d1f9826eb666583`.

## Mechanical contract

For the low byte of the ABI argument:

```text
position = arg & 0x0F
bit      = (arg >> 4) & 0x0F
```

The handler:

1. passes `position` to the same physical matrix-position selector used by `ScanKeyboard`;
2. reads the byte at `0xFFFFF419`;
3. inverts that byte because keyboard sensing is active-low;
4. zero-extends the resulting byte;
5. constructs `1 << bit`;
6. tests only that selected bit; and
7. returns normalized `D0.b = 0` or `1`.

This is therefore a predicate over one **physical matrix coordinate**, not a logical `Key_e` lookup and not an aggregate “any key” predicate.

## Meaningful coordinate range

The high nibble is mechanically decoded as a 4-bit bit index, so values 0..15 are accepted by the instruction sequence. However, the hardware sample is an 8-bit value and is zero-extended before the mask operation.

Consequently:

- bit selectors 0..7 are physically meaningful;
- bit selectors 8..15 can never intersect the sampled byte and therefore always produce `false`.

The low nibble remains a 0..15 matrix-position selector.

This distinction is firmware-derived and is retained rather than inventing a narrower vendor typedef that has not been recovered.

## Relationship to neighboring keyboard syscalls

- A0A4 / `ScanKeyboard` scans the matrix, converts physical changes to logical `Key_e`, combines modifier state, and queues `KeyMod_e` events.
- A0B0 / `IsKeyDownNow` answers whether any physical key is currently down.
- A0B4 / `SYS_A0B4` tests one packed physical coordinate directly.

A0B4 does not consult the logical matrix-to-key translation table and therefore its argument is not a `Key_e`.

## Caller review

The four re-extracted official applets used for the neighboring keyboard audits contain the raw A0B4 import word but no validated structural executable xrefs to the A0B4 veneer:

- AlphaWordPlus 2005: 0
- AlphaWordPlus NEO 2012: 0
- KeyWords 2005: 0
- KeyWords NEO 2012: 0

Raw byte occurrences are not promoted to callers.

## Refutation results

- **Refuted:** A0B4 receives a logical `Key_e`. It operates on a packed physical coordinate and never invokes logical key translation.
- **Refuted:** A0B4 means “is any key down.” It selects one position and one bit, unlike A0B0.
- **Refuted:** all 16 high-nibble selectors describe hardware input bits. The sampled hardware value is only 8 bits; selectors 8..15 always test zero after zero-extension.
- **Refuted:** AS3000 and NEO implement different A0B4 algorithms. After relocation normalization, the 3/3 handlers are identical.

## Regression

Static/contractual regression: **121/121 PASS**.

Coverage includes the three canonical ROM hashes, exact handler bounds and hashes, prologue/epilogue, low/high-nibble extraction, resolution of the shared `ScanKeyboard` position-selection helper, active-low `F419` sampling, bit-mask construction, boolean normalization, cross-ROM normalization, and adversarial model cases including selectors 8..15.

- regression source SHA-256: `77e99feee0ca4d65cf3b8183fa2818f5048f8455e8b9938846a4ba4b69a726db`
- regression output SHA-256: `5e1d76b81b12131af2b84ec855c42ef79190d37f33d6fda85bc0cf0da4ceb63b`

A dynamic 68k probe has **not** been executed; it is additional validation rather than a closure blocker.

## Disposition

Document A0B4 conservatively as `SYS_A0B4` with the firmware-supported working signature `bool SYS_A0B4(uint8_t packed_matrix_coordinate)`. Keep the vendor name and original typedef/encoding symbol as unknown until a nominal historical source is recovered. The next ascending ABI frontier is A0B8 / index 46.
