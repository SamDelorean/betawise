# A16C / index91 — SYS_A16C — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered raw contract

```c
uint8_t SYS_A16C(uint8_t value);
```

The original vendor name has not been recovered, so the neutral `SYS_A16C` name is retained.

## Confirmed behavior

`A16C` is a pure membership predicate over an 8-bit domain. The low byte of the first ABI argument slot is split into two nibbles:

1. the high nibble selects one of 16 bitmap words;
2. the low nibble selects one bit within that word;
3. the function returns `1` when that bit is set and `0` otherwise.

Only `D0.B` is contractual. No mutation, helper call, or direct firmware-internal caller is part of the public handler.

The canonical AS3000 2005, NEO 2005 and NEO 2013 implementations are semantically identical. Their only handler difference is relocation of the absolute bitmap address.

## Character-domain correlation

The recovered 256-bit table accepts 183 byte values. In particular:

- every ordinary printable ASCII byte `0x20..0x7E` is accepted;
- `0x00` and `0x7F` are rejected;
- selected control bytes such as backspace and carriage return are accepted;
- a substantial set of extended/international byte values is also accepted.

This is strong evidence that the table expresses a text/character acceptability class rather than a physical-key class. The exact original policy represented by the extra control and international members has not been recovered, so the public API is not renamed to an invented vendor term such as `IsPrintable` or `IsValidChar`.

## Relation to keyboard translation

The neighboring keyboard API does not justify treating A16C as simply “all characters returned by `TranslateKeyToChar`”. Direct comparison with the NEO character records shows that those sets differ: some bytes produced by the translation records are rejected by A16C, while A16C accepts additional extended bytes. That simpler hypothesis is therefore refuted.

## Caller survey

No direct absolute JSR caller to the A16C handler exists in the three canonical OS3K ROMs.

A survey of nine official SmartApplets — AlphaWord Plus, Calculator, Control Panel, KeyWords, AlphaQuiz, Responder, Accelerated Reader, Beamer and Wireless File Transfer — finds the expected A16C opcode in each applet's contiguous syscall-stub table but no executable position-relative call targeting that stub in the scanned binaries.

Therefore this closure does not infer higher-level semantics from nonexistent callers.

## Regression status

Static regression: **EXECUTED — 48/48 PASS**.

It verifies canonical ROM hashes, vector index 91, handler length and termination, argument-byte selection, bitmap addressing, normalized three-generation equivalence, exact common bitmap, boolean return grammar, negative firmware caller counts, and the official-SmartApplet caller survey.

Dynamic regression: **SPECIFIED / NOT EXECUTED**. A future probe can sweep all 256 input values and compare `D0.B` against the recovered membership table.

## Confidence

- **CONFIRMED:** index/trap identity, one-byte argument, 0/1 byte return, bitmap algorithm, common 3-generation table and mechanics, no direct firmware callers.
- **STRONG INFERENCE:** the class is character/text acceptability-related because it includes the full printable-ASCII interval plus selected control and international bytes.
- **UNKNOWN:** original vendor function name and precise policy for the non-ASCII members.

Raw firmware bytes, bitmap locations, private disassembly and audit corpus remain outside the public repository.
