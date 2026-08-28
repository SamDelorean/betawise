# System 3 / OS3K packed-record search closure — A284

This note closes the mechanical ABI of trap A284 across the November 2005
AlphaSmart 3000 ROM, November 2005 NEO ROM, and July 2013 NEO/System 3.15 ROM.
The original vendor symbol and the semantic names of the two search fields
remain unknown, so the SDK retains the neutral `SYS_A284` name.

## Reconstructed public form

```c
int32_t SYS_A284(
    const uint8_t **cursor,
    uint16_t field_0,
    uint16_t field_2,
    uint16_t *payload_size_out,
    const uint8_t **payload_out);
```

The five arguments occupy 32-bit stack slots. The two keys are consumed through
their low 16 bits; the remaining arguments are full pointers. The sequence uses
the same packed record layout as A27C and A280:

| Record offset | Width | Meaning |
| ---: | ---: | --- |
| `+0x00` | 16 bits | neutral `field_0` key; zero is the terminator |
| `+0x02` | 16 bits | neutral `field_2` key |
| `+0x04` | 16 bits | payload byte count |
| `+0x06` | variable | payload |

## Search, outputs, and cursor

A284 begins at `*cursor`, stops at the first record whose two keys match, and
uses `field_0 == 0` as the end sentinel. The sentinel test precedes both the
key comparison and the size read, so a requested `field_0 == 0` can never
match a record.

On a match, `payload_size_out` receives the exact third word when non-NULL and
`payload_out` receives `record + 6` when non-NULL. The output pointers are
independently optional. A284 then advances `*cursor` past the matching record
and returns 0.

Each nonmatching record is skipped with:

```text
record + 6 + ((payload_size + 1) & 0xfffe)
```

If no match exists, A284 returns -1 and leaves `*cursor` pointing at the
terminator. It does not preserve the original cursor on failure.

## Edge cases and safety

The even rounding uses 16-bit arithmetic:

| Payload size | Next cursor |
| ---: | --- |
| `0` | `record + 6` |
| odd `1..0xfffd` | `record + 6 + size + 1` |
| even `2..0xfffe` | `record + 6 + size` |
| `0xffff` | `record + 6` after 16-bit wrap |

For a matching `0xffff` record, the returned cursor points at its payload. For
a nonmatching one, scanning resumes from that payload, which can reinterpret
payload bytes as another record. This is observable firmware behavior, not
validation.

A284 touches no OS globals. Its only writes are `*cursor` and any non-NULL
outputs. It performs no NULL check for `cursor` or `*cursor`, no alignment or
bounds check, and no verification that the sequence is terminated or
well-formed. Callers must supply readable records and a reachable terminator.

## Return width, cross-ROM evidence, and callers

The handler explicitly materializes D0 as 0 for found and -1 for not found. The
official caller tests the full longword, establishing a signed 32-bit return
contract rather than a byte or word status.

The complete 0x70-byte handler is byte-identical in all three ROMs, with SHA-256
`08bc8917e3d70ce569cd7b6e37f361d539cb6cc8f9bfd1a8b91435963537ca4e`.

| Firmware | Handler runtime address | File offset |
| --- | ---: | ---: |
| AS3000, November 2005 | `0x004d443a` | `0x01443a` |
| NEO, November 2005 | `0x005d6638` | `0x016638` |
| NEO/System 3.15, July 2013 | `0x00427a98` | `0x017a98` |

Each ROM has one structurally equivalent absolute-call site. It pushes the five
slots, removes 0x14 bytes afterward, and branches on `TST.L D0`. The alternate
path in the same caller invokes A27C with five output pointers over the same
record sequence, independently correlating the layout and cursor role. No
direct A284 opcode or absolute jump caller was found.

## Naming and adversarial review

The authorized BetaWise branch contains no earlier definition of A284.
Historical neo-re preserves only the neutral syscall slot, without a prototype,
comment, or vendor name. Therefore neither key receives an invented semantic
name or enum.

The complete control flow rejects interpretations as a one-record parser,
side-effect-free predicate, pointer/size-returning API, checked sequence
validator, or failure-preserves-cursor lookup.

## Regression status

An emulator-first regression is specified, not claimed as executed. It should
cover empty and unterminated-safe fixtures, first/later/duplicate matches,
failure at the terminator, independently NULL outputs, sizes `0`, `1`, `2`,
`0xfffe`, and `0xffff`, plus the unmatchable zero first key. Invalid pointers
and unbounded corrupt sequences should not be tested on hardware.
