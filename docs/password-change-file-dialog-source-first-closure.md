# A1E4 / PasswordChangeFileDialog — source-first closure

Status: mechanically and semantically closed from canonical firmware evidence. This audit also corrects the inherited five-character password statement in the older block documentation.

```c
int32_t PasswordChangeFileDialog(uint16_t file_id);
```

## Source-first correlation

Recovered early AlphaSmart PasswordModule material provides the historical `PasswordChangeFileDialog` concept operating on the implicit current file. Existing System 3 reconstruction identified A1E4 as the evolved explicit-file version and recovered its UI strings and token/status return behavior. Those references were treated as anchors only and rechecked against all three canonical ROMs.

A1DC, immediately preceding this work, independently proved that modern per-file password access accepts NUL-terminated strings of length 0 through 6 and rejects only lengths greater than 6.

## Canonical firmware confirmation

A1E4 is vector index 121. Canonical entry points:

- AS3000 2005: `0x004E308E`;
- NEO 2005: `0x005E59EA`;
- NEO 2013: `0x0043BDEC`.

The public handler is exactly `0x2DE` bytes in all three images. Each body has one terminal `RTS` and 19 absolute internal `JSR` instructions. Raw same-position byte identity is approximately 90% across generations, with the same control structure and relocated private targets.

The handler contains a direct call to the already source-first-closed A084/TextBox at relative offset `+0x180` in every ROM. Immediately before that call, all three images construct the same password-input arguments, including:

- `password = 1`;
- `max_len = 6`;
- an exit-key list containing Escape (`0x48`) and terminator `0xFF` plus the same private/control marker byte.

The handler also calls A08C/StringCompare three times at relative offsets `+0x1C8`, `+0x1E0` and `+0x200` in every generation. The same private `strlen` and `strcpy` helpers independently identified during A1DC are called later in A1E4.

## Confirmed behavior

1. The single argument is a modern 16-bit `file_id` consumed from its ABI slot and resolved through the File API.
2. On successful resolution, the routine presents an interactive password-change flow for that descriptor.
3. The observed firmware strings and control flow correspond to the sequence: current password, new password, confirm new password, with explicit feedback for incorrect current password, mismatched confirmation, and empty new password.
4. Current-file authorization accepts either the stored file password or the master password.
5. The new password and confirmation must compare equal, and the new password must be non-empty.
6. Password entry is performed by A084/TextBox in password mode with effective `max_len=6`. Therefore the modern change dialog itself independently permits entry of up to six characters.
7. On accepted change, the new value is copied into the descriptor password state using the same string-copy family corroborated by A1DC.
8. For a successfully resolved descriptor, dialog exit returns the descriptor's canonical token, including cancellation. Therefore the return value is token/status, not a boolean `password_changed` result.
9. Resolver failure propagates the resolver's status rather than fabricating a successful token.

## Password-length correction

Older project documentation inherited the early-module five-character password convention and described the modern storage as “five characters plus NUL.” That statement is false for the audited System 3 firmware.

Two independent primary paths now agree:

- A1DC accepts `strlen(password) <= 6` and rejects only `>6`;
- A1E4 invokes TextBox with effective `max_len=6` while changing a file password.

The modern contractual boundary is therefore **up to six password characters, followed by NUL in caller/descriptor string storage**. The exact physical field padding beyond the terminator is an implementation detail and is not inferred from the historical source.

## Refutation

- Refuted projecting the historical implicit-current-file ABI onto A1E4: the modern function consumes an explicit 16-bit token.
- Refuted a boolean return contract: a resolved cancellation still returns the canonical token.
- Refuted the inherited five-character limit using an independent direct TextBox call with `max_len=6` in all three ROMs.
- Refuted interpreting the UI as a simple setter: the handler performs current-password authorization and new/confirmation equality checks before copying.

## Classification

**CONFIRMED:** one-argument modern ABI; explicit File API resolution; interactive current/new/confirm flow; master-or-file authorization; new/confirm equality; non-empty new password; password-mode TextBox with max 6; canonical-token/status return; cross-ROM equivalence.

**INFERENCIA FUERTE:** continuity of A1E4 with the historical vendor name `PasswordChangeFileDialog`.

**DESCONOCIDO:** symbolic names of several private UI/helper routines and raw resolver error names.

## Regression

Static primary-byte regression was **EXECUTED: 72/72 PASS** against AS3000 2005, NEO 2005 and NEO 2013. It verifies canonical ROM hashes, handler boundaries/size, prologue/epilogue, single terminal RTS, 19 internal absolute JSRs, direct A084/TextBox target, identical password-mode/max-6 argument construction, three A08C/StringCompare calls, shared A1DC string helpers and cross-generation structural invariants.

Dynamic regression is **SPECIFIED / NOT EXECUTED** and should remain emulator-first with disposable state: correct file password, correct master password, incorrect current password, Escape/cancel, empty new value, lengths 1..6, attempted seventh character, mismatch confirmation, successful change/readback through A1DC and canonical-token return on both accepted completion and cancellation.

Firmware bytes, complete disassembly and ROM-derived workpapers remain private.