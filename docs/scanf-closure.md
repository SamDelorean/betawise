# OS3K A374 / index 221 — `scanf` closure

Status: **mechanically closed / confidence A**.

## Reconstructed contract

```c
int scanf(const char *fmt, ...);
```

A374 is the stdin-specific wrapper around the same scan engine used by the already reconstructed `fscanf` and `sscanf` entries.

## Primary cross-ROM evidence

- AS3000 2005: runtime `0x004E69EA`, file `+0x0269EA`.
- NEO 2005: runtime `0x005E8D32`, file `+0x028D32`.
- NEO 2013: runtime `0x00443C06`, file `+0x033C06`.
- Handler length: `0x30` bytes in all three ROMs, terminal `RTS` at `+0x2E`.
- Raw SHA-256: AS `2de0d11caf386d2410bd5c8374b306d59d5d6322184a7cfcdd1b2406f4b37131`; NEO05 `764675144513e2432359060a4d00f4f8f1509bf35a94a42cf47892ec2bd651c8`; NEO13 `82b8915f3793e9d665619027dd74923db29aa67dd9b2e3cc93b93659c722f90b`.
- Neutralizing only the four relocated absolute operands (two callbacks, stdin descriptor source, common scan engine) yields cross-ROM SHA-256 `6f838544e2723e9b1d2350fa77d52b8833a60dfd0d68a9ef49442ec5ee4da4a8`.

## ABI and scan-family correlation

With original entry stack pointer `S`, A374 reads `S+4` as the fixed format pointer and forms a pointer to `S+8` as the first variadic argument. It then calls the common scan engine with six longword slots equivalent to:

```text
mode = 0
context = stdin
get_callback = A348 / fgetc
unget_callback = A3B0 / ungetc-like adapter
format = fmt
varargs = &first_variadic_argument
```

The scan engine addresses are exactly the same as A354/`fscanf` in each generation: AS `0x004E6A4A`, NEO05 `0x005E8D92`, NEO13 `0x00443C66`. The callbacks are likewise the same A348 and A3B0 adapters used by A354.

The stdin context is loaded from the first longword of the ROM stream-descriptor row: AS `0x004EAC04`, NEO05 `0x005F152E`, NEO13 `0x0044F146`. In every canonical ROM the first three longwords of that row are `0`, `1`, `2`, matching the preserved SDK representation of stdin/stdout/stderr.

A354 differs mechanically by accepting the stream as an external fixed argument and therefore shifts `fmt` and the variadic tail one slot; A344/`sscanf` uses the same engine with string adapters and mode `1`. These independent correlations distinguish `scanf` from both `fscanf` and `sscanf` without relying on the historical symbol map.

The engine's full `D0.L` result is preserved through the wrapper epilogue and returned.

## Callers and adversarial checks

The official SmartApplet corpus was exhausted. All 30 table-bearing applets were hash-verified and scanned with the corrected PC-index detector; the remaining 11 are structural negatives without the relevant table. A374 has **0 executable callers in 41/41 applets**. ROM-wide direct JSR/JMP/BSR searches are also negative. The same detector finds 99 A36C calls and 598 A378 calls, providing positive controls for the negative A374 result.

Rejected alternatives:

- `fscanf`: no external stream slot; stdin is inserted by A374.
- `sscanf`: A374 uses the stream callbacks and mode `0`, not string adapters and mode `1`.
- fixed-arity function: the wrapper deliberately constructs a pointer to the variadic tail.
- `void`: the scan engine result is deliberately preserved in `D0.L`.
- PRNG seed setter: that role is physically implemented by A37C/index223, which writes the state used by A370/`rand`.

The historical BetaWise mapping `A374 = scanf` is therefore only secondary corroboration of a contract already established mechanically.

## Regression

A private reproducible static regression revalidates ROM hashes, vectors, handler boundaries and hashes, callback/engine identities, stdin descriptor values, A354 correlation, normalized hash, and the official caller sweep with positive controls. Result: **OVERALL PASS**. Dynamic emulator-first regression is specified but has not been executed.

Full ROM bytes, disassemblies, and caller workpapers remain private in Drive.
