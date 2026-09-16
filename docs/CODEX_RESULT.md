# BetaWise Codex result

## Current audit state

The SDK/ABI audit is active on `sdk/abi-automation`. The continuity result is maintained as an operational summary; detailed evidence remains in the dated audit documents.

## Latest safe reconciliation

The current audit has established a traceable Clipboard declaration-surface inconsistency without changing source/header files:

- Git history shows `ClipboardSet`, `ClipboardGet`, and `ClipboardClear` were assigned directly to A20C/A210/A214 in `syscall.c` in 2022, before the 2026 focused reconstruction header existed.
- The mechanically closed A20C/A210/A214 signatures are now documented in `file_clipboard_buffer.h`.
- `os3k.h` still does not declare those historical direct-veneer symbols.
- No evidence currently indicates an `os3k.c` wrapper layer for them; `syscall.c` exports the symbols directly.

The structurally consistent repair candidate is therefore direct declarations in the existing BetaWise `os3k.h` organization, not inclusion of reconstructed focused headers.

A further audit of A1D0/A1D4/A1D8/A1DC resolves an earlier umbrella-header candidate without source changes: `file_password_state.h` explicitly states that reliable original System 3 public symbols have **not** been recovered and intentionally retains neutral `SYS_Axxx` names. `syscall.c` likewise retains `SYS_A1D0` through `SYS_A1DC`. Their absence from `os3k.h` is therefore **not currently a demonstrated public-SDK visibility defect**. Do not expose them merely because their mechanical contracts are closed. This supersedes earlier inventory language that treated `file_password_state.h` as a candidate umbrella/public-header gap.

### Corrected A1A0/A1A8 name provenance

An earlier continuity paragraph incorrectly classified `FileSmashFile` and `FileGetCurrentFile` as reconstruction-assigned/descriptive names merely because commit `9f474207db34685d96e57753bee88dbc7d7dc6e2` performs the modern veneer rename. That conclusion is superseded.

The primary historical evidence had already been recorded in `docs/file-core-operations.md` by commit `7531b63142385c0c1c5053a30b726f1746e6b519`: the original AS3000 2000 `FileModule.c` / `FileModule.h` contains both `FileSmashFile` and `FileGetCurrentFile`. The modern handlers were independently matched across AS3000 2005, NEO 2005, and NEO 2013 firmware, with behavior/ABI confidence A and name-continuity confidence B (strong). Commit `9f474207...` is therefore the integration point for historically grounded names, not their evidentiary origin.

Classify both as:

`PUBLIC_COMPILATION_SURFACE / HISTORICAL_AS3000_NAME_RECOVERED / MODERN_OS3K_NAME_CONTINUITY_STRONG_NOT_SYMBOL-TABLE-PROVEN`

The modern ABI evolution remains explicit: A1A0 returns a canonical 16-bit token although historical `FileSmashFile` returned `void`; A1A8 returns the modern canonical 16-bit token rather than the historical 1-based `UInt8` file number. Historical name genealogy must not be confused with byte-for-byte ABI identity.

## Architectural rule

Preserve the original BetaWise organization unless concrete evidence requires correction:

- reconstructed ABI evidence determines the mechanical syscall contract;
- `os3k.h` is the historical public C/SDK declaration surface;
- `syscall.c` contains A-line veneers;
- `os3k.c` contains wrappers/adaptations where the historical SDK requires them;
- reconstructed focused headers are evidence/reconstruction artifacts, not automatic public subheaders.

Reconcile each contract individually as:

`closed ABI contract <-> syscall.c veneer <-> os3k.c wrapper if historically required <-> historical/direct os3k.h representation`

Do not create a parallel subheader-based public SDK architecture.

Historical symbol provenance is an independent dimension from mechanical ABI closure. A descriptive/reconstruction-assigned symbol may be retained by an explicit extended-SDK compatibility decision, but must not be represented as a recovered original BetaWise symbol without independent evidence. Conversely, a modern veneer rename is not evidence that the name itself was invented during reconstruction when primary historical source already establishes the name genealogy.

## ABI/SDK guardrails

- Do not promote `CERRADA_CON_LÍMITE_DE_EVIDENCIA`.
- Do not reopen a closed syscall without a concrete dependency or contradictory primary evidence.
- Do not expose A468/A46C absent a repository consumer plus independently justified exact C contract.
- Treat A0B4 cautiously; mechanical confidence alone is insufficient for a public C prototype.
- `CERRADA` is mechanical ABI closure, not proof of recovered vendor symbol or public SDK status.
- Neutral `SYS_Axxx` names remain neutral unless historical/public identity is independently evidenced.

## Build status

The latest audit work is documentation-only. No m68k compilation was executed and nothing new is `BUILD_VALIDATED`.

Any future change to `os3k.h`, `os3k.c`, or syscall veneers remains pending:

- `make -C os3k clean all`
- one representative applet clean build

Do not mark a source/header change `BUILD_VALIDATED` unless both commands actually run and pass on the configured Linux/m68k toolchain.

## Next safe frontier

Continue the closed-contract consistency audit using historical symbol provenance as a discriminator. Prefer demonstrated public-name/header asymmetries such as A20C/A210/A214. Where the focused header itself says original public names are unrecovered, as for A1D0-A1DC, retain the neutral reconstruction surface and do not manufacture a public API. Audit current named veneers against all available primary historical-source notes before classifying them as reconstruction-assigned; the date of the veneer rename alone is insufficient provenance evidence.
