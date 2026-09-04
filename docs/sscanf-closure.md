# A344 / `sscanf` — ABI closure

Status: **mechanically closed (A) / source-first revalidated / published contract**.

## Contract

```c
int sscanf(const char *str, const char *fmt, ...);
```

## Primary reconstruction

A344 is a compact 0x30-byte wrapper in AS3000 2005, NEO 2005, and NEO 2013. Each version ends in `RTS` at +0x2E. After normalizing three generation-specific absolute operands, all three handlers have SHA-256 `25dc601166e6e558c8ecc56846826845d314f4c694377daf975a71be895851ff`.

The wrapper constructs a pointer to the caller's variadic argument area, fetches the caller's second argument as the format pointer, pushes two relocated helper/context addresses, derives a pointer from the caller's first argument as the string source, pushes constant 1, and calls a common scanning helper. It then removes 0x18 bytes of helper arguments and returns the helper's D0.L result unchanged.

That mechanical structure independently matches the historical A344=`sscanf` mapping and the BetaWise prototype above: string source, format string, variadic output argument area, common scan engine, and an integer result.

This closure deliberately does not import undocumented scanf-family conversion details, EOF behavior, `errno`, extensions, or edge-case semantics that are not established by the wrapper itself.

## Source-first correlation

The historical `sscanf` mapping and prototype are used as an identity hypothesis only. The firmware wrapper independently establishes the required shape: caller string source, format pointer, a pointer into the caller's variadic output area, dispatch into the shared scanning engine, cleanup of the six helper slots, and direct propagation of the engine's D0.L result. Those primary mechanics are sufficient to confirm the variadic scanning contract; the historical source supplies the recovered standard-library name.

The negative official caller result is not treated as proof of ABI. It is independently controlled by the same detector recovering 67 A33C calls, while A344's argument and return contract comes from its explicit stack/dataflow reconstruction.

## Caller survey

The corrected PC-index A-line detector exhausted the canonical official corpus: 30 table-bearing SmartApplets plus 11 structural negatives, 41/41 total. A344 has **0 executable SmartApplet callers**. As an adversarial detector control, the same algorithm finds 67 A33C callers in the same corpus.

Caller absence is not used to infer the variadic ABI or return type; those are determined by the handler's explicit stack construction and propagation of the scan helper's D0.L result.

## Validation

Canonical ROM hashes, vectors, handler length, raw hashes, normalized equivalence, stack reconstruction, helper-argument cleanup, result propagation, historical mapping correlation, and the 41/41 caller sweep were checked by the private static regression: **OVERALL PASS**. Dynamic execution was not performed.

Private workpapers retain exact ROM addresses, disassembly-level details, caller-scan methodology, and reproducible regression evidence.
