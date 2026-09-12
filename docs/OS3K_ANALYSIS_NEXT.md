# OS3K Codex base-analysis task

## Role

This is the foundational and transversal OS3K reverse-engineering front.

Its findings feed:

- AlphaSmart 2000 reconstruction when applicable;
- AlphaSmart 3000 emulator;
- BetaWise / OS3K SDK;
- OS3K RAM and persistence documentation;
- BetaCalc / NeoSheet;
- later NEO work.

This front is not the BetaWise SDK publication task. Do not modify SDK
headers or implementation merely because an ABI contract exists.

## Evidence discipline

Preserve these distinctions:

- mechanically/directly demonstrated fact;
- strong historical or cross-version continuity;
- implementation inference;
- unresolved question.

Do not strengthen an inference into a fact.

Keep AS3000, early NEO, and NEO 2013 behavior separate whenever the
evidence differs.

Do not add proprietary ROMs, firmware images, application binaries,
manager distributions, dumps, or captured proprietary payloads to Git.

## First milestone

Create or update:

    docs/os3k-runtime-memory-storage.md

The purpose of this first milestone is to consolidate the existing project
evidence into a canonical map of OS3K runtime memory and file/storage behavior.

Do NOT restart reverse engineering that is already closed.

Begin from at least:

- docs/file-api-current-reference.md
- docs/file-api-reconstruction.md
- docs/file-core-operations.md
- docs/file-space-accounting.md
- docs/file-identity-dynamic-closure.md
- docs/fileopen-fileclose-closure.md
- docs/clipboard-buffer-closure.md
- docs/clipboard-state-capacity-closure.md
- docs/packed-record-cursor-closure.md
- docs/packed-record-search-closure.md
- docs/packed-record-writer-closure.md
- docs/applet-runtime-api-closure.md
- docs/abi-reconstruction-index.md
- docs/os3k-abi-consolidation-2026-09-11.md
- relevant SYS_Axxx closure documents

## Questions to organize

Document, with evidence level, what is currently known about:

1. OS3K RAM regions and runtime-owned structures.
2. File descriptor table layout and lifetime.
3. File data buffers and storage/base pointers.
4. Active descriptor and namespace state.
5. Clipboard storage and its relationship to file storage.
6. Record-oriented structures.
7. Allocation units, minimum allocation, maximum capacity, reclaimable space.
8. Volatile state versus persistent state.
9. What FileOpen/FileClose actually do.
10. What operations modify data immediately versus merely changing runtime state.
11. What is known, and not known, about physical persistence/backing storage.
12. Differences among AS3000, early NEO and NEO 2013.
13. Implications for emulator implementation.
14. Implications for BetaWise applications.
15. Implications for a spreadsheet application that must eventually save files.

Do not assume that a RAM storage pointer proves the physical persistent
medium or commit mechanism. Explicitly mark that boundary.

## Change boundary for this iteration

Documentation only.

Allowed substantive output:

- docs/os3k-runtime-memory-storage.md
- docs/OS3K_ANALYSIS_RESULT.md

Do not modify:

- os3k/*.h
- os3k/*.c
- applets/
- MAME repositories
- private evidence files

If a useful change is identified outside this boundary, record it as a
cross-project handoff rather than making the change.

## Result

Replace docs/OS3K_ANALYSIS_RESULT.md with a concise report containing:

- evidence reviewed;
- facts consolidated;
- unresolved RAM questions;
- unresolved persistence/storage questions;
- AS3000/NEO differences found;
- emulator-facing implications;
- BetaWise-facing implications;
- spreadsheet-facing implications;
- exact files changed;
- git diff --check result;
- recommended next OS3K analysis frontier.

Commit and push only if the documentation is coherent, evidence-preserving,
and the remote branch can be updated by fast-forward without force-push.
