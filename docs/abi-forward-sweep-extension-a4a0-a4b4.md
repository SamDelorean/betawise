# OS3K forward-sweep traceability — A4A0 through A4B4

This public supplement indexes the mechanically closed late-extension slots from
index 296 through index 301. It contains no firmware bytes or extensive
disassembly; primary workpapers remain private in Drive.

| Trap | Index | Disposition | Static regression | Public evidence |
| --- | ---: | --- | ---: | --- |
| A4A0 | 296 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 115/115 PASS | [`abi-trace-a4a0.md`](abi-trace-a4a0.md) |
| A4A4 | 297 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 85/85 PASS | [`abi-trace-a4a4.md`](abi-trace-a4a4.md) |
| A4A8 | 298 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4a8.md`](abi-trace-a4a8.md) |
| A4AC | 299 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 77/77 PASS | [`abi-trace-a4ac.md`](abi-trace-a4ac.md) |
| A4B0 | 300 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4b0.md`](abi-trace-a4b0.md) |
| A4B4 | 301 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 91/91 PASS | [`abi-trace-a4b4.md`](abi-trace-a4b4.md) |

For every row above, the nominal vector position is occupied by a value that is
not a valid canonical firmware runtime pointer, the complete canonical ROM sweep
does not establish a handler, and the official SmartApplet corpus does not
establish a callable table stub. The individual trace documents record the
slot values, raw-word false-positive classification and generation-specific
evidence.

These are deliberately neutral ABI dispositions. The table does not assign
vendor names, prototypes, arguments, return semantics, enums, flags, errors or
subsystem meanings to non-callable slots.
