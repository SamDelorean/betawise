# SDK audit — `sdk/abi-automation` / `master` divergence (2026-09-15)

## Material finding

The audit branch is no longer a strict descendant of current `master`.

GitHub comparison at audit head `9f68587c02f8a0aa9eacd9c4f8bb2f13920a843c` reports:

- merge base: `a1306c113d5861e7cbed09595d58ec114d30d1b1`
- `sdk/abi-automation`: 729 commits ahead of `master`
- `sdk/abi-automation`: 3 commits behind `master`
- relationship: `diverged`

The three `master`-only commits are documentation/evidence relevant to the File API audit:

1. `54f8c5181d36988e289ce8dbf254d1e6c7b6bff2` — `docs: document AS3000 battery-backed file storage`
2. `98cdecf41786bd448ce6320041e44f292f07040e` — `docs: correlate file allocator with System 3 manual`
3. `ad557fcf4efbf07fcf0ff3f398a71a7987b7a41c` — `docs: close three-generation System 3 file allocator mechanics`

## Audit consequence

Do **not** treat the audit branch alone as the complete evidence corpus for File API/header reconciliation until these three `master`-only documents have been explicitly reconciled. Their absence from the branch does not reopen or downgrade any already closed syscall, but it can hide dependencies or supporting evidence when judging public File API declarations and BetaWise compatibility.

This finding is about source/evidence continuity only. It does not authorize a merge/rebase during the ABI audit and does not change any syscall classification.

## Safety status

- ABI change: none
- header/source change: none
- syscall reopened: none
- `CERRADA_CON_LÍMITE_DE_EVIDENCIA` promotion: none
- m68k build required: no (documentation-only)
- `BUILD_VALIDATED`: not claimed
