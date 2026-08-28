# System 3 / OS3K packed-record writer closure — A280

This note closes the mechanical ABI of trap A280 across the November 2005
AlphaSmart 3000 ROM, November 2005 NEO ROM, and July 2013 NEO/System 3.15 ROM.
The original vendor symbol and the semantic names of the first two record
fields remain unknown, so the SDK retains the neutral `SYS_A280` name and
offset-based labels.

## Reconstructed public form

```c
void SYS_A280(
    uint8_t **cursor,
    uint16_t field_0,
    uint16_t field_2,
    uint16_t payload_size,
    const uint8_t *payload);
```

The five arguments occupy 32-bit stack slots. The three scalar values are
consumed through their low 16 bits. A280 writes this packed record at `*cursor`:

| Record offset | Width | Input |
| ---: | ---: | --- |
| `+0x00` | 16 bits | `field_0` |
| `+0x02` | 16 bits | `field_2` |
| `+0x04` | 16 bits | `payload_size` |
| `+0x06` | `payload_size` bytes | payload |

The first two fields deliberately remain unnamed. No reviewed source recovers
their vendor names or a safe common enum.

## Cursor update and edge cases

When `payload_size` is nonzero, A280 copies exactly that many bytes to
`record + 6`. It then always replaces `*cursor` with:

```text
record + 6 + ((payload_size + 1) & 0xfffe)
```

The rounding is 16-bit arithmetic:

| Size | Copy | Final cursor |
| ---: | --- | --- |
| `0` | none | `record + 6` |
| odd `1..0xfffd` | exact size | `record + 6 + size + 1` |
| even `2..0xfffe` | exact size | `record + 6 + size` |
| `0xffff` | 65535 bytes | `record + 6` after 16-bit wrap |

For odd sizes, the logical padding byte is not written. Callers that require a
defined pad value must initialize the destination separately.

## Return, state, and preconditions

The public return contract is `void`. D0 contains the rounded size on return
as a consequence of the cursor calculation, but every reviewed caller ignores
it and the handler does not materialize a separate result. Code must not depend
on that residual register value.

A280 touches no OS globals. Its side effects are the three 16-bit header writes,
the conditional forward payload copy, and the cursor update. It performs no
NULL, alignment, capacity, record-boundary, or overlap validation. Both pointers
must be valid and the destination must have sufficient writable storage. The
copy helper is forward-only and must not be treated as overlap-safe
`memmove`.

## Cross-ROM and caller evidence

The complete handler is 0x54 bytes in all three ROMs. Its only byte difference
is the relocated absolute address of an otherwise byte-identical copy helper.
After neutralizing that address, the handler SHA-256 is
`097251739fec13ddb01fb12ee1af54c66cff3e2b6eaeb7d6f1b5b6b2c54f07e2`.

| Firmware | Handler runtime address | File offset |
| --- | ---: | ---: |
| AS3000, November 2005 | `0x004d44f4` | `0x0144f4` |
| NEO, November 2005 | `0x005d66f2` | `0x0166f2` |
| NEO/System 3.15, July 2013 | `0x00427b52` | `0x017b52` |

Each ROM has two direct absolute-call sites. Every site pushes five 32-bit
slots, removes 0x14 bytes afterward, computes capacity as six plus the
even-rounded payload size, and ignores D0. No direct A-line opcode or absolute
jump caller was found.

## Naming and adversarial review

Historical BetaWise has no A280 definition, while neo-re preserves only the
neutral syscall slot. Numeric adjacency to A27C and the complementary mechanics
support describing A280 as a packed-record writer, but do not recover a vendor
name.

Alternative interpretations as an allocator, padding initializer,
overlap-safe move, checked serializer, or size-returning API were rejected by
the complete control flow and caller behavior.

## Regression status

An emulator-first regression is specified, not claimed as executed. It should
use guarded local buffers for sizes `0`, `1`, `2`, `3`, `0xfffe`, and
`0xffff`; verify the three header words, exact copied bytes, untouched padding,
guard regions, and final cursor; and repeat against all three firmware
generations. Invalid pointers and undersized buffers should not be tested on
hardware.
