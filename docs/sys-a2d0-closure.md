# OS3K A2D0 closure

Status: **MECÁNICA_CERRADA A / PUBLICADO**. Dynamic regression is specified but not yet executed.

## Neutral contract

```c
uint32_t SYS_A2D0(const void *state,
                  uint32_t limit,
                  uint8_t arg3,
                  uint8_t arg4,
                  uint8_t arg5);
```

Five consumed physical 32-bit stack slots are confirmed. `state` is mandatory and is copied as eleven longwords (44 bytes) into a local snapshot. `limit` is consumed as a full longword. Only the low byte of slots 3 through 5 is used. Names for those byte arguments are intentionally neutral.

## Canonical handlers

| ROM | runtime | file offset | size | raw SHA-256 |
| --- | --- | --- | --- | --- |
| AS3000 2005 | `0x004D5A00` | `0x015A00` | `0x9C` | `b24e0558feccccdb7179eb339b646ae75a545dd98349478710ae50d7b3c15f1f` |
| NEO 2005 | `0x005D7CE6` | `0x017CE6` | `0x9C` | `27993af48ff75b36b12a94c0cf6aebdd2153b0f63fbdfe218a9e8d9d4840a9d8` |
| NEO 2013 | `0x0042971E` | `0x01971E` | `0x9C` | `142ea212c8e67be751efbdb049fdb5a37a7e88ac96db0ece0d3a65fcc64fe60f` |

Each body has a single terminal `RTS` at `+0x9A`; all conditional branch targets remain inside the body. The next vector, A2D4, is exactly `+0x9C`, but adjacency is corroboration rather than the sole boundary proof.

The only cross-ROM handler differences are the `BSR.W` displacement to A2CC and the absolute target of one internal helper. Neutralizing those relocation operands makes all three handlers byte-identical, SHA-256 `3e372832ccdef5c5430c79d4feb92e04be821ac50a8daa472d56ffba96500ead`.

## Operation and A2CC call

A2D0 copies the first 44 bytes of `state` into a local snapshot. It then calls A2CC with the mechanically reconstructed argument vector:

```text
[snapshot, limit, NULL, 0, arg3, arg4, arg5, 0, 0, NULL]
```

A2CC's D0 return is deliberately ignored. A2D0 therefore uses A2CC for transformation/validation of the local snapshot rather than forwarding A2CC's status to its own caller.

Before the normal path, A2D0 loads the pointer stored at `state+0x00` and tests the word at pointee offset `+0x4A`. A non-zero value returns the literal 32-bit value `0x02000005` immediately.

## Main helper and return proof

The normal path calls an eight-argument helper:

- AS3000: `0x004D9184`, size `0x1DC`
- NEO 2005: `0x005DB850`, size `0x1E8`
- NEO 2013: `0x0042DB56`, size `0x1E8`

Its argument shape is pointer, longword, byte, longword, byte, byte, byte, `uint32_t *out_result`. A2D0 supplies original nested state, original fields, corresponding fields from the transformed snapshot, byte arguments 4/5, and a local result pointer.

The helper clears `*out_result` on entry. At its terminal path it calls a secondary helper, writes that helper's full D0.L into `*out_result`, and then returns its own value `1`. A2D0 does **not** return that `1`; after stack cleanup it loads the local result longword.

The secondary helper is:

- AS3000 `0x004D6FF4` through `0x004D781E`
- NEO 2005 `0x005D946E` through `0x005D9D22`
- NEO 2013 `0x0042B5D0` through `0x0042BECC`

In all three generations its unique terminal path executes `MOVEQ #0,D0` immediately before the epilogue. Thus A2D0's normal path returns exactly zero.

**Closed D0.L domain:** `{ 0, 0x02000005 }`.

This disproves `void`, byte-sized, and boolean return models. No vendor status name is assigned to `0x02000005`.

## Callers and negative search

No direct absolute JSR or BSR.W caller of A2D0 was found in any of the three canonical ROMs. Official SmartApplet binary searches found the expected sequential syscall-stub-table occurrence. Additional byte matches, notably in SpellCheck and newer NEO applets, resolve to embedded immediate/data bytes rather than a confirmed A2D0 instruction at an executable boundary. No independent A2D0/SYS_A2D0 prototype was recovered from the checked BetaWise/GitHub sources.

The five-argument contract therefore rests on the handler's exact stack reads: slots 1 and 2 are full-width and slots 3–5 are low-byte-only; no later external slot is read. The analysis does not invent unused trailing arguments merely because C calling conventions would permit them.

## Generational comparison

The external A2D0 handler is mechanically identical across AS3000 and NEO modulo relocation. The main NEO helper has an internal 12-byte extension and a larger local frame; the secondary helper also grows across generations. Those internal changes preserve the eight-argument output-pointer pattern and the externally relevant normal result of zero. The external five-slot ABI and `{0, 0x02000005}` result domain are common to all three ROMs.

The `const` qualifier on `state` describes direct accesses by A2D0 only. The helper mutates an object reached through the pointer stored at `state+0x00`; transitive side effects therefore remain real.

## Adversarial checks

The analysis rejected several tempting but incorrect interpretations: using A2D4 adjacency alone as a function-boundary proof; forwarding A2CC's D0 as A2D0's result; confusing the main helper's explicit `1` return with A2D0's result; narrowing the result to bool; and treating the top-level read-only context as evidence of no transitive mutation.

## Regression

Emulator-first regression is **specified, not executed**. It should test the `+0x4A` guard error path, the normal zero result, invariance to the upper 24 bits of physical slots 3–5, snapshot-versus-original behavior around the A2CC call, transitive helper side effects, and equivalent external results on AS3000/NEO 2005/NEO 2013.

Private ROM bytes, full disassemblies and helper workpapers remain in Drive.
