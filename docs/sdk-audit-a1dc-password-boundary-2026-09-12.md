# SDK audit — A1DC modern password-length boundary

Status: **AUDIT_FINDING / DOCUMENTATION_CORRECTION_REQUIRED**

## Finding

The current branch contains an internal documentation contradiction for `SYS_A1DC`.

The source-first closure and public auxiliary header agree on the modern System 3 contract:

```c
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);
```

For write mode (`read_back == 0`):

- `strlen(password) <= 6` is accepted and copied;
- `strlen(password) > 6` returns raw status `-8` without copying.

This boundary is established directly from the canonical firmware handler: the length is compared with immediate `6`, and the copy path is taken with unsigned `BLS` (lower-or-same). The source-first static regression is recorded as **96/96 PASS**.

## Stale statements found

Two later/current documents still carry the older historical five-character interpretation:

1. `docs/file-api-current-reference.md`
   - descriptor `+0x18` is described as "maximum 5 chars + NUL";
   - the A1DC section states that strings of length `>= 6` return `-8`.
2. `docs/os3k-runtime-memory-storage.md`
   - descriptor `+0x18` is described as "at most five characters plus NUL".

Those statements are superseded by the later source-first A1DC revalidation in `docs/file-password-state-closure.md` and by `os3k/file_password_state.h`.

## Historical boundary

The older five-character limit belongs to the historical 2000 `PasswordModule` source and must not be projected onto the modern OS3K A1DC ABI. The modern handler is bidirectional and accepts six-character strings.

## Required correction

Canonical/current documentation should be reconciled to say that the modern A1DC file-password write path accepts lengths `0..6` and rejects lengths `> 6`. Historical notes may retain the older five-character rule only when clearly labeled as historical behavior.

No SDK C prototype change is required: `os3k/file_password_state.h` already matches the closed ABI. No inventory status changes are required.

## Validation classification

- ABI mechanics: **CERRADA / CONFIRMADO**
- static source-first regression: **EXECUTED — 96/96 PASS**
- dynamic regression: **SPECIFIED / NOT EXECUTED**
- this audit iteration: documentation-only finding; no firmware test or build rerun claimed.
