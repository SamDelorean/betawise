# A088 / index34 — `WaitForKey`

Status: **mechanically closed / source-first revalidated**.

Public SDK identity and signature remain valid:

```c
KeyMod_e WaitForKey(void);
```

## Firmware-confirmed behavior

The canonical AS3000 2005, NEO 2005, and NEO 2013 handlers have the same control-flow skeleton after relocation-only call operands are normalized.

`WaitForKey` is stronger than a simple `while (!IsKeyReady())` wrapper:

1. It drains the existing keyboard queue before waiting. A key already queued before the call is therefore discarded rather than returned.
2. It checks for a ready key.
3. While no key is ready, it services an internal event/status path, performs keyboard scanning, and checks readiness again.
4. When a key is available, it obtains it with `process_special_keys = true`.
5. It returns the resulting 16-bit `KeyMod_e` value. The ordinary path observed from the underlying key getter leaves the high word of the return register zero.

The two internal service helpers used during the wait loop are deliberately left unnamed: their local role is mechanically established, but no vendor symbol was recovered.

## Caller evidence

Two official AlphaWordPlus generations provide extensive executable caller evidence:

- AlphaWordPlus 2005: 41 structural calls to `WaitForKey`.
- AlphaWordPlus NEO 2012: 45 structural calls.

Concrete callers consume the returned key value (commonly its low key-code byte) and compare it with values such as Enter, Escape, and Tab. Other callers intentionally ignore the return value. This refutes a `void` interpretation.

## Regression

Static regression was executed against the three canonical firmware generations and the two official AlphaWordPlus binaries: **93/93 checks passed**. Checks cover canonical-image identity, handler bounds/control flow, call-target sequence, cross-generation normalized equivalence, the fixed `process_special_keys=true` argument, and the caller counts above.

A dynamic hardware/emulator probe remains useful as an additional validation, but is not required for this static ABI closure.

## Compatibility note

Code that expects `WaitForKey()` to return a key already present in the OS3K queue is not firmware-compatible: the routine drains the queue first and waits for a subsequent key event.
