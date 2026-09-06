# OS3K forward-sweep traceability — late extension index

This is the extensible public traceability index for the late OS3K sweep.
It complements the historical forward-sweep indexes without rewriting their
original checkpoints. Firmware bytes and extensive disassembly remain private.

> **Structural-model correction (2026-09-06):** a nominal `A000 + 4*index`
> coordinate is not automatically a syscall. Direct dispatcher analysis shows
> a 256-entry mechanical window (`A000..A3FC`) on the 2005 AS3000/NEO firmware
> and a 512-entry window (`A000..A7FC`) on NEO 2013. In addition, NEO 2013
> contains a separate homologous 32-entry internal function-pointer structure
> at nominal coordinates `A4C8..A544`. Rows intersecting that structure are
> therefore marked for structural reaudit rather than treated as uniform ABI
> holes. See the corrected A500 trace for the first completed example.

| Trap | Index | Disposition | Historical static regression | Public evidence |
| --- | ---: | --- | ---: | --- |
| A4A0 | 296 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 115/115 PASS | [`abi-trace-a4a0.md`](abi-trace-a4a0.md) |
| A4A4 | 297 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 85/85 PASS | [`abi-trace-a4a4.md`](abi-trace-a4a4.md) |
| A4A8 | 298 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 82/82 PASS | [`abi-trace-a4a8.md`](abi-trace-a4a8.md) |
| A4AC | 299 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 77/77 PASS | [`abi-trace-a4ac.md`](abi-trace-a4ac.md) |
| A4B0 | 300 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 82/82 PASS | [`abi-trace-a4b0.md`](abi-trace-a4b0.md) |
| A4B4 | 301 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 91/91 PASS | [`abi-trace-a4b4.md`](abi-trace-a4b4.md) |
| A4B8 | 302 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 89/89 PASS | [`abi-trace-a4b8.md`](abi-trace-a4b8.md) |
| A4BC | 303 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 85/85 PASS | [`abi-trace-a4bc.md`](abi-trace-a4bc.md) |
| A4C0 | 304 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 85/85 PASS | [`abi-trace-a4c0.md`](abi-trace-a4c0.md) |
| A4C4 | 305 | `NO_CONTRATO_CALLABLE / HUECO ABI` (historical; structural classification pending) | 86/86 PASS | [`abi-trace-a4c4.md`](abi-trace-a4c4.md) |
| A4C8 | 306 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 0 | 80/80 PASS (historical ABI-hole test) | [`abi-trace-a4c8.md`](abi-trace-a4c8.md) |
| A4CC | 307 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 1 | 79/79 PASS (historical ABI-hole test) | [`abi-trace-a4cc.md`](abi-trace-a4cc.md) |
| A4D0 | 308 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 2 | 82/82 PASS (historical ABI-hole test) | [`abi-trace-a4d0.md`](abi-trace-a4d0.md) |
| A4D4 | 309 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 3 | 82/82 PASS (historical ABI-hole test) | [`abi-trace-a4d4.md`](abi-trace-a4d4.md) |
| A4D8 | 310 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 4 | 83/83 PASS (historical ABI-hole test) | [`abi-trace-a4d8.md`](abi-trace-a4d8.md) |
| A4DC | 311 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 5 | 82/82 PASS (historical ABI-hole test) | [`abi-trace-a4dc.md`](abi-trace-a4dc.md) |
| A4E0 | 312 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 6 | 82/82 PASS (historical ABI-hole test) | [`abi-trace-a4e0.md`](abi-trace-a4e0.md) |
| A4E4 | 313 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 7 | 82/82 PASS (historical ABI-hole test) | [`abi-trace-a4e4.md`](abi-trace-a4e4.md) |
| A4E8 | 314 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 8 | 82/82 PASS (historical ABI-hole test) | [`abi-trace-a4e8.md`](abi-trace-a4e8.md) |
| A4EC | 315 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 9 | 86/86 PASS (historical ABI-hole test) | [`abi-trace-a4ec.md`](abi-trace-a4ec.md) |
| A4F0 | 316 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 10 | 83/83 PASS (historical ABI-hole test) | [`abi-trace-a4f0.md`](abi-trace-a4f0.md) |
| A4F4 | 317 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 11 | 85/85 PASS (historical ABI-hole test) | [`abi-trace-a4f4.md`](abi-trace-a4f4.md) |
| A4F8 | 318 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 12 | 81/81 PASS (historical ABI-hole test) | [`abi-trace-a4f8.md`](abi-trace-a4f8.md) |
| A4FC | 319 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 13 | 81/81 PASS (historical ABI-hole test) | [`abi-trace-a4fc.md`](abi-trace-a4fc.md) |
| A500 | 320 | `STRUCTURALLY_CLASSIFIED / NO_UNIFORM_SYSCALL` — USB descriptor data on 2005; NEO13 secondary table ordinal 14 | 23/23 structural PASS; prior 41/41 caller-negative evidence retained | [`abi-trace-a500.md`](abi-trace-a500.md) |
| A504 | 321 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 15 | 185/185 PASS (historical ABI-hole test) | [`abi-trace-a504.md`](abi-trace-a504.md) |
| A508 | 322 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 16 | 186/186 PASS (historical ABI-hole test) | [`abi-trace-a508.md`](abi-trace-a508.md) |
| A50C | 323 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 17 | 214/214 PASS (historical ABI-hole test) | [`abi-trace-a50c.md`](abi-trace-a50c.md) |
| A510 | 324 | `STRUCTURAL_REAUDIT_REQUIRED` — NEO13 secondary function table ordinal 18 | 175/175 PASS (historical ABI-hole test) | [`abi-trace-a510.md`](abi-trace-a510.md) |

`abi-forward-sweep-extension-a4a0-a4b4.md` is retained as the immutable
checkpoint created when index 301 was closed. This file is the forward-maintained
late-extension index from index 302 onward.

Historical regression results are preserved as evidence of what was tested at
the time; they are not reinterpreted as proof that the corresponding nominal
coordinate is a syscall. Structural classification now precedes ABI analysis.