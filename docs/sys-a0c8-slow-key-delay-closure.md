# SYS_A0C8 / index 50 — Slow Keys delay parameter

Status: **mechanically closed / source-first revalidated**.

## Recovered contract

```c
uint16_t SYS_A0C8(void);
```

**CONFIRMED:** the raw A-line service consumes no arguments, performs no writes in its direct handler, and returns a raw 16-bit word in `D0.w`. The upper 16 bits of `D0` are not contractual.

**INFERENCIA FUERTE:** the returned word is the active **Slow Key Delay** parameter associated with the Slow Keys enable state exposed by `SYS_A0C4`.

**CONFIRMADO (unidad de interfaz):** the interactive firmware path treats the returned word as a 0..30 value, divides it by 10 for decimal rendering, and prevents increment above 30. Official AlphaSmart documentation independently describes the Slow Keys control in tenths of a second and a maximum of 3 seconds. Therefore one externally displayed count corresponds to **0.1 second**. The neutral syscall name remains because the original vendor symbol/typedef has not been recovered.

## Source-first anchors

BetaWise preserves only `DEFINE_SYSCALL(50, SYS_A0C8)` and the current public `os3k.h` does not provide a named prototype for this slot. Official AlphaSmart Manager documentation describes a setting named **Slow Keys and Slow Key Delay**, defining the delay as the time a key must be held before the character appears. The NEO user guide explicitly states that the delay is increased/decreased in tenths of a second; the AlphaSmart 3000 documentation describes the same control as 0 (off) through 3 seconds. AlphaWord Plus also exposes a distinct Slow Keys status, separate from Sticky Keys and Auto Repeat.

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
2. an internal keyboard/control path that first reads `SYS_A0C4`, compares its low byte exactly with `1`, and only when enabled calls A0C8 and copies `D0.w` to working register `D7`.

These independent consumers confirm a **16-bit** return contract and link the value to the state controlled by A0C4.

The same interactive path provides a stronger unit anchor than the earlier closure recorded: it uses a divisor of `10` while formatting the `D7` value and compares `D7` against `30` before allowing an increment. Those constants are invariant across AS3000 2005, NEO 2005, and NEO 2013. Together with the official UI specification (tenths of a second, maximum 3 seconds), this confirms the public scale as 0.1 s/count.

## Delay semantics

The keyboard-processing path subsequently transforms the associated 32-bit storage using the equivalent arithmetic:

```text
scaled = delay * 5;
scaled <<= 4;
deadline = current_timer + scaled;
```

Thus firmware directly establishes an internal conversion of **`delay * 80` timer-domain units** before forming a deadline. The independent UI evidence now resolves the previously unknown external scale: the raw delay count exposed by A0C8 is the same 0..30 setting rendered as 0.0..3.0 seconds.

The factor of 80 should still be described only as the conversion into the firmware timer domain; it is not itself the proof of the public unit.

## Refutation checks

The audit explicitly rejected three tempting alternatives:

- A0C8 is not Sticky Keys or Auto Repeat state: those use distinct globals and mechanics.
- A0C8 is not a 32-bit getter: handler, dispatcher, and semantic caller all consume a word.
- The adjacent private writer is not A0CC: the real index-51 vector table points to different handlers.

A fourth ambiguity is now resolved: the public count is not an unspecified timer unit. The independent `/10` rendering and `30` ceiling in firmware match the official 0.1-second/3-second user setting.

## Regression

Static regression was **executed: 42/42 PASS** across the three canonical ROMs. It checks canonical hashes, exact handler shape, storage relationship and initialization, private 16-bit-to-32-bit writer, the exact two direct callers, word-width consumption in dispatcher and semantic caller, the unique `delay * 80` deadline sequence, and cross-generation normalized handler identity.

A preliminary harness run contained an incorrectly transcribed expected normalized SHA. Recomputing the digest from the normalized bytes above and correcting only that test constant produced the final 42/42 PASS result; there was no firmware discrepancy.

A supplemental unit-resolution regression was subsequently **executed: 36/36 PASS**. In addition to canonical hashes, vector/handler/storage/writer and direct-call invariants, it asserts the invariant divisor-10 UI path and the `CMPI.W #30,D7` ceiling across all three ROMs. Supplemental harness SHA-256: `c366437bae8c2d5ec9e220449d5f72cc7764e892abfa405a3827c7eb976d9a3b`; stdout SHA-256: `4ad63d8f2c91f84f27e0ea1464674561d5dcab6b0f742739ec16844066ea46ba`.

Dynamic timing validation remains **specified but not executed**. It is no longer required to recover the user-facing scale, but can still validate real-time behavior independently.
