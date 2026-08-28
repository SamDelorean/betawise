# System 3 / OS3K IrDA state-initializer closure — A28C

This note closes the mechanical ABI of trap A28C across the November 2005
AlphaSmart 3000 ROM, November 2005 NEO ROM, and July 2013 NEO/System 3.15 ROM.
The original vendor symbol and the semantic names of the two strings and event
codes have not been recovered, so the SDK retains the neutral `SYS_A28C` name.

## Reconstructed public form

```c
typedef void (*SYS_A28C_DataCallback)(uint8_t *data, uint32_t remaining);
typedef void (*SYS_A28C_EventCallback)(uint32_t event_code);

uint8_t SYS_A28C(
    const char *primary_name,
    const char *secondary_name,
    SYS_A28C_DataCallback data_callback,
    SYS_A28C_EventCallback event_callback);
```

A28C consumes four 32-bit slots. Both NUL-terminated strings and
`data_callback` are mandatory; `event_callback` may be null. The routine
initializes a singleton OS state block used by the correlated IrDA/IrCOMM,
IrLMP and TinyTP paths. It does not invoke either callback during
initialization.

The data callback is later called with a pointer to a data span and a byte-sized
remaining count extended into a 32-bit slot. The event callback is later called
with one 32-bit event-code slot. Callback returns are ignored. Event values
`0x23` and `0x24` are observed, but their vendor meanings remain unknown.

## Return contract

The low byte of D0 is `0x00` on success and `0x22` on invalid mandatory inputs
or rejected string lengths. Success clears only D0.B, leaving its upper 24 bits
inherited; callers must treat the return as `uint8_t`, not a defined 32-bit
integer. The error path uses `moveq` and therefore happens to set full D0 to
`0x00000022`, but that does not widen the success contract.

## State effects and ordering

Before validating arguments, A28C clears two status bytes, writes a constant
`0x54`, clears both callback slots and both stored string lengths, and installs
any non-null callbacks supplied. Consequently failure is not transactional:
the singleton IrDA state is already partly reset or replaced when `0x22` is
returned. Existing string storage is not fully erased.

On success the routine stores both callbacks, length-prefixes both strings and
NUL-terminates their stored copies. Repeated calls replace the singleton state;
the handler has no locking or reentrancy guard.

## First-string boundary

`primary_name` accepts lengths 0 through 20. Exactly 20 bytes fit and receive a
terminator. A longer primary string is truncated to 20 bytes in the state and
then produces `0x22`, preserving the partial mutation.

## Cross-ROM second-string defect

All three handlers contain the same unsafe comparison in the second copy loop:
they test the first string's count instead of the second string's count.

- With a primary length below 20, the secondary copy has no effective bound and
  continues until NUL. It can overrun the intended state area, and its stored
  byte count wraps after 255.
- With a primary length exactly 20, only an empty secondary string succeeds; a
  non-empty secondary string produces `0x22`.
- With a primary longer than 20, the primary path already fails after partial
  mutation.

This behavior is part of the reconstructed firmware contract. SDK code must not
describe A28C as two independent safe 20-byte copies. Practical callers should
keep both strings short, and destructive boundary tests belong in an emulator.

## Cross-ROM evidence

The complete handler is `0xCA` bytes in every compared ROM. After replacing
only the seven relocated absolute state-field operands with canonical offsets,
all three handlers are byte-identical with normalized SHA-256
`59dfd6fcf1133152118c6b56414607b610d3c9821585cb5d061534be3f8b6075`.

| Firmware | Handler runtime | File range | State base |
| --- | ---: | ---: | ---: |
| AS3000, November 2005 | `0x004c4f5c` | `0x004f5c..0x005026` | `0x00000d20` |
| NEO, November 2005 | `0x005c4f7e` | `0x004f7e..0x005048` | `0x00000d1c` |
| NEO/System 3.15, July 2013 | `0x004153f2` | `0x0053f2..0x0054bc` | `0x00000d4c` |

Equivalent state consumers in each ROM call the two stored pointers with the
same argument shapes. Nearby strings identify the subsystem as IrDA and name
IrCOMM, IrLMP and TinyTP components, without recovering A28C's vendor symbol.

## Caller and historical review

No direct A28C opcode or absolute call to the handler was found in the three
ROMs. A scan of 30 accessible official non-font SmartApplets found A28C only in
their standard A-line stub tables or non-executable packed data; no standalone
caller was validated.

Historical BetaWise material and `ioma8/neo-re` provide neither a recovered
A28C name nor a prototype. Numeric adjacency was not used to assign semantics.

## Adversarial review

An opaque four-pointer interpretation is rejected because the third and fourth
values are later invoked as code with stable argument layouts. A 32-bit return
is rejected because the success path defines only D0.B. A safe 20-byte bound
for the secondary string is rejected by the raw `cmpi.b #20,d1` instruction in
all three ROMs; the secondary counter is D6.

The exact purpose of each string and the meanings of event codes remain open.

## Regression status

An emulator-first regression is specified but has not been executed. It covers
null mandatory inputs, optional event callback, primary lengths 19/20/21,
secondary overflow and length wrap, partial state after failure, callback
non-invocation, D0 upper-bit preservation and all three ROMs. Overflow cases
must not be tested on hardware.
