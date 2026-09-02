# A0A4 / index 41 — ScanKeyboard closure

## Status

**MECÁNICA_CERRADA A / REVALIDADO_SOURCE_FIRST / 56/56 PASS / COMPLETADO**

```c
void ScanKeyboard(void);
```

The historical name and prototype are confirmed by direct firmware mechanics in all three canonical ROMs.

## Primary firmware evidence

| Generation | Handler | File offset | Physical span to A0B0 | Instructions |
|---|---:|---:|---:|---:|
| AS3000 2005 | `0x004CE472` | `0x00E472` | `0xF6` | 88 |
| NEO 2005 | `0x005D2456` | `0x012456` | `0x17C` | 140 |
| NEO 2013 | `0x00423446` | `0x013446` | `0x17C` | 140 |

Each window disassembles reproducibly with zero unknown opcodes and terminates in `RTS` immediately before the A0B0 handler.

A0A4 itself is a local veneer into a larger keyboard-scan core. The core acquires the keyboard matrix, evaluates changes, updates keyboard state, and can produce `KeyMod_e` events.

## Matrix acquisition

All three implementations select keyboard-matrix positions and read the keyboard I/O register at `0xFFFFF419`, inverting the sampled byte. The scan covers 16 positions with an explicit special case for index 8.

The AS3000 and NEO implementations differ materially:

- **AS3000:** stores the inverted matrix sample directly into a 16-byte scan-state array.
- **NEO 2005 / NEO 2013:** add a temporal stabilization layer. The scan keeps three 16-byte sample banks, combines samples with OR/AND logic, and maintains a stable matrix state. A time delta threshold of 40 controls the fallback to a fresh sample. NEO 2005 and NEO 2013 have the same `0x17C` control-flow layout; their observed differences are relocations/data addresses.

The mechanism is consistent with keyboard debounce/stabilization. That descriptive label is an engineering interpretation; no unrecovered internal vendor helper name is asserted.

## Change detection and queue production

The per-position processing path compares previous and current matrix bytes and isolates changed bits. For each changed bit it:

1. forms a physical matrix index from the scan position and bit number;
2. translates that index through the firmware keyboard-map helper to a `Key_e` value;
3. ignores `0xFF` as no valid key;
4. combines the key byte with the current 16-bit modifier-state word;
5. sets bit 7 (`KEY_MOD_KEY_UP`, `0x0080`) on the release path;
6. calls **A0A8 / QueueKey** with the resulting `KeyMod_e` word.

The modifier word is the same state independently confirmed by A0A0 / `GetModifierKeys` and A0AC / `SetModifierKeys`. The queue is the same circular 14-word queue consumed by A094 / `GetKey` and observed by A09C / `IsKeyReady`.

This establishes that `ScanKeyboard` is a producer-side keyboard service: it scans physical state and may enqueue translated key events. It does not consume queued keys.

## ABI and return

A0A4 receives no arguments. The historical `void` return is retained. The implementation leaves helper residue in registers on some paths but does not construct a portable return value; callers must not treat `D0` as an API result.

## Official callers

A structural scan of four re-extracted official applets resolved 55 executable A0A4 xrefs:

| Applet | Generation | A0A4 xrefs |
|---|---:|---:|
| AlphaWordPlus | 2005 | 8 |
| AlphaWordPlus | NEO 2012 | 8 |
| KeyWords | 2005 | 18 |
| KeyWords | NEO 2012 | 21 |

These applets also import the neighboring keyboard polling/consumer services A09C and A094. Previously closed firmware paths in A084 / `TextBox` and A088 / `WaitForKey` independently call A0A4 before checking A09C and obtaining a key through A094.

A raw `A0A4` word elsewhere in the NEO AlphaWordPlus image has no structural xref to the import veneer and is not counted as a caller.

## Refutation results

- **Refuted:** A0A4 is a no-op or simple getter. It mutates matrix/state data and can enqueue events.
- **Refuted:** A0A4 consumes the keyboard queue. Queue consumption is in A094; A0A4 reaches the A0A8 producer.
- **Refuted:** AS3000 and NEO use the same scan implementation. NEO adds a real stabilization/history layer and its handler span is `0x86` bytes larger.
- **Refuted:** residual `D0` constitutes an API return contract. The historical and mechanical contract is `void`.

## Regression

Static/contractual regression: **56/56 PASS**.

It checks the three canonical ROM hashes and A0A4 vectors, physical A0B0 boundaries, reproducible instruction decoding, matrix-loop constants, hardware register access, AS3000/NEO acquisition differences, NEO temporal-history markers, effective calls into A0A8, four official applet hashes, and 55 structural callers.

- regression source SHA-256: `f708b05dc9dc90f87147d323b101fd8f058b328736e985a7a0eae1aafe18f5e4`
- regression output SHA-256: `ac7ae05ca1f66843dff56b69b76b9ee893bf8e38e6a8d808aedd680c5415824c`

A dynamic 68k probe has **not** been executed; it is additional validation rather than a closure blocker.

## Disposition

Document A0A4 as the raw `void ScanKeyboard(void)` syscall. Preserve the AS3000-versus-NEO implementation difference instead of flattening it into one hardware algorithm. Internal helper names that are not recoverable from source remain unnamed.
