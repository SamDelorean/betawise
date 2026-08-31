# OS3K forward-sweep traceability — late extension index

This is the extensible public traceability index for the late OS3K ABI sweep.
It complements the historical forward-sweep indexes without rewriting their
original checkpoints. Firmware bytes and extensive disassembly remain private.

| Trap | Index | Disposition | Static regression | Public evidence |
| --- | ---: | --- | ---: | --- |
| A4A0 | 296 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 115/115 PASS | [`abi-trace-a4a0.md`](abi-trace-a4a0.md) |
| A4A4 | 297 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 85/85 PASS | [`abi-trace-a4a4.md`](abi-trace-a4a4.md) |
| A4A8 | 298 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4a8.md`](abi-trace-a4a8.md) |
| A4AC | 299 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 77/77 PASS | [`abi-trace-a4ac.md`](abi-trace-a4ac.md) |
| A4B0 | 300 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4b0.md`](abi-trace-a4b0.md) |
| A4B4 | 301 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 91/91 PASS | [`abi-trace-a4b4.md`](abi-trace-a4b4.md) |
| A4B8 | 302 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 89/89 PASS | [`abi-trace-a4b8.md`](abi-trace-a4b8.md) |
| A4BC | 303 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 85/85 PASS | [`abi-trace-a4bc.md`](abi-trace-a4bc.md) |
| A4C0 | 304 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 85/85 PASS | [`abi-trace-a4c0.md`](abi-trace-a4c0.md) |
| A4C4 | 305 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 86/86 PASS | [`abi-trace-a4c4.md`](abi-trace-a4c4.md) |
| A4C8 | 306 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 80/80 PASS | [`abi-trace-a4c8.md`](abi-trace-a4c8.md) |
| A4CC | 307 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 79/79 PASS | [`abi-trace-a4cc.md`](abi-trace-a4cc.md) |
| A4D0 | 308 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4d0.md`](abi-trace-a4d0.md) |
| A4D4 | 309 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4d4.md`](abi-trace-a4d4.md) |
| A4D8 | 310 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 83/83 PASS | [`abi-trace-a4d8.md`](abi-trace-a4d8.md) |
| A4DC | 311 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4dc.md`](abi-trace-a4dc.md) |
| A4E0 | 312 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4e0.md`](abi-trace-a4e0.md) |
| A4E4 | 313 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4e4.md`](abi-trace-a4e4.md) |
| A4E8 | 314 | `NO_CONTRATO_CALLABLE / HUECO ABI` | 82/82 PASS | [`abi-trace-a4e8.md`](abi-trace-a4e8.md) |

`abi-forward-sweep-extension-a4a0-a4b4.md` is retained as the immutable
checkpoint created when index 301 was closed. This file is the forward-maintained
late-extension index from index 302 onward.

A `NO_CONTRATO_CALLABLE / HUECO ABI` row records a demonstrated non-callable
nominal slot, not an invented prototype. Individual traces preserve the exact
cross-generation slot evidence, negative caller/stub coverage, false-positive
classification and executed regression state.
