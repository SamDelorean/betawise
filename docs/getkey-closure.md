# A094 / index 37 — `GetKey`

Status: source-first closure, revalidated against the three canonical OS3K ROM generations.

## Public contract

```c
KeyMod_e GetKey(bool process_special_keys);
```

The historical SDK name and prototype agree with the firmware implementation. A094 is the keyboard-queue dequeue primitive; it is **not** a blocking wait routine.

## Behavior

- The keyboard queue contains 14 entries of 16-bit `KeyMod_e` values.
- If the queue is empty, `GetKey` returns `KEY_NONE` (`0x00FF`) immediately.
- Otherwise it removes exactly one word from the circular queue, decrements the queue count, advances the read index, and wraps the read index at 14.
- The dequeued 16-bit key is zero-extended into the return register and is also copied to the OS last-key state.
- Only the low byte of the 32-bit `process_special_keys` argument slot is tested.
- If that byte is nonzero, the dequeued key is passed to the internal special-key processor through `SYS_A25C(8, key)`. The helper's return value is discarded; the original dequeued `KeyMod_e` remains the value returned by `GetKey`.

Thus `process_special_keys` controls processing side effects; it does not translate or replace the key returned by A094.

## Queue corroboration

The independent producer A0A8 / `QueueKey` uses the same queue count and storage, rejects an enqueue when the count is already at least 14, stores a 16-bit key, advances the write index, and wraps it at 14. This independently confirms the queue layout and capacity used by `GetKey`.

## Caller evidence

Two already revalidated OS3K routines exercise both parameter states:

- A088 / `WaitForKey` calls A094 with `process_special_keys = true` after it has waited for keyboard availability.
- A084 / `TextBox` normally calls A094 with `true`, but its `0xFD` exit-key marker switches the call to `false`.

This establishes that the boolean is operational rather than decorative and also separates the blocking behavior of `WaitForKey` from the nonblocking dequeue behavior of `GetKey`.

## Cross-generation equivalence

The A094 handlers are 0x60 bytes at:

- AlphaSmart 3000 2005: `0x004CE094`
- NEO 2005: `0x005D2078`
- NEO 2013: `0x00423068`

The only cross-ROM differences are relocated absolute operands for queue state, the A25C call target, and last-key state. Normalizing those five operands makes all three handlers byte-identical.

Normalized SHA-256:

`e0e92c989c16df0b196fbe0a5cb26b0853faa7d1615911e3f624d5c35319720b`

## Refutation and regression

Firmware evidence refutes several plausible but incorrect interpretations:

- A094 is not blocking; an empty queue returns `0x00FF` immediately.
- `process_special_keys` does not transform the returned key.
- The queue capacity is 14, corroborated by both consumer and producer.
- Empty-queue return is explicit, not indeterminate.
- Only the low byte of the boolean argument slot is consumed.
- The return is a 16-bit `KeyMod_e`, not merely an 8-bit key code.

A private static contractual regression executed directly against the three canonical ROM images plus a behavior model completed **101/101 PASS**. This is a byte/flow/model regression; a dynamic 68k CPU probe was not executed and is not claimed.

## Confidence

- Identity `GetKey`: **CONFIRMED**
- Historical prototype: **CONFIRMED**
- Nonblocking queue-dequeue semantics: **CONFIRMED**
- Empty result `KEY_NONE = 0x00FF`: **CONFIRMED**
- 14-entry, 16-bit circular queue: **CONFIRMED**
- Low-byte-only boolean consumption: **CONFIRMED**
- Conditional `SYS_A25C(8, key)` call: **CONFIRMED**
- Interpretation of A25C action-mask bit 3 as special-key handling: **CONFIRMED by correlation with the separately reconstructed A25C contract**
- Dynamic 68k execution: **NOT EXECUTED**
