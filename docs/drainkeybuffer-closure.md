# A098 / index 38 — `DrainKeyBuffer`

Status: source-first closure, revalidated against the three canonical OS3K ROM generations.

## Public contract

```c
void DrainKeyBuffer(void);
```

The historical SDK identity and prototype agree with the firmware implementation.

## Behavior

`DrainKeyBuffer` does not clear queue metadata directly. Its logic is equivalent to:

```c
while (IsKeyReady()) {
    (void)GetKey(true);
}
```

It therefore drains the keyboard queue by consuming entries one by one until the first readiness check reports empty.

Because it calls `GetKey(true)`, every drained key follows the normal special-key processing path used by A094, including the `SYS_A25C(8, key)` side effect and the A094 last-key update. The routine is therefore not equivalent to simply zeroing a queue count.

The operation is not an atomic exclusion barrier: it stops when one `IsKeyReady()` observation is false. A key arriving after that observation can remain for a subsequent call.

## Multi-entry physical block

A098 shares a compact physical region with the following ABI entry point. The logical A098 handler ends exactly at A09C / `IsKeyReady`:

- AlphaSmart 3000 2005: A098 `0x004CE070`, A09C `0x004CE082`
- NEO 2005: A098 `0x005D2054`, A09C `0x005D2066`
- NEO 2013: A098 `0x00423044`, A09C `0x00423056`

Thus the logical A098 handler is **0x12 bytes**, not 0x24 bytes. Its complete 18-byte implementation is identical in all three generations and uses only relative branches/calls.

SHA-256 of the logical handler, identical 3/3:

`315e4e2239f133d9a07ad5bb186c68fe426bbd7d24b9bce75ac9a9a138da4c32`

The relative call targets resolve exactly to A09C / `IsKeyReady` and A094 / `GetKey` in every canonical ROM.

## Caller evidence

A088 / `WaitForKey`, already independently revalidated, calls A098 immediately on entry. This discards any keyboard events already queued before A088 begins waiting for a new event, matching the reconstructed A098 semantics.

## ABI and return

A098 has no public arguments. The SDK contract is `void`. On the observed exit path the data register still contains the residual result from the final `IsKeyReady()` call that caused loop termination, but that incidental value is not part of the public ABI.

## Refutation and regression

Firmware evidence refutes several tempting interpretations:

- A098 does not directly set queue count/indexes to zero.
- Drained keys are not discarded without processing; A094 is called with `process_special_keys = true`.
- The logical handler is not the full 0x24-byte physical region; A09C has its own vector at +0x12.
- The routine does not wait for future input; when readiness is false it returns immediately.
- A residual zero in the return register is not a boolean return contract; the API is `void`.

A private static regression executed on all three canonical ROMs plus a control-flow model completed **52/52 PASS**. It validates handler boundaries, identical bytes, relative targets, the fixed `true` argument to A094, the readiness loop, and drain behavior for queues of 0, 1, 3, and 14 entries. This is not a dynamic 68k CPU probe.

## Confidence

- Identity `DrainKeyBuffer`: **CONFIRMED**
- `void` / no-argument ABI: **CONFIRMED**
- Logical handler boundary 0x12: **CONFIRMED**
- Loop `IsKeyReady` → `GetKey(true)`: **CONFIRMED**
- Special-key and last-key side effects inherited through A094: **CONFIRMED by direct call structure and the separately closed A094 contract**
- Atomicity against concurrently arriving input: **NOT GUARANTEED**
- Dynamic 68k execution: **NOT EXECUTED**
