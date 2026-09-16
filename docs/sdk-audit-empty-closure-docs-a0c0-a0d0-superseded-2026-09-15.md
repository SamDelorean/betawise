# SDK audit — A0C0–A0D0 empty-closure finding superseded

Date: 2026-09-15
Branch: `sdk/abi-automation`

## Correction

The previous audit note `sdk-audit-empty-closure-docs-a0c0-a0d0-2026-09-15.md` is **SUPERSEDED**.

Direct inspection of the branch tree at commit `e9e87da9a35cbe2e5d4025db6fb9e679e770185d` shows that the five closure documents are not empty:

- `sys-a0c0-sticky-keys-state-setter-closure.md` — 3515 bytes
- `sys-a0c4-slow-keys-state-closure.md` — 5343 bytes
- `sys-a0c8-slow-key-delay-closure.md` — 6228 bytes
- `sys-a0cc-show-marks-flag-closure.md` — 5006 bytes
- `sys-a0d0-show-marks-flag-setter-closure.md` — 4053 bytes

Therefore the earlier zero-byte observation must not be used as evidence in SDK/ABI decisions.

## Audit consequence

No syscall status is changed by this correction. In particular, this note does not promote any `CERRADA_CON_LÍMITE_DE_EVIDENCIA`, does not reopen any closed syscall, and makes no ABI/header/source change.

The general rule remains valid that a filename alone is not closure evidence, but A0C0–A0D0 cannot be cited as examples of empty closure stubs.

## Build status

Documentation-only correction. No m68k build is required and no `BUILD_VALIDATED` claim is made.
