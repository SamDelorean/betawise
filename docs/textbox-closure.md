# A084 — `TextBox`

Status: mechanically closed from primary firmware evidence and revalidated source-first.

The historical SDK prototype is retained:

```c
char TextBox(char *buffer,
             uint8_t *len,
             uint16_t max_len,
             const Key_e *exit_keys,
             bool password);
```

`os3k/syscall.c` maps index 33 to `TextBox`.

## What `len` means

`len` is an **input/output current length**, not merely an output parameter. The firmware does not reset `*len` when the function begins. Instead it renders the existing `buffer[0..*len-1]`, then writes a NUL terminator at `buffer[*len]`.

This allows a text box to begin with pre-existing editable content. Callers must provide a valid writable `len` pointer and enough writable buffer storage for the existing data plus the terminating NUL.

## Effective maximum length

Although the public source prototype spells `max_len` as `uint16_t`, the firmware compares `*len` only with the **low byte** of that ABI slot. The mechanically effective maximum is therefore:

```text
max_len & 0xFF
```

For example, 255 remains 255, 256 becomes 0, and 511 becomes 255.

The insertion test is equality, not `>=`. A caller should therefore maintain the invariant:

```text
*len <= (max_len & 0xFF)
```

before entering `TextBox`. Violating that precondition can bypass the intended limit.

The public prototype is not widened or narrowed here: the historical spelling remains part of the SDK surface, while this byte-consumption quirk is documented explicitly.

## Password mode

When the low byte of `password` is exactly `1`, both existing and newly typed characters are displayed as `'*'`, while the real translated bytes remain stored in `buffer`.

The raw handler tests equality with `1`; arbitrary nonzero raw values are not mechanically equivalent to the normal C `true` representation.

## Exit-key list

`exit_keys` is a byte list terminated by `0xFF` (`KEY_NONE`). The terminator itself is not part of the ordinary exit-key scan.

When the low-byte key returned by the keyboard path matches an entry before the terminator, `TextBox` returns that **matching key byte**. It does not use a generic 0/1 success result.

Two reserved marker bytes have additional pre-scan effects:

- `0xFD`: causes the internal A094/GetKey call to run with `process_special_keys == false` instead of the default true.
- `0xFE`: enables an additional platform/event check before the ordinary keyboard loop. If that internal check produces a nonzero status other than 6, `TextBox` returns `0xFE`.

The vendor-level semantic name of that platform/event helper has not been recovered, so this document intentionally describes only its observed effect.

`buffer`, `len`, and `exit_keys` are dereferenced without NULL guards; valid pointers are preconditions.

## Keyboard/editing behavior

After the initial setup, the handler uses the established keyboard services A0A4/ScanKeyboard, A09C/IsKeyReady and A094/GetKey. The full `KeyMod_e` value is retained for A164/TranslateKeyToChar, while the low key byte is used for control decisions.

Confirmed paths:

- **Backspace (`Key_e 0x03`)**: if `*len > 0`, one displayed cell is erased, the cursor moves back, `*len` is decremented and the buffer is NUL-terminated at the new length. At length zero it is ignored.
- **Listed exit key**: returns the matching low-byte key code. Modifier bits are not part of the return value.
- **Tab (`Key_e 0x06`)**: when not itself listed as an exit key, it is ignored.
- **Other keys**: the full key/modifier value is passed to A164/TranslateKeyToChar. A zero translation is ignored. A nonzero translated byte is appended when the effective maximum has not been reached.
- After an accepted edit, `buffer[*len]` is set to NUL again.

## Cursor/display side effect

The routine repeatedly uses A008/GetCursorPos and A004/SetCursor while rendering and editing. It does not read or restore the incoming cursor mode; its SetCursor calls use mode `0x0F` (SHOW).

Therefore `TextBox` preserves/synchronizes cursor position as it edits but may force the cursor visible. A caller that requires a hidden cursor afterward must hide it again.

## Return contract

The ordinary contractual return is the low-byte `Key_e` value that caused exit through `exit_keys`. The special `0xFE` marker path can return `0xFE`.

A dormant-looking terminal path contains `MOVEQ #72,D0`, but it is guarded by a local/register state that the visible handler explicitly clears and never assigns. Under the ordinary m68k callee-saved ABI it is not reachable from the reconstructed function flow. It is therefore not promoted into the public contract.

## Official caller evidence

Structural resolution found one executable A084 caller in each of two official AlphaWordPlus generations:

- AlphaWordPlus 2005: file offset `0x4D1C` → A084 veneer at `0x122AC`.
- AlphaWordPlus NEO 2012: file offset `0x5034` → A084 veneer at `0x12DB4`.

Both construct the exit list:

```text
{ 0x40, 0x48, 0xFF }
```

corresponding to ENTER, ESC and the terminator, pass `max_len = 15`, pass password false, and then compare the return value directly with `0x40` (ENTER). This independently confirms that the normal return is the exit key byte rather than a success flag.

## Important correction to non-primary explanatory material

An illustrative `TextBox` implementation present in one project documentation document resets `*len` to zero and returns 0/1. Direct firmware evidence refutes both behaviors. That illustrative code is therefore not implementation evidence and must not be used to define the ABI.

The useful source-level parts of that documentation are limited to the historical prototype and the statement that `exit_keys` is terminated by `0xFF`; firmware is authoritative for actual behavior.

## Cross-generation verification

| Platform | Runtime | File offset | Size |
| --- | ---: | ---: | ---: |
| AlphaSmart 3000, 2005 | `0x004CC17C` | `0x0000C17C` | `0x20C` |
| NEO, 2005 | `0x005CBCD4` | `0x0000BCD4` | `0x20C` |
| NEO, 2013 | `0x0041C6F4` | `0x0000C6F4` | `0x20C` |

Each handler decodes to 180 instructions with zero unknown opcodes and a terminal `RTS` at `+0x20A`. The only handler-byte differences are relocated absolute JSR destinations. Neutralizing those call operands makes all three bodies byte-identical; normalized SHA-256:

`d725ce896c8f075a42b4175e1fc88e1a42d3ab8fadc8f6f29f40a7fb2fa7f492`

Firmware bytes and full disassembly remain private.

## Regression status

A dedicated static regression was **executed and passed 137/137 checks**, with exit status 0 and empty stderr. It verifies canonical ROM hashes, vector identity, handler hashes/bounds, five-slot consumption, low-byte `max_len`, exact-one password behavior, `0xFD`/`0xFE`/`0xFF` list mechanics, backspace/tab paths, NUL maintenance, reused OS3K calls, cross-ROM normalized identity, official AlphaWordPlus caller resolution and representative argument/return use.

Regression source SHA-256:

`8beb75696ff1deab1759d5353165f25f36db62b83228491aaee3af73f5b8fa7f`

Regression output SHA-256:

`d96c84703aedcb3e530979b2d6cc9fef8bb250dbc624ba36471a2a069651f092`

The first harness run contained four test-harness-only failures: one mistyped expected NEO 2005 handler hash and three incorrect slice lengths for the `max_len` pattern assertion. Correcting those test constants/ranges produced the final 137/137 result; no firmware conclusion changed.

A dynamic hardware/emulator probe has **not** been executed.

## Classification

- **CONFIRMADO:** five-slot order; existing-content behavior; `len` input/output role; low-byte effective maximum; exact-one password masking; `0xFF` terminator; `0xFD`/`0xFE` mechanics; editing behavior; low-byte exit-key return; caller ENTER/ESC usage; cross-ROM normalized identity.
- **INFERENCIA FUERTE:** retaining the historical public C spelling as the SDK surface despite low-byte consumption of `max_len` and `password` inside the raw handler.
- **DESCONOCIDO:** original vendor semantic name of the internal pre-key platform/event helper and intended purpose of the dormant D2/72 path.
