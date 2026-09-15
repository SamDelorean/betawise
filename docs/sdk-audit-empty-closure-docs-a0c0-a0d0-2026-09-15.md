# SDK audit — empty closure-document stubs A0C0–A0D0

Date: 2026-09-15
Branch: `sdk/abi-automation`

## Material documentation inconsistency

The repository currently contains zero-byte files named as closure documents for:

- `sys-a0c0-sticky-keys-state-setter-closure.md`
- `sys-a0c4-slow-keys-state-closure.md`
- `sys-a0c8-slow-key-delay-closure.md`
- `sys-a0cc-show-marks-flag-closure.md`
- `sys-a0d0-show-marks-flag-setter-closure.md`

At the same time, `os3k/os3k.h` publicly declares `SYS_A0C0` through `SYS_A0D0`, and `os3k/syscall.c` emits the corresponding veneers. A file name ending in `-closure.md` must not be treated as closure evidence when the file is empty.

## Audit rule

These empty files are documentation stubs only. They provide **no evidence** and must not be used to promote, reopen, or otherwise change the status of the corresponding syscalls. Existing ABI/status evidence, if any, must be taken from the actual reconstruction index/workpapers or primary firmware evidence.

This finding does not assert that A0C0–A0D0 are open or closed; it only removes the unsafe inference `closure filename => documented closure`.

## SDK consequence

No ABI/header/source change is made in this batch. The public declarations and veneers are left untouched pending a separate evidence reconciliation. Therefore this documentation-only correction does not require an m68k build and does not establish `BUILD_VALIDATED`.
