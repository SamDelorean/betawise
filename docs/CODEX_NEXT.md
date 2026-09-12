# BetaWise Codex next task

## Execution context

The project now has a Linux integration workspace at `~/Projects/alphasmart`, coordinated from its local `work` branch. Before touching BetaWise from that machine, read the global workspace `AGENTS.md`, `CLAUDE.md`, and `README.md` if present, then read this repository's `AGENTS.md`.

The global workspace is an orchestration layer only. BetaWise retains its own publication branch and history. Any BetaWise source change must still be made on `sdk/abi-automation`; do not merge or copy the global `work` branch into BetaWise automatically.

## Global Codex setup scope

Configuring Codex to operate correctly from the Linux global workspace is now an explicit project deliverable, not an external prerequisite.

The setup must establish and verify all of the following before autonomous project work is considered ready:

1. Codex starts with working directory `~/Projects/alphasmart` for global-project tasks.
2. The global repository is on branch `work` before Codex edits any global orchestration file.
3. Codex reads the global `AGENTS.md`, `CLAUDE.md`, and `README.md` before repository-specific instructions.
4. Any launcher/script used for unattended iterations must fail closed if the global branch is not `work`, the worktree is unexpectedly dirty, or the requested child repository is on the wrong publication branch.
5. Child repositories keep their own branch rules. In particular, BetaWise work remains on `sdk/abi-automation`; the global `work` branch must never be pushed into or substituted for that branch.
6. Codex must report the resolved workspace path, active global branch, selected child repository/branch, exact validation commands, and final commit/result for every iteration.
7. Perform a non-destructive smoke test of the configuration before relying on it for unattended changes. Record the outcome in the global continuity/handoff documentation.

If any of these conditions cannot be verified, stop before making source changes and report the configuration gap.

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

1. Verify the global Linux workspace state and complete/confirm the Codex setup scope above before any unattended source-editing iteration.
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
