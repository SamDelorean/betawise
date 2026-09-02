# A0B0 / index 44 — IsKeyDownNow closure

## Status

**MECÁNICA_CERRADA A / REVALIDADO_SOURCE_FIRST / 70/70 PASS / COMPLETADO**

```c
bool IsKeyDownNow(void);
```

The historical name and no-argument boolean contract are confirmed by direct firmware mechanics in all three canonical ROMs.

## Source-first semantic anchor

The project EhBASIC source uses `IsKeyDownNow` in its input vector before `ScanKeyboard` and `IsKeyReady`, with the explicit intent “check if a key is pressed.” It tests `D0.b`; only when a key is physically down does it force a keyboard scan. `IsKeyReady` is then queried separately for queued input.

That source establishes a useful hypothesis: A0B0 is a physical instantaneous predicate, not a FIFO-ready predicate. The ROM mechanics independently confirm it.

## Primary firmware evidence

| Generation | Handler | File offset | Length | Instructions |
|---|---:|---:|---:|---:|
| AS3000 2005 | `0x004CE568` | `0x00E568` | `0x24` | 12 |
| NEO 2005 | `0x005D25D2` | `0x0125D2` | `0x68` | 34 |
| NEO 2013 | `0x004235C2` | `0x0135C2` | `0x68` | 34 |

All three windows disassemble reproducibly with zero unknown opcodes and terminate in `RTS`.

Raw handler SHA-256:

- AS3000: `8ef60f16234828abe23f2fb668714fd4ec5d242ce06121a33a0c805de0838ef9`
- NEO 2005: `d0edfb57e00cb45996e3e1f47277c6009d2992f5f1914b7735ee77fea59bbfff`
- NEO 2013: `d0edfb57e00cb45996e3e1f47277c6009d2992f5f1914b7735ee77fea59bbfff`

NEO 2005 and NEO 2013 are byte-identical for A0B0. AS3000 implements the same predicate through a different, shorter hardware setup path.

## Core predicate

Every implementation configures platform keyboard-detection hardware and reads the keyboard input byte at `0xFFFFF419`. The byte is active-low, so the handler inverts it and then reduces the result to a boolean:

- inverted byte == 0 → `D0.b = 0`
- inverted byte != 0 → `D0.b = 1`

Thus A0B0 reports whether **any physical key is currently down**. It does not identify which key is pressed and does not return the physical matrix bitmap.

Only `D0.b` is contractual; the upper 24 bits of `D0` are not normalized.

## AS3000 versus NEO

### AS3000

AS3000 performs platform-specific setup through `0x00600000` and `0xFFFFF401`, reads `0xFFFFF419` once, inverts the byte, normalizes it to 0/1, and returns.

### NEO 2005 / NEO 2013

NEO configures `0xFFFFF410`, `0xFFFFF411`, `0xFFFFF440`, `0xFFFFF441`, `0xFFFFF408`, and `0xFFFFF409` before sampling `0xFFFFF419`.

It then adds temporal stabilization to the boolean predicate. The current normalized 0/1 result is compared with the previous observation. On a change, the stable counter is reset to zero; on equality, the counter is incremented. Sampling repeats until that counter reaches 5.

This is a real implementation difference from AS3000 and should not be flattened away.

## Separation from the keyboard FIFO

A0B0 does not read the FIFO count, read index, write index, or queue base established by A094 / `GetKey`, A09C / `IsKeyReady`, and A0A8 / `QueueKey`. It does not call `ScanKeyboard`, `IsKeyReady`, or `GetKey`, and it consumes no queued event.

The raw predicate is therefore distinct from `IsKeyReady`: A0B0 asks whether hardware currently reports any key down; A09C asks whether the event FIFO contains a queued key.

## Caller review

The four re-extracted official applets used for the neighboring keyboard closures contain the A0B0 import word but no structural executable xrefs to it:

- AlphaWordPlus 2005: 0
- AlphaWordPlus NEO 2012: 0
- KeyWords 2005: 0
- KeyWords NEO 2012: 0

This negative result is retained rather than treating raw `A0B0` bytes as callers.

EhBASIC provides the positive source-level caller described above and independently distinguishes physical key-down state from queue readiness.

## Refutation results

- **Refuted:** A0B0 is another FIFO-ready test. It reads keyboard hardware directly and does not access queue state.
- **Refuted:** A0B0 receives a specific key code. It has no arguments.
- **Refuted:** A0B0 returns a key bitmap. It normalizes any active bit to boolean 1.
- **Refuted:** AS3000 and NEO use the same implementation. AS3000 is `0x24` bytes; NEO is `0x68` and includes stabilization.
- **Refuted:** NEO returns a single unstabilized sample. Its boolean stability counter must reach 5.

## Regression

Static/contractual regression: **70/70 PASS**.

Coverage includes canonical ROM hashes and vectors, exact handler hashes/lengths, active-low `F419` sampling, boolean normalization, byte identity of NEO 2005/2013, AS3000-versus-NEO divergence, the NEO stability loop, absence of known FIFO state addresses, four official applet negative structural searches, and adversarial predicate cases.

- regression source SHA-256: `c6f1d5b163a17b712cbc64463bc3c8dadcc416d9d2da5ff4e6657d37c4b6d9e2`
- regression output SHA-256: `89eec7d0924f93f6ae69198f05881ffdfda2763e7ccc30df41a129c86d71cff4`

A dynamic 68k probe has **not** been executed; it is additional validation rather than a closure blocker.

## Disposition

Document A0B0 as the physical aggregate predicate `bool IsKeyDownNow(void)`, separate from queue readiness. Preserve the real AS3000-versus-NEO hardware and stabilization differences. The next ascending ABI frontier is A0B4 / index 45.