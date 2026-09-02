# A0A8 / index 42 — QueueKey closure

## Status

**MECÁNICA_CERRADA A / REVALIDADO_SOURCE_FIRST / 41/41 PASS / COMPLETADO**

```c
void QueueKey(KeyMod_e key);
```

The historical name and signature are confirmed directly by all three canonical ROMs.

## Canonical handler

All three implementations are exactly `0x32` bytes / 16 decoded 68000 instructions / zero unknown opcodes. After neutralizing only the relocated queue-state addresses, the handlers are byte-identical.

Normalized SHA-256:

`be7649ce67e39e84b39471e24eea7e5ab47f3dd6bb390f7c20fd212946552f8b`

The queue state is the same state independently used by A094 / `GetKey` and A09C / `IsKeyReady`.

## Mechanics

The handler implements a 14-entry circular queue of 16-bit `KeyMod_e` values:

1. Read `queue_count` as a word.
2. If `queue_count >= 14`, return immediately without changing the queue, count, or write index.
3. Otherwise increment `queue_count`.
4. Read `write_index`.
5. Store the low word of the argument at `queue_base + 2 * write_index`.
6. Increment `write_index` and wrap it to zero at 14.
7. Return.

Thus a full queue **silently drops the new event**. It does not overwrite the oldest queued event.

The stored value is a word, not a byte, so modifier flags and the `KEY_MOD_KEY_UP` bit survive intact.

## Queue relationship

A094 / `GetKey` is the corresponding consumer: it reads from a separate read index, decrements the same count, and wraps its read index at 14. Together the producer and consumer establish a 14-word FIFO circular queue.

A0A4 / `ScanKeyboard` is a confirmed internal caller in all three ROMs. It constructs a `KeyMod_e` from a translated matrix key plus the current modifier state and calls A0A8 to enqueue that event.

## ABI and return

A0A8 consumes one `KeyMod_e` word argument. The historical `void` return is correct.

There is no portable status result: on the full-queue branch `D0` is not initialized by the handler, while on the insertion path it contains calculation residue. Callers must not interpret `D0` as success/failure.

## Official callers

A re-extracted official-app sample finds direct A0A8 import-veneer callers in KeyWords:

- KeyWords 2005: 2 executable xrefs.
- KeyWords NEO 2012: 2 executable xrefs.

AlphaWordPlus 2005 and NEO 2012 have no structural A0A8 xrefs in the same sample. The direct firmware caller from A0A4 independently confirms normal producer use.

## Refutation results

- **Refuted:** QueueKey grows beyond 14 entries.
- **Refuted:** a full queue overwrites the oldest event; it drops the new event instead.
- **Refuted:** only the key byte is stored; the full 16-bit `KeyMod_e` word is written.
- **Refuted:** QueueKey returns a boolean/status value; the raw ABI is `void`.

## Regression

Static/model regression: **41/41 PASS**.

Coverage includes the three canonical ROM hashes and vectors, complete handler mechanics, capacity and modulo-14 behavior, relocation-normalized identity, official KeyWords callers, and a synthetic fill/wrap/full-drop model.

- regression source SHA-256: `293b5cc9655fa428529c3ebf03929b4e046025512f6d633abaac3cbcf8cdbe33`
- regression output SHA-256: `19e888a611a7eec92008c583fd4df5ec8b80a6e5ef76998b43ff0340f5fc0edc`

A dynamic 68k probe has **not** been executed; it is additional validation and not a closure blocker.

## Disposition

Document A0A8 as `void QueueKey(KeyMod_e key)`, preserving the 14-entry capacity and silent-new-event-drop behavior as part of the observed firmware contract.
