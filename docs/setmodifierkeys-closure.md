# A0AC / index 43 — SetModifierKeys closure

## Status

**MECÁNICA_CERRADA A / REVALIDADO_SOURCE_FIRST / 66/66 PASS / COMPLETADO**

Raw syscall contract:

```c
void SetModifierKeys(uint16_t modifiers);
```

`syscall.c` names index 43 `SetModifierKeys`. The reconstructed
`os3k.h` and historical `os3k.pdf` spell the API
`SetKeyModifiers(uint16_t mask)`; that spelling is retained only as a
source-compatibility alias and does not describe a different firmware
operation.

## Primary firmware evidence

| Generation | Handler | File offset | Length | `modifier_state` |
|---|---:|---:|---:|---:|
| AS3000 2005 | `0x004CDF66` | `0x00DF66` | `0x0A` | `0x0000432C` |
| NEO 2005 | `0x005D1F4A` | `0x011F4A` | `0x0A` | `0x000057B6` |
| NEO 2013 | `0x00422F3A` | `0x012F3A` | `0x0A` | `0x00005D0E` |

All three handlers are exactly two 68000 instructions:

```text
MOVE.W 6(SP),modifier_state
RTS
```

Raw handler SHA-256:

- AS3000: `143544727efebd5a0b847d4f564a1815ad23bd39aa4aa6eb8d5914d866e1f55e`
- NEO 2005: `6f193c512d3339a6577d21c1664ccad37e02a19dc6773d9960886d03a7037fc5`
- NEO 2013: `24d1fcf97cd3de3267fe0170595d1b98264fdd92a98630b175f3b03a75f54400`

After neutralizing only the relocated RAM address, the normalized body is
`33ef0006000000004e75` in all three ROMs, SHA-256
`5acf415afa102c1a3ed576668a088bfdeb1a2ec0a91f6189180268043485881f`.

## ABI and semantics

The first C argument occupies a 32-bit stack slot. On this big-endian 68k ABI
its low word is at `6(SP)`; A0AC consumes exactly those 16 bits and stores
them directly into `modifier_state`.

The operation is whole-word **replacement**, not an operation applied to the
previous value. The handler does not read the old state and performs no OR,
AND, XOR, shift, filtering, validation or normalization. Unknown/reserved bits
are stored as supplied.

A0AC does not construct a return value. The handler itself leaves D0 untouched,
but that residual register content is not an API result. It does not scan the
keyboard, construct a `KeyMod_e` event or mutate the key queue.

## Relationship to neighboring keyboard syscalls

A0AC writes exactly the 16-bit word read by A0A0 / `GetModifierKeys`.
A0A4 / `ScanKeyboard` subsequently updates and consumes this state when it
constructs events for A0A8 / `QueueKey`; therefore a value installed through
A0AC is shared state, not a permanent override immune to later scans.

```c
KeyMod_e GetModifierKeys(void);            /* A0A0 */
void SetModifierKeys(uint16_t modifiers);  /* A0AC */
```

## Official caller evidence

The final structural scan covered **40 official `.os3kapp` files** from the
AS3000 2005 and NEO 2012 distributions. Twenty-nine contain the relevant
import-table sequence. It found **12 executable A0AC xrefs in six applets**:

| Applet | Generation | Xrefs | Argument construction |
|---|---|---:|---|
| AlphaWordPlus | 2005 | 2 | constant zero |
| ControlPanel | 2005 | 2 | constant zero |
| AlphaWordPlus | NEO 2012 | 2 | constant zero |
| ControlPanel | NEO 2012 | 2 | constant zero |
| Responder | NEO 2012 | 2 | dynamic word; toggles bit 9 |
| WirelessFileTransfer | NEO 2012 | 2 | dynamic word; toggles bit 9 |

The eight AlphaWordPlus/ControlPanel calls execute `CLR.L -(SP)` and clear the
complete modifier word.

The four Responder/WirelessFileTransfer calls construct a full word with the
equivalent sequence:

```text
MOVEQ   #0,D1
MOVE.W  D0,D1
BCHG    #9,D1
MOVE.L  D1,D0
MOVE.L  D0,-(SP)
JSR     A0AC
```

Bit 9 is `KEY_MOD_CAPS_LOCK` (`0x0200`). These callers independently prove
that A0AC is used with nonzero, dynamically updated modifier words and not only
as a clear-state helper. All 12 callers discard D0 and clean the argument slot.

The earlier 41/41 checkpoint covered only AlphaWordPlus and KeyWords. Its
four-positive-caller result remains correct for that subset, but its statement
that all observed callers pass zero is superseded by this complete corpus.

## Refutation results

- **Refuted:** A0AC applies a mask to the previous state. It replaces the word.
- **Refuted:** only one byte is written. The handler stores a complete word.
- **Refuted:** the value is shifted like the conflicting historical getter
  comment. No shift exists.
- **Refuted:** A0AC is only a clear-state helper. Four official callers pass a
  dynamic word with `KEY_MOD_CAPS_LOCK` toggled.
- **Refuted:** A0AC returns a status or state value. The raw contract is
  `void`.
- **Refuted:** AS3000 and NEO differ semantically. Only the RAM address is
  relocated.

## Regression

Static/contractual regression: **66/66 PASS**.

Coverage includes the three canonical ROM hashes and sizes, A0A0/A0AC vectors,
exact handler bytes and hashes, shared getter/setter global, normalized 3/3
identity, absence of internal absolute JSRs, all 40 official applets, 29 import
tables, all 12 executable xrefs, per-applet counts, zero/dynamic argument
shapes, stack cleanup and non-consumption of D0.

- regression source SHA-256:
  `e43ce0982b3cf8c2c513cc89d0c2b80c58ab8eb1a8f51e62f4ecf41e3ddf6dbb`
- regression output SHA-256:
  `2565ebd31beb2108d6ed24a8296ba5971edae117e56f010b244436c5b691a825`

This is an executed static bytes/flow/contract regression, not a dynamic 68k
probe. A dynamic probe is additional validation and not a closure blocker.

## Disposition

Document A0AC as the raw `SetModifierKeys` syscall. `SetKeyModifiers` may
remain as an explicitly documented compatibility alias, but must not imply
incremental mask semantics. The next ascending ABI frontier is A0B0 / index 44
(`IsKeyDownNow`).
