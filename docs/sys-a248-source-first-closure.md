# A248 / SYS_A248 — source-first closure

Status: **CERRADO A / SOURCE-FIRST**  
Date: 2026-09-03 10:20 America/Mexico_City

## Contract

```c
uint8_t SYS_A248(void);
```

A248 returns the low byte of the global file-password-protection state. Only `D0.B` is contractual; the upper 24 bits are not normalized by the handler.

## Source-first correlation

Historical/public SDK material does not provide a reliable vendor name for this trap, so the neutral name `SYS_A248` is retained. Prior project material associated it with file-password protection and paired it with A258. That interpretation was treated as an anchor only and revalidated directly against all three canonical ROMs.

## Firmware-confirmed behavior

The handler is exactly 8 bytes in AS3000 2005, NEO 2005 and NEO 2013:

* it executes a byte load from one global into `D0.B`;
* it returns immediately;
* it consumes no arguments;
* it calls no helper and writes no state.

The immediately following A258 handler writes the first byte-sized argument to the **same global**, confirming the getter/setter pairing independently of the retrospective notes.

Two direct absolute callers exist in each canonical ROM. One compares `D0.B` explicitly with `1`; the other performs a zero/non-zero test. This is consistent with the normal 0/1 protection-state convention while also confirming that A248 itself performs no boolean normalization.

## Canonical primary evidence

Canonical ROM SHA-256 values were reverified:

* AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
* NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
* NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Entries and global addresses:

| ROM | A248 entry | state global |
|---|---:|---:|
| AS3000 2005 | `0x004DFF5A` | `0x00004472` |
| NEO 2005 | `0x005E28B6` | `0x000058A4` |
| NEO 2013 | `0x00438CB4` | `0x000118A2` |

Handler SHA-256 (`0x08` bytes):

* AS3000 2005: `dadb938d84d6b5526e1a6344b71048f3665ab708766c08bdceabeee44c96e621`
* NEO 2005: `b0a659e05afa3b79730cdc5b48c3d91b017cc83f50a1bef9d2140f72bb06ccd9`
* NEO 2013: `99d474d0a80242def6dfc61536052423b282e35816710d09ead984b8e9c17118`

Direct absolute `JSR` xrefs are `2 / 2 / 2` for AS3000 2005 / NEO 2005 / NEO 2013.

## Classification

* **CONFIRMED:** `uint8_t SYS_A248(void)` getter mechanics, no arguments, read-only behavior, exact global pairing with A258, two direct callers per ROM, callers using compare-to-1 and zero/non-zero tests.
* **INFERENCE STRONG:** the global is specifically the file-password-protection enable state. This is supported by the A258 pair plus the surrounding password subsystem and historical project correlation, but no public vendor symbol was recovered.
* **UNKNOWN:** original vendor function name, if any.

## Refutation pass

* A248 as a normalized boolean function — **REFUTED**; it returns the stored byte literally.
* A248 as a setter or mutating control — **REFUTED**; handler is read-only.
* A248 requiring an argument — **REFUTED**; no stack argument is consumed.
* A248 as an isolated unrelated byte — **REFUTED** by the immediately adjacent A258 setter targeting the same global and by consistent callers.

## Regression

Static source-first regression: **EXECUTED 30/30 PASS** (10 checks per ROM), covering canonical ROM hash, exact 8-byte boundary, byte-load opcode, global address, RTS, immediate A258 setter pairing, direct-xref count/locations, compare-to-1 caller, and zero-test caller.

Dynamic regression: **SPECIFIED / NOT EXECUTED**. Emulator/sandbox cases should write A258 values `0`, `1`, and a non-boolean byte such as `0x7F`, then verify that A248 returns each literal low byte and produces no additional side effects.
