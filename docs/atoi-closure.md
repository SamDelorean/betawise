# A33C / `atoi` — ABI closure

Status: **mechanically closed (A) / published contract**.

## Contract

```c
int atoi(const char *str);
```

On the OS3K m68k ABI used by the canonical firmware, `int` is 32 bits. A33C consumes exactly one pointer argument and returns the parsed scalar in D0.L.

## Primary reconstruction

The complete A33C handler is 0x8E bytes in AS3000 2005, NEO 2005, and NEO 2013. The three implementations are mechanically identical after relocation of two absolute references to the internal character-classification table; their common relocation-normalized SHA-256 is `22025f4157fc177727291a3cb16de4d0daa0a7d8aadeedfa442e5166de888b75`.

The handler skips leading characters whose classification byte has the whitespace bit set, accepts one optional `-` or `+`, then accumulates decimal digits as `acc = acc * 10 + (c - '0')`. Parsing stops at the first non-digit. The sign is applied before the final D0.L result is returned.

The implementation does not establish a separate status result. This closure does not claim NULL checking, `errno`, saturation, or any stronger overflow semantics than the observed 32-bit longword arithmetic.

## Caller survey

The corrected PC-index detector exhausted the canonical SmartApplet corpus: 30 table-bearing applets plus 11 structural negatives, for 41/41 total.

A33C has **67 executable callers**, all in four NEO applets: Accelerated Reader (16), AlphaQuiz (2), MathFacts in a Flash (41), and MathFacts Offline (8). The other 26 table-bearing applets and all 11 structural-negative applets contain no A33C caller.

Simple call sites provide direct ABI evidence: they push one string pointer, invoke A33C, clean exactly four bytes, and consume D0.L as a scalar result. No caller supplies a second argument.

## A33C versus A340

A340, historically mapped as `atol`, uses the same 0x8E-byte parser body in all three canonical ROMs. Because OS3K `int` and `long` are both 32-bit here, firmware mechanics alone do not distinguish A33C from A340. The nominal A33C=`atoi` / A340=`atol` distinction is retained from the historical consecutive syscall mapping, while the one-pointer / D0.L parser contract is independently confirmed from primary firmware and callers.

## Adversarial conclusions

- A generic multi-base parser is rejected: the handler accepts decimal digits only and has no radix argument.
- A 64-bit conversion is rejected: accumulation and the observable return are longword-sized.
- A two-argument or status-return contract is rejected by both handler stack access and caller cleanup.
- Saturating conversion, `errno`, NULL safety, and a particular out-of-range policy are not asserted because the firmware does not demonstrate them.

Private workpapers retain exact ROM hashes, runtime/file offsets, per-generation disassemblies, raw handler hashes, ctype-table correlation, all 67 caller offsets, and the reproducible regression.

Static regression: **OVERALL PASS**. Dynamic regression: **not executed**.
