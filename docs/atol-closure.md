# A340 / `atol` — ABI closure

Status: **mechanically closed (A) / source-first revalidated / published contract**.

## Contract

```c
long atol(const char *str);
```

On the OS3K m68k ABI used by the canonical firmware, `long` is 32 bits. A340 consumes exactly one pointer argument and returns the parsed scalar in D0.L.

## Primary reconstruction

The complete A340 handler is 0x8E bytes in AS3000 2005, NEO 2005, and NEO 2013. The three implementations are mechanically equivalent after relocation of two absolute references to the internal character-classification table; normalized SHA-256 is `22025f4157fc177727291a3cb16de4d0daa0a7d8aadeedfa442e5166de888b75`.

The handler skips leading characters whose classification marks whitespace, accepts one optional `-` or `+`, then accumulates decimal digits as `acc = acc * 10 + (c - '0')`. Parsing stops at the first non-digit. The sign is applied before the final D0.L result is returned.

A340 is byte-identical to A33C inside each of the three canonical ROMs for the full 0x8E-byte body. Firmware mechanics therefore do not distinguish `atoi` from `atol`; that nominal distinction is retained from the historical consecutive syscall mapping and their SDK prototypes, while the one-pointer / 32-bit result contract is independently confirmed from firmware.

This closure does not claim NULL checking, `errno`, saturation, or a stronger overflow policy than the observed 32-bit arithmetic.

## Source-first correlation

The historical A-line mapping assigns index 208 to `atol` and declares `long atol(const char *str)`. That source is used only to recover the nominal identity. Primary firmware independently proves a one-pointer signed decimal parser returning a 32-bit D0.L value. The strongest adversarial comparison is A33C/`atoi`: A340 and A33C are byte-identical for the complete handler in every canonical ROM, so firmware cannot encode a semantic width distinction between them on this ABI where both `int` and `long` are 32 bits.

The nominal `atoi`/`atol` split is therefore retained only because the consecutive historical symbol mapping is consistent with the independently established common mechanical contract. No artificial binary difference, overflow policy, or wider arithmetic is inferred.

## Caller survey

The corrected A-line PC-index detector exhausted the official corpus: 30 table-bearing applets plus 11 structural negatives, 41/41 total. A340 has **0 executable SmartApplet callers**. The same detector finds 67 A33C callers, providing an adversarial control against a false-negative detector.

ROM-internal direct absolute JSR/JMP references to A340 were also zero in all three canonical ROMs. Caller absence does not define the return type; the handler itself constructs the D0.L result.

## Validation

Canonical ROM hashes, vectors, handler length, raw and normalized handler hashes, A340=A33C byte equality, stack access, decimal-parser behavior, corpus count, and direct ROM xrefs were checked by the private static regression: **OVERALL PASS**. Dynamic execution was not performed.

Private workpapers retain exact ROM hashes, runtime/file offsets, per-generation disassemblies, caller/xref survey, and reproducible regression details.
