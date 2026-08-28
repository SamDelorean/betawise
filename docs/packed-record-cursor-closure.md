# System 3 / OS3K packed-record cursor closure — A27C

This note closes the mechanical ABI of trap A27C across the November 2005
AlphaSmart 3000 ROM, November 2005 NEO ROM, and July 2013 NEO/System 3.15 ROM.
The original public/vendor symbol and the semantic names of the first two
record fields have not been recovered, so the SDK retains the neutral
`SYS_A27C` name and offset-based field labels.

## Reconstructed public form

```c
void SYS_A27C(
    const uint8_t **cursor,
    uint16_t *field_0_out,
    uint16_t *field_2_out,
    uint16_t *payload_size_out,
    const uint8_t **payload_out);
```

A27C consumes five 32-bit pointer arguments. It reads a packed record at
`*cursor` with this mechanically established layout:

| Record offset | Width | Output |
| ---: | ---: | --- |
| `+0x00` | 16 bits | `*field_0_out` |
| `+0x02` | 16 bits | `*field_2_out` |
| `+0x04` | 16 bits | `*payload_size_out` |
| `+0x06` | pointer | `*payload_out` points here |

The two header fields at `+0x00` and `+0x02` deliberately remain unnamed.
Callers use them as selectors or flags in different paths, but that evidence
does not recover vendor field names or a single safe public enum.

## Cursor update

The routine always writes all four output values. It then rounds the 16-bit
payload size upward to an even byte count and, when the rounded value is
nonzero, replaces `*cursor` with:

```text
payload_out + rounded_payload_size
```

The rounding occurs in 16-bit arithmetic. Observable edge cases are therefore:

| Encoded size | Cursor result |
| ---: | --- |
| `0` | unchanged |
| odd `1..0xfffd` | advances by `size + 1` |
| even `2..0xfffe` | advances by `size` |
| `0xffff` | wraps to zero and remains unchanged |

The unrounded encoded value is still written to `*payload_size_out`.

## Return and side effects

A27C has no stable return contract. `D0` is only a temporary on the nonzero
advance path and is not initialized on the zero-length path. All reviewed
callers ignore it, so the portable declaration is `void`.

The handler touches no OS global state. Its only writes are the four output
locations and, conditionally, `*cursor`. It performs no NULL, alignment,
record-boundary, payload-boundary, or destination validation. Every pointer
must be valid and the record header must contain at least six readable bytes.

## Cross-ROM evidence

The complete handler is 0x4a bytes in all three compared ROMs and the extracted
handler bytes are identical, with SHA-256
`7e4bdb60e8a39d2acbb9c63d45c25fe76d31670a261355e0626283ef65849756`.

| Firmware | Handler runtime address | File offset |
| --- | ---: | ---: |
| AS3000, November 2005 | `0x004d44aa` | `0x0144aa` |
| NEO, November 2005 | `0x005d66a8` | `0x0166a8` |
| NEO/System 3.15, July 2013 | `0x00427b08` | `0x017b08` |

Each ROM contains three direct internal callers. Every caller pushes five
32-bit pointer slots and removes 0x14 bytes afterward. Corresponding call sites
are structurally equivalent across the three generations:

| Caller set | AS3000 | NEO 2005 | NEO 2013 |
| --- | ---: | ---: | ---: |
| 1 | `0x004c027e` | `0x005c027e` | `0x00410286` |
| 2 | `0x004c1bf4` | `0x005c1a9a` | `0x00411c04` |
| 3 | `0x004c78f0` | `0x005c7142` | `0x00417664` |

The callers independently confirm argument order, output widths, conditional
cursor progression, and the absence of a consumed return value.

## Naming and adversarial review

Historical BetaWise and neo-re material only preserve the neutral `SYS_A27C`
slot without a prototype or vendor name. Numeric adjacency to A278/A280 and the
record-like mechanics do not justify assigning a subsystem name.

An alternative interpretation as a value-returning parser was rejected because
the zero-length path leaves `D0` undefined and every internal caller discards
it. An interpretation that the third word is a total record size was also
rejected: the handler adds it to the payload address at `+0x06`, proving that it
counts payload bytes rather than the six-byte header.

## Regression status

The tables above specify a synthetic emulator-first regression matrix; no
dynamic regression is claimed as executed. A safe regression should use local
buffers for sizes `0`, `1`, `2`, `0xfffe`, and `0xffff`, verify all outputs and
the conditional cursor update, and avoid malformed pointers on hardware.
