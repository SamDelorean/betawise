# A24C / SYS_A24C — source-first closure

Status: **CERRADO A / SOURCE-FIRST**  
Date: 2026-09-03 10:49 America/Mexico_City

## Contract

```c
uint8_t SYS_A24C(const char *password);
```

A24C compares a valid NUL-terminated candidate string against the current master-password string and returns low-byte `1` on equality and `0` on inequality. Only `D0.B` is contractual.

`password` must be non-NULL and point to a readable NUL-terminated string. The handler performs no NULL guard before dispatching to its string-comparison helper; NULL is therefore outside the supported contract rather than a defined false result.

## Source-first correlation

The historical SDK and public BetaWise surface do not recover a reliable vendor name for index 147, so the neutral name `SYS_A24C` is retained. Earlier project material associated A24C with master-password comparison. That was treated only as an anchor and revalidated against the three canonical ROMs.

The existing `password_runtime.h` declaration already matches the recovered ABI and therefore requires no code change.

## Firmware-confirmed behavior

Canonical ROM SHA-256 values were reverified directly:

* AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
* NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
* NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

A24C is exactly `0x26` bytes in all three generations:

| ROM | runtime entry | file offset | master-password global | compare helper |
|---|---:|---:|---:|---:|
| AS3000 2005 | `0x004E04F6` | `0x0204F6` | `0x00004473` | `0x004CC388` |
| NEO 2005 | `0x005E2E52` | `0x022E52` | `0x000058A5` | `0x005CBEE0` |
| NEO 2013 | `0x00439250` | `0x029250` | `0x000118A3` | `0x0041C900` |

The handler mechanics are structurally identical after relocation:

1. supplies two zero-valued trailing arguments to the private comparison helper;
2. passes the caller's first pointer argument and the generation-specific master-password global;
3. invokes the same four-argument comparison implementation used for unbounded, case-insensitive string comparison;
4. tests the helper result;
5. maps comparison result `0` (equal) to `D0.B = 1`, and nonzero to `D0.B = 0`;
6. returns without modifying the password data or master-password storage.

The helper dereferences its string arguments in its scan loop and contains no preceding NULL-pointer guard. This closes the previously open `NULL` question: NULL is unsafe/out-of-contract, not a documented false case.

## Caller correlation

A complete absolute-JSR scan of each canonical ROM finds exactly **1 / 1 / 1** direct A24C caller. Each caller passes a pointer to a local password buffer, removes one 32-bit argument after return, and immediately tests `D0.B` as an equality predicate. This independently supports both the one-pointer ABI and the boolean-byte return.

The surrounding native password workflow also uses the same generation-specific master-password global and comparison helper, providing an independent semantic anchor for the global's role.

## Cross-generation result

No semantic divergence was found. Differences are relocation/global addresses only; the `0x26`-byte control structure and compare-to-boolean mapping are equivalent in AS3000 2005, NEO 2005 and NEO 2013.

## Refutation pass

* A24C as a raw `strcmp`-style return — **REFUTED**; firmware explicitly converts zero/nonzero comparison into byte `1/0`.
* A24C as a file-password comparison — **REFUTED for the direct operand**; it compares against the shared master-password global, not an active-file password field.
* A24C accepting NULL as false — **REFUTED**; no guard exists before the helper dereferences the candidate string.
* A24C mutating password state — **REFUTED**; the handler is read-only apart from stack temporaries and return register state.
* A24C having more than one public argument — **REFUTED**; the additional zero values are handler-supplied helper arguments, not caller ABI slots.

## Classification

* **CONFIRMED:** `uint8_t SYS_A24C(const char *password)`; one caller-supplied pointer; master-password comparison; equality-to-1 / inequality-to-0 mapping; no state mutation; `0x26`-byte handler; one direct caller per ROM; NULL has no guard.
* **INFERENCE STRONG:** the compared global is the system master-password string; this is supported by the surrounding password workflows and reuse of the same global/helper.
* **UNKNOWN:** original vendor-facing function name, if one existed outside the unrecovered private interface.

## Regression

Static source-first regression: **EXECUTED 39/39 PASS** (13 checks per ROM), covering canonical hash, entry/file mapping, exact `0x26` boundary, candidate-pointer load, master-global load, two handler-supplied zero arguments, helper target, stack cleanup, equality test, `1/0` return mapping, direct caller count, caller one-argument cleanup, and caller byte-test consumption.

Dynamic regression: **SPECIFIED / NOT EXECUTED**. Emulator/sandbox cases should test equal, unequal, empty, prefix, and extended-byte candidate strings. NULL should not be used as a normal functional test because firmware provides no safe NULL contract.
