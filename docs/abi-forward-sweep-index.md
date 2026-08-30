# OS3K ABI forward-sweep traceability index

This document is the canonical public traceability supplement for the forward
ABI reconstruction work that follows A28C. It complements
[`abi-reconstruction-index.md`](abi-reconstruction-index.md), whose historical
base table currently ends at A28C.

It deliberately distinguishes mechanically closed/public interfaces from
blocks whose remaining contractual intent cannot be proved from the available
corpus. Original vendor names are not invented.

## Forward sweep status

| Trap | Neutral/current contract | Status | Public evidence |
| --- | --- | --- | --- |
| A290 | `uint8_t SYS_A290(SYS_A290_Record records[6], uint8_t optional_out32[32])` | mechanical A; published; 6 records of 6 bytes; optional 32-byte output | [`sys-a290-closure.md`](sys-a290-closure.md), `os3k/sys_a290.h` |
| A294 | `uint8_t SYS_A294(uint32_t value)` | mechanical A; published; byte result; retry/state machine closed | [`sys-a294-closure.md`](sys-a294-closure.md), `os3k/sys_a294.h` |
| A298 | `uint8_t SYS_A298(uint8_t selector, uint8_t value)` | mechanical A; published; two physical slots with low-byte consumption | [`sys-a298-closure.md`](sys-a298-closure.md), `os3k/sys_a298.h` |
| A29C | `uint8_t SYS_A29C(void)` | mechanical A; published; no arguments; byte result | [`sys-a29c-closure.md`](sys-a29c-closure.md), `os3k/sys_a29c.h` |
| A2A0 | `uint8_t SYS_A2A0(uint8_t selector)` | mechanical A; published | [`sys-a2a0-closure.md`](sys-a2a0-closure.md), `os3k/sys_a2a0.h` |
| A2A4 | `uint8_t SYS_A2A4(uint8_t flags, uint8_t selector, const uint8_t *payload, uint8_t payload_size)` | mechanical A; published | [`sys-a2a4-closure.md`](sys-a2a4-closure.md), `os3k/sys_a2a4.h` |
| A2A8 | `uint8_t SYS_A2A8(uint32_t context, const uint8_t *payload, uint8_t payload_size)` | mechanical A; published | [`sys-a2a8-closure.md`](sys-a2a8-closure.md), `os3k/sys_a2a8.h` |
| A2AC | `int32_t SYS_A2AC(uint8_t *out_byte)` | mechanical A; published; AS3000 and NEO generation difference retained | [`sys-a2ac-closure.md`](sys-a2ac-closure.md), `os3k/sys_a2ac.h` |
| A2B0 | one physical byte-valued input; residual D0 equals zero-extended input | **blocked / contractual return unknown**; not published as a callable contract | no public contract; private evidence retained in Drive |
| A2B4 | `void SYS_A2B4(uint32_t *out_0, uint32_t *out_1, uint32_t *out_2)` | mechanical A; published | [`sys-a2b4-closure.md`](sys-a2b4-closure.md), `os3k/sys_a2b4.h` |
| A2B8 | raw `_OS3K_CallSysInt(uint32_t unused_zero, SysInt_e info, void *io)` | mechanical A; published; first physical slot unused; selector-dependent I/O | [`sys-a2b8-closure.md`](sys-a2b8-closure.md), `os3k/os3k.c` |
| A2BC | `int32_t SYS_A2BC(void *state, uint16_t selector, uint16_t flags, uint32_t value, uint16_t coord_x, uint16_t coord_y, uint16_t extent_x, uint16_t extent_y)` | mechanical A; published; generation differences retained | [`sys-a2bc-closure.md`](sys-a2bc-closure.md), `os3k/sys_a2bc.h` |
| A2C0 | `uint32_t SYS_A2C0(void *state, uint32_t selector, uint32_t param, uint32_t reserved, uint32_t *status_out)` | mechanical A; published; fourth slot physically present but unused | [`sys-a2c0-closure.md`](sys-a2c0-closure.md), `os3k/sys_a2c0.h` |
| A2C4 | `int32_t SYS_A2C4(void *state)` | mechanical A; published | [`sys-a2c4-closure.md`](sys-a2c4-closure.md), `os3k/sys_a2c4.h` |
| A2C8 | `uint32_t SYS_A2C8(const void *source, void *out_state, uint8_t selector)` | mechanical A; published; AS/NEO output difference retained | [`sys-a2c8-closure.md`](sys-a2c8-closure.md), `os3k/sys_a2c8.h` |
| A2CC | `uint32_t SYS_A2CC(void *state, uint32_t limit, uint8_t *out_bytes, uint8_t arg4, uint8_t arg5, uint8_t arg6, uint8_t arg7, uint8_t arg8, uint8_t arg9, const uint8_t *arg10)` | mechanical A; published; ten physical slots | [`sys-a2cc-closure.md`](sys-a2cc-closure.md), `os3k/sys_a2cc.h` |
| A2D0 | `uint32_t SYS_A2D0(const void *state, uint32_t limit, uint8_t arg3, uint8_t arg4, uint8_t arg5)` | mechanical A; published | [`sys-a2d0-closure.md`](sys-a2d0-closure.md), `os3k/sys_a2d0.h` |
| A2D4 | `uint32_t SYS_A2D4(const void *state, const uint8_t *source, uint32_t value, uint8_t flags)` | mechanical A; published; four 32-bit physical slots | [`sys-a2d4-closure.md`](sys-a2d4-closure.md), `os3k/sys_a2d4.h` |
| A2D8 | `uint32_t SYS_A2D8(const void *state, uint8_t post_flag)` | mechanical A; published; two physical slots | [`sys-a2d8-closure.md`](sys-a2d8-closure.md), `os3k/sys_a2d8.h` |
| A2DC | `uint32_t SYS_A2DC(void *state, const void *source_4e, const void *source_52, const void *source_56, const void *source_5a, uint8_t post_flag)` | mechanical A; published; AS3000 is a platform stub while NEO implements the state operation | [`sys-a2dc-closure.md`](sys-a2dc-closure.md), `os3k/sys_a2dc.h` |
| A2E0 | `uint32_t SYS_A2E0(void *state)` | mechanical A; published; NEO 2013 preparation difference retained | [`sys-a2e0-closure.md`](sys-a2e0-closure.md), `os3k/sys_a2e0.h` |
| A2E4 | two mandatory pointer inputs; confirmed memset/copy effects; D0 mechanically retains destination pointer | **blocked / contractual return intent unknown**; no public callable contract | no public contract; private evidence retained in Drive |
| A2E8 | `uint8_t SYS_A2E8(void *state, uint32_t selector, void *arg3, uint32_t *out_value, void *unused_arg5)` | mechanical A; published; five physical slots, fifth unused by all compared handlers; exact final D0 domain 0/1 and official callers consume D0.B | [`sys-a2e8-closure.md`](sys-a2e8-closure.md), `os3k/sys_a2e8.h` |
| A2EC | mechanics, stack accesses, optional pointer outputs and cross-generation control flow reconstructed; D0 mechanically propagates from a terminal helper but no observed caller consumes it | **blocked / contractual return intent unknown**; `void` is strong inference only; no callable contract published | no public callable contract; private evidence retained in Drive |
| A2F0 | `uint32_t SYS_A2F0(uint32_t arg1_slot, void *state, uint32_t arg3_slot)` | mechanical A; published; three physical slots; low16/ptr/low8 consumption; raw D0.L status | [`sys-a2f0-closure.md`](sys-a2f0-closure.md), `os3k/sys_a2f0.h` |
| A2F4 | `uint32_t SYS_A2F4(void *state, uint32_t message, uint32_t param, void *arg4, uint32_t *out_value)` | mechanical A; published; five physical slots; writable output; contractual raw D0.L 0/1 | [`sys-a2f4-closure.md`](sys-a2f4-closure.md), `os3k/sys_a2f4.h` |
| A2F8 | two physical pointer inputs; destination initialization and source-field transfer reconstructed; D0 mechanically retains `source+0x12` and all observed callers ignore it | **blocked / contractual return intent unknown**; `void` is a very strong inference only; no callable contract published | no public callable contract; private evidence retained in Drive |
| A2FC | `uint8_t SYS_A2FC(void *state, uint32_t selector, void *arg3, uint32_t *out_value, void *unused_arg5)` | mechanical A; published; five physical slots, fifth unused; exact final D0 domain 0/1; official callers consume D0.B | [`sys-a2fc-closure.md`](sys-a2fc-closure.md), `os3k/sys_a2fc.h` |
| A300 | one physical pointer input; state-relative update chain terminating in A2F0; D0 mechanically propagates the A2F0 result | **blocked / contractual return intent unknown**; `void` is a very strong inference only; no callable contract published | [`sys-a300-blocked.md`](sys-a300-blocked.md); private primary evidence retained in Drive |
| A304 | `uint8_t SYS_A304(void *state, uint16_t value, const char *string)` | mechanical A; published; three physical slots; byte 0/1 return consumed with `TST.B`; generation difference retained | [`sys-a304-closure.md`](sys-a304-closure.md), `os3k/sys_a304.h` |

## A2E8 publication checkpoint

A2E8 was published atomically on `sdk/abi-automation` as:

- `os3k/sys_a2e8.h`: `9fa0da8a0c6fb7b7ca521257397d910e34791ccc`
- `docs/sys-a2e8-closure.md`: `565fba401ed56d0eefe5d717324ea73d21a522e7`

The authoritative branch comparison before and after publication was linear with
`master`: merge-base `a1306c113d5861e7cbed09595d58ec114d30d1b1`,
zero commits behind. No firmware, ROM bytes or extensive disassembly are stored
in this public supplement.

## A2F0 publication checkpoint

A2F0 was published on `sdk/abi-automation` as:

- `os3k/sys_a2f0.h`: commit `5b1d30d7ceea32bc4b15524cafe744ff61158f61`
- `docs/sys-a2f0-closure.md`: commit `db12a3877ff57a5b399c49c94973cb0b3ccce743`

Before publication the branch was 88 commits ahead and zero behind `master`,
with merge-base `a1306c113d5861e7cbed09595d58ec114d30d1b1`. The closure publishes only
neutral contract and synthetic documentation; ROM bytes and extensive
workpapers remain private in Drive.

## A2F4 publication checkpoint

A2F4 was published on `sdk/abi-automation` as:

- `os3k/sys_a2f4.h`: commit `2cf5740e581440ac55f7ac747033a9c8f92b54b2`
- `docs/sys-a2f4-closure.md`: commit `89ed3b1f2b9e47848dbe2ee3920bbbddfe0735fc`

The corrected static regression passed after the exhaustive 41/41 official
SmartApplet caller sweep. The public material contains only the neutral ABI and
functional closure; ROM bytes and extensive disassembly remain private in Drive.

## A2FC publication checkpoint

A2FC was published on `sdk/abi-automation` as:

- `os3k/sys_a2fc.h`: commit `fa3ddeb5a5550507bc31bf42bcfd5e4077dc2568`
- `docs/sys-a2fc-closure.md`: commit `3b954b806f4a2d9b26b74d3d2506c9f20f5c7c50`

The private static regression passed after the 41/41 official SmartApplet sweep,
separate per-ROM disassembly, exhaustive fifth-slot access check, helper
correlation, and adversarial correction of the return width. Although D0.L is
mechanically defined as 0 or 1, both official callers consume the result with a
byte-sized test, so the neutral public interface exposes `uint8_t` rather than
inventing a wider contract.

## A304 publication checkpoint

A304 was published on `sdk/abi-automation` as:

- `os3k/sys_a304.h`: commit `4fe0df98d475967c7eb1e4a5e7d72f66cf196616`
- `docs/sys-a304-closure.md`: commit `0307f83bffa464a6f69f9c584dc4d04c6034705b`

The private static regression executed with OVERALL PASS after the complete
41/41 official SmartApplet caller sweep. AS3000 retains an eight-byte mechanical
difference from the NEO implementations; NEO 2005 and NEO 2013 normalize to the
same handler after relocations. The public contract therefore preserves that
generational difference rather than normalizing it away.

## Validation policy

All entries marked mechanical A were reconstructed from primary firmware and
correlated evidence under the project methodology. A regression marked
"specified" in a closure document is not reported as executed unless that
closure explicitly records execution. Blocks A2B0, A2E4, A2EC, A2F8 and A300
remain deliberately blocked rather than receiving guessed return types.