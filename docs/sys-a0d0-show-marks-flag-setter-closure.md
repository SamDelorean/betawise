# SYS_A0D0 / index 52 — Show Marks flag setter

Status: **mechanically closed / source-first revalidated**.

## Recovered raw contract

```c
void SYS_A0D0(uint8_t enabled);
```

**CONFIRMADO:** the raw A-line service reads the low byte of the first ABI argument slot (`7(SP)` on entry), writes that byte unchanged to one system flag, and returns without a contractual value.

**INFERENCIA FUERTE:** the byte is the system **Show Marks** enable flag paired with `SYS_A0CC`. The original vendor symbol/prototype has not been recovered, so the neutral name remains `SYS_A0D0`.

## Source-first anchors

BetaWise preserves this slot only as `DEFINE_SYSCALL(52, SYS_A0D0)` and does not recover a vendor function name. Official AlphaSmart documentation identifies Show Marks as a system/AlphaWord display setting; AlphaWord Plus' visible command help groups `Sticky Keys, Auto Repeat, and Show Marks status` while Slow Keys is separately exposed.

Those references are functional anchors only. The ABI below is derived from firmware.

## Firmware confirmation

Index 52 resolves to:

| Generation | Handler | File offset | Flag byte |
| --- | ---: | ---: | ---: |
| AS3000 2005 | `0x004CE05E` | `0x00E05E` | `0x4331` |
| NEO 2005 | `0x005D2042` | `0x012042` | `0x57BB` |
| NEO 2013 | `0x00423032` | `0x013032` | `0x5D13` |

All three handlers normalize to:

```text
MOVE.B  7(SP),<relocated_flag>.L
RTS
```

Normalized bytes: `13 EF 00 07 00 00 00 00 4E 75`.
Normalized SHA-256: `22f19ecd692f22902fafe754e19cb20c36ab252e0e2fca760bee18b32da18eaa`.

The offset `7(SP)` selects the low byte of the first 32-bit ABI stack slot used by the syscall stubs, so the portable raw contract is an 8-bit value. No masking, normalization, OR/AND merge, validation, or secondary state mutation occurs in the direct handler.

## Getter pairing and callers

`SYS_A0CC` / index 51 reads the exact same storage byte using `MOVE.B <flag>,D0`. This establishes a direct getter/setter pair independently of any guessed feature name.

A0D0 has exactly four direct firmware callers in each canonical ROM. The call family includes:

- an explicit `0` argument;
- an explicit `1` argument; and
- a save/temporarily-disable/restore sequence in which A0CC first captures the current byte and A0D0 later restores it.

The backing byte is explicitly cleared during initialization. A downstream renderer compares the same byte exactly with `1` before performing an additional display operation.

These primary facts establish setter semantics and boolean-style use. Combined with the documented UI feature and the exclusion of the neighboring Auto Repeat, Sticky Keys, and Slow Keys states, the strongest supported functional interpretation is **set Show Marks state**.

## Refutation checks

- A0D0 is not an unrelated adjacent private helper: the index-52 vector itself resolves to this writer.
- It is not a bit-mask operation: the handler stores the input byte unchanged.
- It is not the setter for Auto Repeat, Sticky Keys, or Slow Keys: those states are held in distinct storage and are exercised by preceding syscall families.
- The parameter is not word/long-width in the raw handler: only byte `7(SP)` is consumed.
- A return value is not established; the handler only stores and returns.

## Regression

Static regression was **EXECUTED: 42/42 PASS** across AS3000 2005, NEO 2005, and NEO 2013. It verifies canonical hashes, index-52 and paired index-51 vectors, exact setter form, first-slot byte access, four direct setter calls per ROM, explicit false/true call sites, paired getter calls, shared storage, zero initialization, the renderer's exact `flag == 1` predicate, and save/disable/restore behavior.

Harness SHA-256: `593088d84bbee8c893d749d47dea5503d1e0484af6f196aba663ea5fcdc8806f`.
Stdout SHA-256: `919e911f27637a5e02a878b1ffc39ea41cd7f2cfb8c8ad5bc9eb768f5e17b573`.

A dynamic display probe is **SPECIFIED / NOT EXECUTED**: write 0/1 through A0D0, read back through A0CC, render text with normally invisible structure, and verify that only the marks layer changes.
