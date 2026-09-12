# BetaWise Codex next task

## Execution context

The project now has a Linux integration workspace at `~/Projects/alphasmart`, coordinated from its local `work` branch. Before touching BetaWise from that machine, read the global workspace `AGENTS.md`, `CLAUDE.md`, and `README.md` if present, then read this repository's `AGENTS.md`.

The global workspace is an orchestration layer only. BetaWise retains its own publication branch and history. Any BetaWise source change must still be made on `sdk/abi-automation`; do not merge or copy the global `work` branch into BetaWise automatically.

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

Already reconciled and not to be repeated: A32C/A334, A0B8-A0D0, the closed NEO13 raw batch A3B4/A3B8/A3BC/A3C0/A404/A41C/A438/A470, and A250.

## Candidate selection rules

Prefer a candidate that:

1. is already `CERRADA` with mechanically demonstrated argument and return widths;
2. requires no new semantic guess;
3. has a clear inconsistency between canonical documentation, `os3k.h`, and/or an auxiliary header;
4. can be corrected without wrappers or unrelated architectural changes;
5. is smaller and safer than alternative candidates.

Do not promote `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entries.

Do not expose A468 or A46C unless an actual repository consumer requires them and their exact C contract is independently justified.

Treat A0B4 cautiously: its mechanics are strong but the public C prototype remains partly inferential. Do not select it merely because it is absent from `os3k.h`; only promote it if the exact callable C contract is independently demonstrated by the repository evidence.

If the safest result is that no additional declaration should be exposed, make no SDK source change and record that conclusion rather than weakening the evidence standard.

## Required workflow

1. Verify the global Linux workspace state and then the BetaWise repository state separately.
2. In BetaWise, require branch `sdk/abi-automation`, a clean worktree, and synchronization with `origin/sdk/abi-automation` before editing.
3. Audit closed contracts against public/auxiliary SDK declarations and choose one candidate only.
4. Trace chronology when documentation disagrees; later evidence does not automatically win unless it is technically better supported.
5. Make the minimum coherent correction.
6. Run:
   - `make -C os3k clean all`
   - one representative applet clean build
7. Update the relevant canonical/continuity documentation.
8. Write a concise result to `docs/CODEX_RESULT.md` including: candidate, files changed, exact build commands, PASS/FAIL, evidence basis, and next safe frontier.
9. Include `docs/CODEX_RESULT.md` in the same iteration commit. Commit and push only if the change is evidence-supported, build-valid, and the remote branch can be updated safely without force-push.

Do not call a change `BUILD_VALIDATED` unless both build gates actually ran on the Linux machine and passed.

## Cross-project handoff

If the audit exposes emulator-relevant behavior that should not become public SDK API, document it as an emulator-facing finding instead of strengthening the header. Do not modify `mame-as3k` from the BetaWise iteration. Record a concise handoff for the global workspace so that the MAME front can consume it in a separate change.

## Legal boundary

Do not add ROMs, firmware images, proprietary applications, dumps, Manager binaries, or proprietary captured payloads to Git.
