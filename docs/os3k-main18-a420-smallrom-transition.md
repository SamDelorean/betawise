# OS3K main request 0x18 → A420 → Small ROM handshake

This note records the public, firmware-derived call contract without reproducing ROM bytes or private disassembly.

## Confirmed path

On the NEO 2013 firmware, Manager request `0x18` reaches the existing A-line slot `A420` through the low-memory A-line veneer. The A-line dispatcher decodes the opcode, discards the exception frame, and transfers control to the A420 handler.

The Manager handler supplies exactly two explicit stack arguments before entering the veneer. Depending on an internal state byte, the observed pairs are:

- `(3, 0)`
- `(4, zero-extended internal byte)`
- `(5, zero-extended internal byte)`
- `(0, 0)` on the remaining path

A420 also observes a third external stack slot. That slot is inherited from the surrounding Manager frame rather than explicitly constructed by request `0x18`. Its producer and semantic role remain unresolved.

A420 expands the opaque context supplied through its third slot into four 32-bit values, appends a zero-valued seventh argument, and invokes A3DC. A420 forwards its first external argument unchanged into A3DC slot 1.

## A3DC slot-1 correction

The recovered request-0x18 caller proves that A3DC slot 1 legitimately receives the scalar values `0`, `3`, `4`, and `5`. It must therefore be described as a `full32` slot, not as pointer-required. A3DC initially zero-tests this value before forwarding it deeper; a semantic label such as "mode" or "selector" is not yet justified.

The other A3DC argument properties and its return contract are unchanged by this correction. The contractual return remains unknown.

## Relationship to Small ROM request 0x18

The Small ROM handler for request `0x18` is a separate endpoint. Direct two-generation firmware analysis shows that, once Small ROM is already executing, that handler only emits response `0x56` with zero payload fields. It does not itself perform the transition into Small ROM.

Thus the demonstrated high-level sequence is:

`main Manager request 0x18 → A420/A3DC private transition path → Small ROM becomes active → Small ROM request 0x18 endpoint provides response 0x56`

The exact private helper that effects the execution-context transition has not yet been identified, so that final arrow remains a mechanically strong but incomplete reconstruction rather than a named vendor contract.

## Evidence status

- Main request `0x18` → A-line `A420`: confirmed mechanically.
- A-line veneer/dispatcher behavior: confirmed mechanically.
- A420 first argument → A3DC slot 1 unchanged: confirmed mechanically.
- A3DC slot 1 is `full32` and not pointer-required: confirmed mechanically.
- Small ROM request `0x18` is an ACK/presence endpoint returning `0x56`: confirmed in AS3000 and NEO Small ROMs.
- Meaning of A420 inherited third context: unknown.
- Exact helper responsible for the transition: unresolved / insufficient evidence.
- A420/A3DC return contract: unknown.

No new A-line slot is introduced by this analysis; the project ABI frontier remains unchanged.
