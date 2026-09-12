# OS3K Codex base-analysis task

## Role

This is the foundational and transversal OS3K reverse-engineering front.

The initial runtime/file-storage consolidation is complete. Do not repeat it.

## Current frontier

Resolve as far as the evidence permits the initialization and ownership of the
OS3K file descriptor table and file-storage arena, starting with AS3000 System 3
November 2005.

The previous analysis specifically left unresolved:

- descriptor-table root and exact bounds;
- file-storage arena root/end;
- initialization writers;
- cold/default initialization versus warm/resume behavior;
- relocation/compaction ownership;
- relationship between descriptor storage pointers and allocator state.

## Method

Use source-first correlation before blind binary exploration.

Begin from the existing closures and canonical map, including:

- docs/os3k-runtime-memory-storage.md
- docs/file-api-current-reference.md
- docs/file-api-reconstruction.md
- docs/file-core-operations.md
- docs/file-space-accounting.md
- docs/file-identity-dynamic-closure.md
- docs/fileopen-fileclose-closure.md
- docs/abi-reconstruction-index.md
- relevant SYS_A1xx/A2xx source-first closures

When locally available, canonical firmware/Ghidra/private evidence may be read
to resolve exact writers, globals and boundaries. Do not commit proprietary
material or extensive extracted disassembly.

For each proposed root, pointer or boundary:

1. identify the writer/initializer;
2. identify readers/consumers;
3. distinguish pointer storage from pointed-to storage;
4. establish width and lifetime where possible;
5. identify cold/warm/reset behavior where evidence exists;
6. compare with NEO only after the AS3000 unit is mechanically coherent;
7. actively test alternative interpretations;
8. mark anything not demonstrated as unresolved.

Do not infer physical persistence merely from a RAM pointer or runtime allocator.

## Allowed substantive output

- docs/os3k-descriptor-storage-init-closure.md
- docs/os3k-runtime-memory-storage.md
- docs/OS3K_ANALYSIS_RESULT.md

Documentation only for this iteration.

Do not modify:

- os3k/*.h
- os3k/*.c
- applets/
- MAME repositories
- private evidence files

## Result

Replace `docs/OS3K_ANALYSIS_RESULT.md` with a concise report containing:

- evidence reviewed;
- descriptor-table roots/bounds demonstrated;
- storage-arena roots/bounds demonstrated;
- initialization writers found;
- cold/warm behavior demonstrated or unresolved;
- alternative hypotheses rejected or retained;
- AS3000/NEO implications;
- emulator-facing handoff;
- BetaWise-facing handoff;
- spreadsheet/storage-facing handoff;
- exact files changed;
- validation performed;
- recommended next frontier.

Commit and push only if the result remains inside this scope and
`origin/os3k/base-analysis` can be updated by fast-forward.
