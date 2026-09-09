# OS3K A338 / index 206 — abort-like runtime re-entry

Status: **PARCIAL_CERRADO / NO_RESUELTO**

This note records reproducible functional evidence for OS3K trap A338 without publishing ROM bytes or private disassembly corpora.

## Source-first identity evidence

Historical BetaWise declares:

- `void abort(void);` in `os3k/os3k.h`.
- `DEFINE_SYSCALL(206, abort);` in `os3k/syscall.c`.
- Its wrapper macro emits the nominal A-line word `0xA000 + 4*index`, so index 206 corresponds to A338.

The later `ioma8/neo-re` BetaWise SDK copy preserves the same declaration and index mapping. This makes `abort` a reproducible historical analyst/API name for index 206. It is not claimed here to be an original AlphaSmart vendor symbol.

## NEO13 canonical firmware verification

Regression artifact: `verify_neo13_dual_entry_updater_transition_a338_2026-09-08.py` and its captured output in the private project evidence folder.

Canonical NEO13 SHA-256:

`32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

**EJECUTADA: 20/20 PASS.**

The verified handler corresponding to A338 performs this control-flow sequence:

1. set bit 7 of the byte at RAM address `0x35E2`;
2. call runtime re-entry entrypoint `0x00410070`;
3. a physical `RTS` follows the call in the handler image.

The verified `0x00410070` entry stub does not preserve the caller stack. It resets `A7` to `0x0007FFF0`, clears `A6`, then transfers control by `JMP` to a distinct runtime target. A nearby second entrypoint at `0x00410082` performs the same stack/base reset but jumps to a different target.

Consequently, the physical `RTS` after A338's call must **not** be interpreted as evidence that ordinary control returns to the A338 caller. Under the verified entry sequence, the caller's stack has been discarded before the runtime transfer. This is consistent with the historical `abort` interpretation and strongly supports an effective no-return contract for the A338 path in NEO13.

The same regression found exactly 11 direct calls to `0x00410070` in the canonical NEO13 image and exactly two exact sites that set `0x35E2.bit7`, one of them A338. Those facts constrain but do not yet name the semantic role of the bit or of the two re-entry destinations.

## Nearby alternate entry

A distinct verified response path carrying status/response value `0x52` reaches `0x00410082`, while the examined outer path contains no direct literal reference to `0x00410070`. This demonstrates two physically distinct runtime re-entry paths, but does **not** by itself justify naming them `updater` versus `normal`.

## Classification

- A338/index206 as a real ABI object: **CONFIRMADO for NEO13**, because historical wrapper/source evidence correlates with a valid canonical firmware handler.
- Historical API name `abort`: **INFERENCIA FUERTE**. Reproducible in BetaWise and neo-re; not promoted to an original vendor-symbol claim.
- Signature `void(void)`: **INFERENCIA FUERTE**, from both historical headers and absence of a demonstrated argument dependency in the verified handler fragment.
- Effective no-return behavior on the verified NEO13 A338 path: **CONFIRMADO structurally**. `0x00410070` replaces the stack and jumps onward rather than returning through the caller frame.
- `0x35E2.bit7` semantic name: **NO_RESUELTO / EVIDENCIA_INSUFICIENTE**.
- Semantic names for `0x00410070` and `0x00410082`: **NO_RESUELTO / EVIDENCIA_INSUFICIENTE**.
- Cross-generation equivalence (AS3000 / NEO05): **NO_RESUELTO** pending canonical comparison.

## Falsification result

Dominant alternative tested: *A338 may be a conventional returning function because an RTS follows its JSR.*

Result: **refuted for the verified NEO13 path**. The callee entry resets the stack pointer before an unconditional jump, so ordinary return via the caller's saved return address is not structurally available.

## Next evidence targets

1. Correlate all direct callers of `0x00410070` with source-visible reboot/reset/abort/updater operations.
2. Identify independent readers/writers of `0x35E2.bit7` and determine whether it selects a boot/update mode, records applet state, or has another role.
3. Reconstruct the distinct targets reached from `0x00410070` and `0x00410082` without assigning semantic names ahead of evidence.
4. Compare the equivalent object in canonical NEO05 and AS3000 before claiming a generation-independent contract.

No ROM image, extensive disassembly, or Ghidra database is included in this public note.
