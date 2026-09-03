# SYS_A1D4 / A1D4 — source-first file-state query

Status: mechanically and semantically revalidated against the three canonical System 3 firmware generations. No trustworthy modern vendor symbol was recovered, so `SYS_A1D4` remains the neutral name.

## Contract

```c
int32_t SYS_A1D4(uint16_t file_id, uint32_t state_mask);
```

The call resolves `file_id` through the common File API resolver and queries the same descriptor state longword at `+0x20` that A1D0 mutates.

## Admission-gate correction

The generation-dependent constants are a **low-byte admission gate**, not a strict whitelist applied to the query:

- AS3000 2005 / NEO 2005: `(uint8_t)state_mask & 0x0D` must be nonzero.
- NEO 2013: `(uint8_t)state_mask & 0x1D` must be nonzero.
- Zero intersection returns `-14`.

After admission, the firmware evaluates the **entire 32-bit mask**:

```text
(descriptor_state & state_mask) != 0
```

and returns 1 when nonzero, 0 otherwise. Resolution failure also returns 0.

## Handler-boundary correction

Although the next public vector A1D8 is much farther away, A1D4 itself ends at the first `RTS`, only `0x5A` bytes after its entry point. The bytes following that return are private routines and must not be attributed to the public A1D4 handler.

## Evidence summary

The 0x5A-byte public body is structurally equivalent across AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013), after normalizing relocated private-call/diagnostic operands. The only functional generational difference observed is expansion of the low-byte admission gate from `0x0D` to `0x1D` in NEO 2013.

Direct absolute internal JSR callers: **3 / 3 / 3**.

## Confidence

- **CONFIRMED:** ABI, common resolver, descriptor `+0x20`, low-byte admission gate, full-longword state-mask test, returns 0/1 or -14, handler boundary, and 2005→2013 gate change.
- **UNKNOWN:** modern vendor symbol, semantic names for individual state bits, and vendor name for status `-14`.

Static regression against the three canonical images: **49/49 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
