# SYS_A298 closure

## Status

- Audit status: **CLOSED A / SOURCE_FIRST / PUBLISHED**.
- Mechanical confidence: **A**.
- Original vendor name: **unknown**; retain the neutral `SYS_A298` identity.
- Dynamic/emulator regression: **specified, not executed**.

## Reconstructed contract

```c
uint8_t SYS_A298(uint8_t selector, uint8_t value);
```

Native callers allocate two 32-bit stack slots and remove eight bytes after the
call. The compared handlers consume only the low byte of each slot. Callers and
the handler use only `D0.B` as the return value; the upper 24 bits are not part
of the contract.

## Mechanical behavior

The operation is a synchronous indexed request over a small global state table.
The neutral behavior established from firmware is:

- the subsystem's primary state byte must be `2` or `3`, otherwise the function
  returns `0x15`;
- `selector` must be in the mechanically accepted range `0..4`, otherwise it
  returns `0x16`;
- each selector addresses an eight-byte table entry; an entry whose first byte
  is already nonzero returns `0x17`;
- `value` is stored in byte `+1` of the selected entry;
- request globals are prepared and a neighboring internal service is invoked;
  a nonzero byte result from that service is propagated;
- the routine then waits for shared busy state to clear and for the selected
  entry to complete;
- a completion byte of `1` returns `0`; another completed value returns `0x18`;
- if the byte counter advances by ten while the entry remains pending, the
  routine clears the entry status and returns `0x1B`;
- if the primary state leaves `2/3` during the waits, it returns `0x15`.

These descriptions are mechanical labels only. They are not recovered vendor
enum names or public semantic names for the selectors or status codes.

Selectors `3` and `4` take a mechanically distinct request-setup path: one
additional request field is set to `5`, and the count passed to the neighboring
service changes from `2` to `3`. The meaning of that distinction remains
unknown.

## Caller evidence

Three equivalent internal callers exist in each canonical ROM. The observed
call patterns use selectors `2`, `3`, and `1`; the second argument is either
zero or a zero-extended byte read from caller state. All three callers discard
exactly eight argument bytes and consume only `D0.B`. Two callers explicitly
tolerate `0x17` in addition to success, confirming that `0x17` is an expected
nonfatal outcome in at least those paths without establishing its vendor name.

No original vendor symbol has been recovered from this evidence.

## Cross-ROM comparison

The handler is exactly `0x126` bytes in all three canonical images:

- AS3000 System 3, November 2005;
- NEO System 3, November 2005;
- NEO/System 3.15, July 2013.

Control flow, constants, selector bound, table stride and numeric result behavior
are equivalent. Literal differences are limited to nine relocated absolute
operands. After neutralizing only those relocations, all three bodies have the
same SHA-256:

`8359e7108d5cc7dd689a770974fad5675da8ccf11dd4261a9b2178b4722c13fc`

The raw firmware bytes and extended disassembly remain in the private research
archive and are intentionally not reproduced here.

## Source-first correlation

The historical/BetaWise/neo-re search did not recover an independent vendor
symbol, prototype, selector enumeration, or status-code enumeration for A298.
That negative result is recorded explicitly rather than converted into a name.

Independent AlphaSmart engineering material from the AS3000 era confirms that
the surrounding firmware family contains a layered IrDA/LAP stack with
state-sensitive, byte-status APIs. Together with the already reconstructed
A28C/A290/A294 neighborhood and the shared state used by A298, this supports a
**strong subsystem-level inference** that A298 belongs to the same IrDA/transport
state machine. It does not justify assigning meanings to selectors `0..4`, the
second byte argument, or numeric results `0x15..0x1B`.

## Adversarial conclusions

The evidence rejects the following alternatives:

- a one-argument ABI: callers reserve two slots and the handler reads both;
- a clean 32-bit return contract: only the low byte is defined/consumed;
- an unrestricted selector: the `< 5` check is explicit;
- a fire-and-forget operation: the routine waits synchronously for shared and
  per-entry completion states;
- treating the indexed storage as unrelated bytes: `selector << 3` establishes
  an eight-byte entry stride.

The public interface therefore remains `SYS_A298`.

## Regression specification

The emulator-first regression remains to be executed. It should cover:

1. invalid primary state -> `0x15` without starting a request;
2. selector `5` -> `0x16`;
3. occupied entry -> `0x17`;
4. propagation of a nonzero low-byte result from the neighboring service;
5. distinct request setup for selectors `3/4` versus the other selectors;
6. pending-to-success transition -> `0`;
7. completed non-success state -> `0x18`;
8. ten-tick pending timeout -> entry status cleared and `0x1B`;
9. loss of primary state during a wait -> `0x15`;
10. upper `D0` bits are ignored as noncontractual.

## Safety

This routine mutates shared subsystem state and can block while polling for
completion. Call it only when the surrounding subsystem is initialized and the
caller understands the state-machine implications. The neutral reconstruction
does not claim meanings for selectors `0..4` beyond their observed mechanical
behavior.
