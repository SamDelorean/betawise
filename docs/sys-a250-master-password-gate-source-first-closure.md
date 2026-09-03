# SYS_A250 — source-first closure

Status: **CLOSED A / SOURCE-FIRST**

## Public contract

```c
uint8_t SYS_A250(uint8_t prompt_variant, const char *optional_text);
```

`SYS_A250` is the interactive master-password gate used by System 3. The modern vendor name was not recovered, so the neutral A-line name is retained.

## Evidence synthesis

The source-first pass correlates the existing SDK/syscall index, historical password work, official NEO user documentation, previous private reverse-engineering artifacts, all three canonical firmware generations, and concrete native callers.

Official NEO documentation describes master passwords as 1–6 characters and passwords as case-insensitive. Firmware independently matches those user-visible rules: A250 configures its password-entry path with a literal maximum length of 6 and compares the entered text against the shared master-password state through the same case-insensitive comparison machinery already recovered for A24C.

## Confirmed firmware behavior

Across AS3000 2005, NEO 2005, and NEO 2013:

- the handler is structurally equivalent and exactly `0x170` bytes;
- the low byte of the first ABI slot is read and forwarded into the prompt/UI setup path;
- `optional_text == NULL` is accepted; a non-NULL pointer is displayed before password entry;
- password entry is configured with a literal maximum length of 6;
- only the affirmative result `0x40` proceeds to master-password comparison;
- a matching master password terminates with low-byte return value `1`;
- a non-affirmative/cancel result terminates with low-byte return value `0`;
- a wrong password does **not** immediately return false: System 3 runs its failure feedback/delay sequence and retries the entry loop;
- only the low byte of D0 is contractual on return.

The password comparison uses the same shared master-password object as A24C. The exact UI meaning/name of `prompt_variant` remains unknown; it is intentionally not assigned a vendor-style symbolic name.

## Correction to retrospective header

An earlier provisional header described the first 32-bit argument slot as unused/reserved. Firmware refutes that interpretation: each canonical handler reads its low byte. The ABI declaration is therefore corrected from a reserved `uint32_t` argument to `uint8_t prompt_variant`.

All six direct native callers found in each canonical ROM pass the literal value `2` in that first slot. This confirms the slot is present and stable, but does not by itself establish the human-readable semantic label of selector value `2`.

## Caller/xref review

Direct absolute JSR references to A250 in the complete canonical ROMs:

- AS3000 2005: 6
- NEO 2005: 6
- NEO 2013: 6

The corresponding call sequences consistently supply two ABI slots, with the first slot equal to `2`.

## Refutation checks

The firmware evidence refutes the following alternatives:

- **first argument is unused/reserved** — false; its low byte is consumed by the handler;
- **wrong password returns 0 immediately** — false; it enters the failure-feedback path and retries;
- **password length is five characters** — false for this modern OS3K path; the handler supplies maximum length 6, consistent with official NEO documentation;
- **A250 compares file-local password state** — false; it uses the same shared master-password state as A24C;
- **second argument must be non-NULL** — false; NULL has an explicit skip path.

## Regression

Static source-first regression: **48/48 PASS — EXECUTED**.

Checks cover canonical ROM identity, handler bounds/structure, argument extraction, optional-pointer branch, six-character entry limit, affirmative gate, comparison path, success/cancel returns, failure retry loop, and low-byte return behavior across all three generations.

Dynamic regression remains **SPECIFIED / NOT EXECUTED** and should be performed emulator-first when the UI/password path is available.

## Confidence

- ABI shape and low-byte first argument: **CONFIRMED**
- interactive master-password gate role: **CONFIRMED**
- six-character maximum in this entry path: **CONFIRMED**
- case-insensitive master-password comparison: **CONFIRMED**
- wrong-password feedback + retry behavior: **CONFIRMED**
- exact vendor name and symbolic meaning of `prompt_variant`: **UNKNOWN**
