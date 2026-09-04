# System 3 / OS3K closure — A294

This note closes the mechanical ABI of trap A294 across the November 2005 AlphaSmart 3000 ROM, November 2005 NEO ROM, and July 2013 NEO/System 3.15 ROM. The original public/vendor symbol has not been recovered, so the SDK retains the neutral `SYS_A294` name.

## Reconstructed public form

```c
uint8_t SYS_A294(uint32_t value);
```

A294 consumes one full 32-bit argument. Only `D0.B` is contractual on return.

## Control flow and return behavior

A294 calls an internal worker with `value`. If the worker returns `0x12` in `D0.B`, the call is retried immediately. Any other non-zero worker result is propagated through `D0.B`.

When the worker accepts the operation with byte result `0`, A294 waits while a relocated subsystem-state byte remains `6`. After the state leaves `6`, only final state `3` is treated as success and returns byte `0`; any other final state returns `0x1e`.

The worker stores the complete 32-bit argument in subsystem state before constructing the operation. It exposes early byte errors including `0x12` and `0x13`, and its accepted path moves the subsystem into state `6` while installing the continuation/callback used by the later transition.

## Cross-ROM evidence

The handler is 0x3c bytes in all three canonical ROMs:

| Firmware | Handler runtime | Literal SHA-256 |
| --- | ---: | --- |
| AS3000, November 2005 | `0x004c510a` | `8cda087f8e238cb8ba48b5d790be4c1e5f082fc69c430a0ae26f4fb5716e6bc2` |
| NEO, November 2005 | `0x005c512c` | `7d7cc0a7301521c4267fea1986332aebf79005f31f4d709ca75a21f0dd3f37f4` |
| NEO/System 3.15, July 2013 | `0x004155a0` | `383bfa13532989b9a6912b095f6baff874ce33a19ece9ed1629704be5fbe2f04` |

The differences are relocated absolute operands only. After masking only those relocation bytes, normalized SHA-256 is `e59287ef6f968c1c45f662df073ef182110a81721c585310c7ecb9009f77478c`.

The corresponding workers are at `0x004c3d32`, `0x005c3d54`, and `0x004141c8` and have equivalent control structure across the three firmware generations.

## Caller evidence

Each ROM contains exactly one equivalent absolute JSR caller of A294. No direct A294 opcode or absolute JMP to the handler was found. The caller is the same internal routine that consumes A290: after A290 produces and normalizes a six-entry table, the caller passes one selected record's 32-bit `field_0` unchanged as A294's sole argument.

This independently confirms the argument width and the mechanical A290→A294 relationship. It does not establish a vendor name or a semantic name for the 32-bit value.

## Source-first historical correlation

A 1998 AlphaSmart AS3000 engineering-conventions document contains an independent example API for the IrDA LAP stack. It describes a discovery operation that polls for other IrDA-compatible devices, returns discovery results, and distinguishes media-busy and incorrect-LAP-state conditions from success. This source predates the canonical System 3 firmware and therefore provides an independent architectural anchor rather than a derivation from the A-line binaries.

Combined with the already-confirmed A290 discovery/list construction and the direct A290→A294 caller relationship, this makes it a **strong inference** that A294 initiates a subsequent LAP-layer operation against the selected discovery result. It still does **not** recover the vendor symbol, prove that `value` was named a device address in the original source, or map the numeric status values to historical enum names. Those details remain deliberately neutral.

## Historical and adversarial review

No independent A294 name or prototype was recovered from the historical BetaWise material or the modern `neo-re` reference. The surrounding subsystem is compatible with IrDA operation, but the public contract intentionally does not rename `value` as a device address or other protocol-specific identifier without independent nominal evidence.

A pointer signature is rejected because neither A294 nor its worker dereferences the argument; it is treated as a stored 32-bit value. A clean 32-bit return is rejected because the wrapper and its caller consume only the low byte. `0x12` is not a terminal A294 error because the wrapper explicitly retries it. State `6` is a wait state, not success; only final state `3` yields success.

## Regression status

The existing static/cross-ROM evidence remains the basis of the confidence-A mechanical closure. An emulator-first dynamic regression is specified but has not been executed. It should verify: one or more worker `0x12` results followed by `0` cause retries; `0x13` or another non-zero value other than `0x12` propagates immediately; accepted operation remaining in state `6` waits; transition from `6` to `3` returns `0`; transition from `6` to another state returns `0x1e`; the input remains full-width 32-bit; and only `D0.B` is part of the return contract.
