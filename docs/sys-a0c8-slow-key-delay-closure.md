# SYS_A0C8 / index 50 — Slow Keys delay parameter

Status: **mechanically closed / source-first revalidated**.

## Recovered contract

```c
uint16_t SYS_A0C8(void);
```

**CONFIRMED:** the raw A-line service consumes no arguments, performs no writes in its direct handler, and returns a raw 16-bit word in `D0.w`. The upper 16 bits of `D0` are not contractual.

**INFERENCIA FUERTE:** the returned word is the active **Slow Key Delay** parameter associated with the Slow Keys enable state exposed by `SYS_A0C4`.

**DESCONOCIDO:** the original vendor symbol, original C typedef/spelling, and the user-facing unit represented by one returned count. Therefore the neutral name `SYS_A0C8` is retained.

## Source-first anchors

BetaWise preserves only `DEFINE_SYSCALL(50, SYS_A0C8)` and the current public `os3k.h` does not provide a named prototype for this slot. Official AlphaSmart Manager documentation describes a setting named **Slow Keys and Slow Key Delay**, defining the delay as the time a key must be held before the character appears. AlphaWord Plus also exposes a distinct Slow Keys status, separate from Sticky Keys and Auto Repeat.

These references anchor the expected subsystem, but the ABI below is derived from firmware rather than from the labels.

## Firmware confirmation

The three canonical ROMs were rehashed and matched their established SHA-256 values. Index 50 resolves to:

| Generation | Handler | File offset | Returned storage |
| --- | ---: | ---: | ---: |
| AS3000 2005 | `0x004CDFC4` | `0x00DFC4` | `0x4348` |
| NEO 2005 | `0x005D1FA8` | `0x011FA8` | `0x57D2` |
| NEO 2013 | `0x00422F98` | `0x012F98` | `0x5D2A` |

All three handlers have the same eight-byte instruction form after relocation normalization:

```text
MOVE.W  <relocated_global>.L,D0
RTS
```

The normalized handler bytes are `30 39 00 00 00 00 4E 75`, SHA-256 `eedb3cd56bcc98e82ffae3b68085ecb32194edbc2c17ae2bff816daa91725e47`.

The returned word is the low word of a zero-initialized 32-bit storage object (`0x4346`, `0x57D0`, `0x5D28`). A private helper accepts a 16-bit value, zero-extends it, and writes the full 32-bit storage. That nearby helper is **not** syscall A0CC: the actual index-51 vectors resolve elsewhere. This distinction is important because physical code adjacency does not imply A-line identity.

## Callers and width

Each ROM contains exactly two direct absolute calls to A0C8:

1. the A-line dispatcher path, which clears `D1` and then copies `D0.w` into `D1`; and
2. an internal keyboard path that first reads `SYS_A0C4`, compares its low byte exactly with `1`, and only when enabled calls A0C8 and copies `D0.w` to a working word register.

These independent consumers confirm a **16-bit** return contract and link the value to the state controlled by A0C4.

## Delay semantics

The keyboard-processing path subsequently transforms the associated 32-bit storage using the equivalent arithmetic:

```text
scaled = delay * 5;
scaled <<= 4;
deadline = current_timer + scaled;
```

Thus firmware directly establishes an internal conversion of **`delay * 80` timer-domain units** before forming a deadline. Combined with the A0C4 gate and the official Slow Key Delay terminology, this supports the Slow Keys delay interpretation strongly.

The factor of 80 does **not** by itself establish whether one public count means seconds, milliseconds, centiseconds, or another unit. No public unit is assigned until an independent source or timing probe proves it.

## Refutation checks

The audit explicitly rejected three tempting alternatives:

- A0C8 is not Sticky Keys or Auto Repeat state: those use distinct globals and mechanics.
- A0C8 is not a 32-bit getter: handler, dispatcher, and semantic caller all consume a word.
- The adjacent private writer is not A0CC: the real index-51 vector table points to different handlers.

## Regression

Static regression was **executed: 42/42 PASS** across the three canonical ROMs. It checks canonical hashes, exact handler shape, storage relationship and initialization, private 16-bit-to-32-bit writer, the exact two direct callers, word-width consumption in dispatcher and semantic caller, the unique `delay * 80` deadline sequence, and cross-generation normalized handler identity.

A preliminary harness run contained an incorrectly transcribed expected normalized SHA. Recomputing the digest from the normalized bytes above and correcting only that test constant produced the final 42/42 PASS result; there was no firmware discrepancy.

Dynamic timing validation is **specified but not executed**. It is optional for the mechanical closure and would be useful specifically to recover the external time unit.
