# SYS_A1D0 / A1D0 — source-first file-state mutator

Status: mechanically and semantically revalidated against the three canonical System 3 firmware generations. No trustworthy modern vendor symbol was recovered, so `SYS_A1D0` remains the public neutral name.

## Contract

```c
int32_t SYS_A1D0(uint16_t file_id, uint32_t state_mask, uint8_t enable);
```

The call resolves `file_id` through the common File API resolver and mutates the descriptor state longword at `+0x20`.

## Important mask correction

The generation-dependent constants are a **low-byte admission gate**, not a strict whitelist for the whole mask:

- AS3000 2005 / NEO 2005: `(uint8_t)state_mask & 0x0D` must be nonzero.
- NEO 2013: `(uint8_t)state_mask & 0x1D` must be nonzero.
- Zero intersection returns `-14` without mutation.

Once admitted, firmware applies the **entire 32-bit `state_mask`** to descriptor `+0x20`. Thus a mask containing at least one admitted low bit plus additional bits is not reduced to `0x0D`/`0x1D` by this handler.

## Mutation

- `enable == 1`: `state |= state_mask`.
- any other byte: `state &= ~state_mask`.

Resolution failure returns 0 without mutation. Successful mutation returns the descriptor's canonical 16-bit token, zero-extended.

## Evidence summary

Public handler length is exactly `0x68` bytes in all three canonical firmware generations. There are **4 / 4 / 4** direct absolute internal JSR callers. Observed firmware call windows include set/clear use of mask `0x04` and set use on the reserved `0x00CB` token path, consistent with the reconstructed argument order.

The only functional generational change identified in this handler is expansion of the admission gate from `0x0D` to `0x1D` in NEO 2013.

## Confidence

- **CONFIRMED:** ABI, common resolver, descriptor `+0x20`, low-byte admission gate, full-longword OR/AND mutation, exact `enable == 1` behavior, token return, and 2005→2013 gate change.
- **UNKNOWN:** modern vendor symbol, semantic names for individual state bits, and vendor name for status `-14`.

Static regression against the three canonical images: **52/52 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
