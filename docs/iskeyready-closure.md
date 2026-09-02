# A09C / index 39 — `IsKeyReady`

Status: source-first closure, revalidated against the three canonical OS3K ROM generations.

## Public contract

```c
bool IsKeyReady(void);
```

The historical SDK identity and prototype agree with the firmware implementation.

## Behavior

A09C is a queue-state predicate. It inspects only the shared 16-bit keyboard queue count used by A094 / `GetKey` and A0A8 / `QueueKey`.

Under the independently confirmed queue invariant (`count` in the range 0..14):

- `count == 0` → boolean false
- `count != 0` → boolean true

The routine does not scan the keyboard hardware, consume an entry, change queue indexes, or modify the queue count.

## ABI precision

The boolean result is produced in **D0.b**. The handler does not clear or normalize the upper 24 bits of D0, so callers must not infer a normalized 32-bit `0` or `1` value from the full register.

This matches the already revalidated firmware callers A098 / `DrainKeyBuffer` and A088 / `WaitForKey`, which test only the low byte of the returned value.

## Cross-generation equivalence

The logical A09C handler is 0x12 bytes at:

- AlphaSmart 3000 2005: `0x004CE082`
- NEO 2005: `0x005D2066`
- NEO 2013: `0x00423056`

The only cross-generation difference is the relocated absolute address of the shared queue-count word. After neutralizing that single operand, all three handlers are byte-identical.

Normalized SHA-256:

`ce89ee23c182e474effddc3d049144f9ae8404d6701fe8c7c9483e26c8056cad`

## Refutation and regression

Firmware evidence refutes several plausible alternatives:

- A09C does not return the queue count itself; it produces a boolean in D0.b.
- The full D0.L register is not guaranteed to be normalized.
- A09C does not scan keyboard hardware.
- A09C does not consume or mutate queued input.

A private static contractual regression executed against all three canonical ROM images plus the valid count range completed **45/45 PASS**. This is byte/flow/model validation; a dynamic 68k CPU probe was not executed and is not claimed.

## Confidence

- Identity `IsKeyReady`: **CONFIRMED**
- `bool IsKeyReady(void)` ABI: **CONFIRMED**
- Queue-count-only predicate semantics: **CONFIRMED**
- Boolean result in D0.b: **CONFIRMED**
- Upper 24 bits of D0 not part of the contract: **CONFIRMED**
- No queue mutation / no hardware scan: **CONFIRMED**
- Dynamic 68k execution: **NOT EXECUTED**
