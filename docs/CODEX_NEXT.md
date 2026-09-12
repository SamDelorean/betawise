# BetaWise Codex next task

## Goal

Perform one low-risk SDK consistency audit against the already reconstructed OS3K ABI.

The sequential A-line discovery phase through A470 is complete. Do not search for new traps merely to extend the sequence. Instead, identify exactly one mechanically closed contract whose current public SDK representation is missing, stale, duplicated, or inconsistent with later source-first evidence.

## Starting evidence

Use these as primary project state:

- `docs/abi-reconstruction-index.md`
- `docs/os3k-abi-consolidation-2026-09-11.md`
- the relevant closure/source-first document for the candidate
- `os3k/os3k.h`
- auxiliary headers under `os3k/`

A250 is already reconciled and must not be repeated.

## Candidate selection rules

Prefer a candidate that:

1. is already mechanically closed;
2. requires no new semantic guess;
3. has a clear inconsistency between canonical documentation, `os3k.h`, and/or an auxiliary header;
4. can be corrected without wrappers or unrelated architectural changes;
5. is smaller and safer than alternative candidates.

Do not promote `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entries.

Do not expose A468 or A46C unless an actual repository consumer requires them and their exact C contract is independently justified.

## Required workflow

1. Fetch and inspect current branch state.
2. Audit closed contracts against public/auxiliary SDK declarations.
3. Select one candidate only.
4. Trace chronology when documentation disagrees; later evidence does not automatically win unless it is technically better supported.
5. Make the minimum coherent correction.
6. Run:
   - `make -C os3k clean all`
   - one representative applet clean build
7. Update the relevant canonical/continuity documentation.
8. Write a concise result to `docs/CODEX_RESULT.md`.
9. Include `docs/CODEX_RESULT.md` in the same iteration commit. Commit and push only if the change is evidence-supported, build-valid, and the remote branch can be updated safely without force-push.

If no defensible low-risk correction exists, make no source change and document that result instead.

## Legal boundary

Do not add ROMs, firmware images, proprietary applications, dumps, Manager binaries, or proprietary captured payloads to Git.
