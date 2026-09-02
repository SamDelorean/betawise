# A0AC / index 43 — SetModifierKeys closure

## Status

**MECÁNICA_CERRADA A / REVALIDADO_SOURCE_FIRST / 41/41 PASS / COMPLETADO**

Raw syscall contract:

```c
void SetModifierKeys(uint16_t modifiers);
```

`syscall.c` names index 43 `SetModifierKeys`. The reconstructed `os3k.h`/`os3k.pdf` spelling `SetKeyModifiers(uint16_t mask)` is retained only as historical/source-compatibility spelling; it does not describe a different firmware operation.

## Primary firmware evidence

| Generation | Handler | File offset | Length | modifier_state |
|---|---:|---:|---:|---:|
| AS3000 2005 | `0x004CDF66` | `0x00DF66` | `0x0A` | `0x0000432C` |
| NEO 2005 | `0x005D1F4A` | `0x011F4A` | `0x0A` | `0x000057B6` |
| NEO 2013 | `0x00422F3A` | `0x012F3A` | `0x0A` | `0x00005D0E` |

All three handlers are exactly two 68000 instructions:

```text
MOVE.W 6(SP),modifier_state
RTS
```

The raw handler SHA-256 values are:

- AS3000: `143544727efebd5a0b847d4f564a1815ad23bd39aa4aa6eb8d5914d866e1f55e`
- NEO 2005: `6f193c512d3339a6577d21c1664ccad37e02a19dc6773d9960886d03a7037fc5`
- NEO 2013: `24d1fcf97cd3de3267fe0170595d1b98264fdd92a98630b175f3b03a75f54400`

After neutralizing only the relocated RAM address, the normalized handler is identical in all three ROMs: `5acf415afa102c1a3ed576668a088bfdeb1a2ec0a91f6189180268043485881f`.

## ABI and semantics

On the big-endian 68k ABI used here, the first argument occupies a 32-bit stack slot after the return address and its low word is at `6(SP)`. A0AC consumes only those 16 bits and stores them directly into the modifier-state word.

The operation is a **replacement**, not a mask operation over the previous value. The handler does not read the old state and performs no OR, AND, XOR, shift, filtering, validation, or normalization. Therefore the historical parameter name `mask` should be read as “bitmask value to store,” not “mask to apply.”

A0AC does not construct an API return value and does not directly mutate the key queue.

## Relationship to neighboring keyboard syscalls

A0AC writes exactly the same 16-bit state read by A0A0 / `GetModifierKeys`. A0A4 / `ScanKeyboard` consumes that state when constructing `KeyMod_e` events before sending them to A0A8 / `QueueKey`.

This closes the raw setter/getter pair:

```c
KeyMod_e GetModifierKeys(void);   /* A0A0: returns the complete word in D0.W */
void SetModifierKeys(uint16_t modifiers); /* A0AC: replaces that word */
```

## Official callers

A structural scan of the four re-extracted official applets used for the neighboring keyboard closures found four A0AC callers:

- AlphaWordPlus 2005: 2
- AlphaWordPlus NEO 2012: 2
- KeyWords 2005: 0
- KeyWords NEO 2012: 0

All four positive callers execute `CLR.L -(SP)` immediately before calling the A0AC import veneer, so they pass zero and clear the complete modifier state. These callers independently corroborate the whole-state clearing use case. Nonzero behavior is established by the unconditional firmware `MOVE.W` and the paired getter/consumer of the same word.

## Refutation results

- **Refuted:** A0AC applies a mask to the previous modifier state. It replaces the word.
- **Refuted:** only one byte is written. The handler stores a complete word.
- **Refuted:** the value is shifted as suggested by the conflicting historical getter comment. No shift exists.
- **Refuted:** A0AC returns a status/state value. The raw contract is `void`.
- **Refuted:** AS3000 and NEO differ semantically here. The only observed handler difference is RAM relocation.

## Regression

Static/contractual regression: **41/41 PASS**.

Coverage includes the three canonical ROM hashes, A0AC vectors, exact handler hashes, the paired A0A0 global, normalized three-ROM identity, four official applet hashes, four positive structural xrefs with zero arguments, two negative applet results, and adversarial replacement/16-bit behavior checks.

- regression source SHA-256: `85776f44a30498530acc458163781f7ba56ba4550b6a3d6d7f65204635887f41`
- regression output SHA-256: `5500502a76083cad86990d0d7959a54e96e9d304b4da9cbcfa7b89517068ab38`

A dynamic 68k probe has **not** been executed; it is additional validation rather than a closure blocker.

## Disposition

Document A0AC as the raw `SetModifierKeys` syscall. `SetKeyModifiers` may remain as an explicitly documented compatibility alias, but should not obscure the native name or imply incremental mask semantics. The next ascending ABI frontier is A0B0 / index 44 (`IsKeyDownNow`).