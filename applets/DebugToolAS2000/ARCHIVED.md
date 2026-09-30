# DebugTool-AS2000 archive notice

Status: **ARCHIVED / FRONT ABANDONED**

Date: **2026-09-30**

Reason: the overall AlphaSmart project direction changed. This front is
preserved as a completed technical reference and is no longer an active
implementation path.

## Final retained checkpoint

The last completed milestone is **Increment 9 — v0 consolidation**.

Retained state:

- MEM: PASS
- GOTO: PASS
- EDIT: PASS
- CALL: PASS
- INFO: PASS
- combined v0 regression: PASS
- reproducible build: PASS
- core .text: 1175 B
- diagnostic binding .text: 102 B
- executable .text total: 1277 B
- fixed DebugTool workspace: 30 B

## Frozen artifacts

The following remain intentionally preserved:

- portable HC11 core source
- diagnostic binding layer
- v0 build scripts
- MAME regression harnesses
- dependency/binding inventory
- ABI/RAM ledger
- size budget/history
- increment reports 3 through 9
- stock-ROM helper findings
- DynFS CALL/INFO diagnostic findings

## Cancelled continuation

The archived front will not proceed to:

- Increment 10 — current-ROM integration
- Increment 11 — later-ROM relocation

No ROM placement, final workspace assignment, entry hook or later-ROM
relocation should be derived from this branch as an active task.

## Reactivation rule

If this front is ever reconsidered, resume only by explicit decision. Do not
infer reactivation from unrelated AlphaSmart work.
