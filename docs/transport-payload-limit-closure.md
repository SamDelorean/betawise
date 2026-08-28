# System 3 / OS3K transport payload-limit closure — A288

This note closes the mechanical ABI of trap A288 across the November 2005
AlphaSmart 3000 ROM, November 2005 NEO ROM, and July 2013 NEO/System 3.15 ROM.
The original public/vendor symbol and the transport's original name have not
been recovered, so the SDK retains the neutral `SYS_A288` name.

## Reconstructed public form

```c
uint16_t SYS_A288(void);
```

A288 consumes no arguments. It reads one relocated 16-bit OS global and returns
its value minus six in the low word of `D0`, with 16-bit wraparound:

```text
result = (current_transport_limit - 6) mod 65536
```

The subtraction corresponds to the six-byte overhead used by the independently
correlated internal send path. That routine rejects a byte-sized payload length
greater than the same `global - 6` value. This establishes the neutral
payload-limit meaning without assigning an unsupported transport or vendor
name.

## Return width and edge behavior

The handler uses only word operations. It does not clear or sign-extend `D0`
before returning, so the upper 16 bits retain their incoming value and are not
part of the contract. `uint16_t` is therefore the portable public form.

| Global value | Returned low word |
| ---: | ---: |
| `0x0000` | `0xfffa` |
| `0x0005` | `0xffff` |
| `0x0006` | `0x0000` |
| `0x0007` | `0x0001` |
| `0x0080` | `0x007a` |
| `0xffff` | `0xfff9` |

The default initialization observed in every compared ROM is `0x0080`, which
yields a payload limit of 122 bytes. This is an observed default, not a
hard-coded API constant: other OS paths can adjust the underlying global.

## Side effects and preconditions

A288 does not write memory and has no pointer, range, alignment, or initialization
preconditions. Apart from the low word of `D0`, it affects only condition codes
as a consequence of the word arithmetic. No error sentinel is distinguished
from a wrapped numeric result.

## Cross-ROM evidence

The complete handler is 0x0c bytes in all three ROMs. After neutralizing only
the relocated absolute global operand, all three extracted handlers are
byte-identical with SHA-256
`dc170245b6872f0b0a780f50b63d9e64225055e729d2d69f91b4ac8939574619`.

| Firmware | Handler runtime | File offset | Global |
| --- | ---: | ---: | ---: |
| AS3000, November 2005 | `0x004c4f50` | `0x004f50` | `0x00000cee` |
| NEO, November 2005 | `0x005c4f72` | `0x004f72` | `0x00000cea` |
| NEO/System 3.15, July 2013 | `0x004153e6` | `0x0053e6` | `0x00000d1a` |

Each ROM contains equivalent initialization and send-path consumers of the
global. The send routine takes a byte-sized payload length, rejects values
greater than `global - 6`, and otherwise proceeds through the same five-channel
state machinery.

## Caller and historical review

No direct A288 opcode or absolute call to the handler was found in the three
ROMs. A scan of 41 accessible official SmartApplets found A288 in their common
A-line stub tables but no validated standalone executable caller. This is a
documented negative result, not evidence for a vendor name.

Historical BetaWise material and `ioma8/neo-re` do not provide a recovered
symbol or prototype for A288. Numeric adjacency to A284/A28C was not used to
assign semantics.

## Adversarial review

An alternative 32-bit return contract was rejected because the handler leaves
the upper half of `D0` untouched. A generic “buffer size” interpretation was
also rejected as too broad: the independent consumer proves specifically that
`global - 6` is the maximum payload length admitted by its internal send path.
The exact transport name remains unknown.

The six-byte subtraction is compatible with a framing/header overhead, but no
field names or wire format are inferred from that arithmetic alone.

## Regression status

An emulator-first regression is specified but has not been executed. It should
test global values `0`, `5`, `6`, `7`, `0x80`, and `0xffff`, verify
low-word wraparound and upper-word preservation, and confirm that memory remains
unchanged. The underlying limit global must not be patched on hardware.
