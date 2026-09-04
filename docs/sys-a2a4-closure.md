# SYS_A2A4 closure

## Status

`A2A4` is closed under the current **source-first** methodology with confidence **A** for its ABI and mechanical behavior. No vendor symbol or API name has been recovered, so the neutral symbol `SYS_A2A4` is retained.

```c
uint8_t SYS_A2A4(uint8_t flags,
                 uint8_t selector,
                 const uint8_t *payload,
                 uint8_t payload_size);
```

The physical ABI uses four 32-bit stack slots. The handler consumes only the low byte of slots 1, 2 and 4; slot 3 is a 32-bit pointer. `flags` is deliberately not modeled as `bool`: the A2A0 caller passes `0x80` and other callers pass `0`.

## Source-first correlation

Historical AS3000 documentation independently establishes a state-sensitive IrDA LAP stack, while contemporary AlphaWord release notes confirm IrDA as an operational transfer transport. Within the reconstructed firmware family, A28C establishes IrDA/IrCOMM/IrLMP/TinyTP state, A288 returns the same dynamic `global16 - 6` quantity that A2A4 enforces as its payload-size ceiling, and A298/A2A0 share A2A4's selector domain, state table and byte-sized status model.

Taken together, this makes membership in the same IrDA/transport request machinery a **strong inference**, not merely vector-neighborhood speculation. The correlation also independently strengthens the interpretation of A288 as a transport payload ceiling because A2A4 consumes that exact limit.

No historical source recovered the A2A4 symbol, selector meanings, error enum, protocol meaning of `flags`, or a vendor name for the payload format. `0x80` is therefore preserved as an observed flag value rather than renamed as a protocol bit.

## Corrected function boundary and cross-ROM evidence

The vector interval from A2A4 to A2A8 is 0x40C bytes, but it contains three separate functions. A2A4 itself ends at the first register-restore/RTS and is exactly **0x14A bytes** in all three canonical images:

- AS3000 2005: `0x004C5374..0x004C54BD`
- NEO 2005: `0x005C5396..0x005C54DF`
- NEO 2013: `0x0041580A..0x00415953`

The two functions following that RTS are not attributed to A2A4. This corrects the initial structural upper-bound hypothesis.

Literal handler hashes differ because of absolute relocations. After neutralizing 14 relocated references, the bodies are identical with SHA-256:

`20560082aff06f4610579fd06990a01ef79a15593ec83ea4d4661133160767f1`

No control-flow, constant, argument-layout or structure-offset differences were observed between generations.

## Reconstructed behavior

1. `payload_size` must not exceed a shared 16-bit limit minus 6; otherwise the routine returns `0x11`.
2. `selector` must be below 5; otherwise it returns `0x16`.
3. `selector` indexes a shared table with stride 8.
4. When `flags == 0`, the selected record's state byte must equal 1 or the routine returns `0x19`.
5. If a shared execution byte is zero, A2A4 stores all four arguments in shared request fields, raises a pending flag, and waits with a byte-sized deadline. Failure to complete that deferred request returns `0x20`.
6. On the immediate path, it builds an output buffer. Header byte 0 is derived from one selected-record byte and receives bit 7 when `flags != 0`; header byte 1 comes from another selected-record byte.
7. Selectors 3 and 4 with `flags == 0` append additional control bytes. If payload is present on this path, a shared counter is required; zero returns `0x21`, otherwise the counter is decremented.
8. Except when `payload` equals a demonstrated special shared pointer, exactly `payload_size` bytes are copied into the output buffer.
9. The internal helper is called with a byte-sized total length of `2 + payload_size + extra_length`. A non-zero helper result is propagated through `D0.B`; helper success leads to `D0.B = 0`.

The special pointer is not documented as `NULL`: primary evidence only establishes equality with a particular shared pointer as a copy-suppression sentinel.

## Internal helper

The final helper is 0x84 bytes in all three ROMs:

- AS3000: `0x004C3F2E`
- NEO 2005: `0x005C3F50`
- NEO 2013: `0x004143C4`

After neutralizing absolute relocations, its normalized SHA-256 is:

`617dccea6787cf59e23df8bb42b2ea85a28fb74e99988989fae851b58e73d5bc`

It accepts a byte-sized length, requires a shared major state of 2 or 3 (`0x13` otherwise), returns `0x14` when the shared busy byte is already set, and on acceptance prepares shared buffer/control state, records total length, advances a modulo-8 counter, sets busy, and returns 0. A2A4 propagates its byte-sized non-zero results rather than assigning new meanings to them.

## Callers

Exactly ten absolute JSR sites were found per canonical ROM, with no absolute JMP sites, plus the direct relative call from A2A0.

Four caller families replay the four shared arguments saved by A2A4's deferred path. Six additional families pass concrete combinations using selector 1 or 3, `flags = 0`, pointers, and byte-sized payload lengths such as `0x24` or dynamic values bounded at `0x20`. A2A0 passes the equivalent of `(0x80, selector, &global_param, 2)`.

Every caller accounts for four 32-bit stack slots (16 bytes). Callers that inspect the result use only `D0.B`, confirming the public byte-sized return contract.

Searches in the historical BetaWise/neo-re references did not recover an independent vendor prototype or symbol.

## Adversarial checks

Primary evidence rejects the following alternatives:

- A2A4 is **not** the full 0x40C vector interval; two independent functions follow its RTS.
- `flags` should **not** be published as `bool`, because a real caller passes `0x80`.
- `selector` and `payload_size` are **not** 32-bit in meaning; only their low bytes are consumed.
- `payload` is **not** proven nullable; only a specific pointer sentinel is demonstrated.
- Not every non-zero status is generated locally; internal-helper byte errors are propagated.
- The source-first IrDA/LAP correlation is insufficient to assign protocol names to selectors, flags, headers or status codes.
- A semantic API name such as send-frame/send-packet/IrLMP-send is not justified without nominal evidence and is not published.

## Evidence classification

- **CONFIRMED:** four-argument ABI; byte widths for flags/selector/payload_size; pointer payload; dynamic payload ceiling shared with A288; selector bound; stride-eight table; immediate/deferred paths; header construction; helper behavior; byte return; exact 0x14A function boundary; cross-ROM equivalence.
- **STRONG INFERENCE:** A2A4 is a transmit/request-construction primitive inside the IrDA/transport state machine shared with A28C/A288/A298/A2A0.
- **UNKNOWN:** vendor symbol; selector enum; protocol meaning of flags including `0x80`; names of local status codes; exact protocol-layer interpretation of the two-byte header and selector-3/4 control bytes.

## Validation status

An emulator-first regression is **specified but not yet executed**. It covers size boundaries, selector bounds, state preconditions, non-zero flags including `0x80`, deferred replay and timeout including tick wraparound, immediate buffer construction, the special-pointer path, selectors 3/4, shared-counter exhaustion, helper `0x13`/`0x14` and arbitrary error propagation, byte-only return behavior, and the corrected function boundary.

Full ROM bytes, addresses of private globals, and correlated reverse-engineering workpapers remain private in Drive.
