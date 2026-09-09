# OS3K Manager selector 0x18: cross-ROM Small ROM entry

This note records only reproducible functional behavior and intentionally excludes ROM bytes, extensive disassembly, and proprietary firmware.

## Critical correction: address `0x00400420` is not syscall opcode `A420`

A previous revision conflated two numerically similar but structurally different objects:

- `0x00400420` is a **runtime address** in the NEO Small ROM image;
- `A420` is a **Motorola 68k Line-A opcode** used by the neutral index-264 syscall stub.

The canonical Small ROM object at runtime `0x00400420` begins with privileged interrupt-mask setup and an absolute jump to another Small ROM routine. Its first word is not opcode `A420`. Therefore the NEO 2013 main Manager selector `0x18` does **not** call syscall A420 when it loads `0x00400420` and executes `JSR (A4)`.

This refutes the prior inferred chain `Manager18 -> A420 -> A3DC` and all derived claims about an inherited Manager-frame context being supplied to A420 by this caller.

## Source-first correlation

`ioma8/neo-re`, pinned at commit `732814871fe493e80ed1ab1f959d8dad174ee540`, provides two independent constraints:

1. `alpha-core/src/protocol.rs` constructs the host enter-Small-ROM request as `command(0x18, 0, 0)` and expects status `0x56`.
2. `smartapplets/betawise-sdk/syscall.c` defines syscall stubs by **emitting** `.word 0xA000 + 4*index` at the stub symbol. For index 264 this emits opcode `A420`; it does not establish that syscall A420 resides at absolute runtime address `0x00400420`.

This distinction is exactly the structural rule required by the ABI audit: physical address space and nominal A-line opcode numbering must not be conflated.

## Main OS selector `0x18`

The canonical NEO 2013 main-OS Manager selector `0x18` loads runtime address `0x00400420` into an address register and calls it indirectly. It supplies two 32-bit stack arguments. The first is selected from `3`, `4`, `5`, or `0` according to Manager state; the second is either zero or a zero-extended byte from Manager state. After the call returns, the selector removes exactly eight argument bytes and continues through the shared Manager flow.

The literal `0x00400420` occurs once in the canonical main OS image, in this selector.

## Small ROM entry at `0x00400420`

The independently stored canonical Small ROM maps runtime `0x00400420` to a real executable entry. Mechanically, that entry:

- raises the status-register interrupt mask to `0x2700`;
- transfers control by absolute jump to Small ROM runtime `0x00401642`.

Because the entry uses `JMP`, the return address created by the main OS `JSR (A4)` is retained. The target routine at `0x00401642` later returns with `RTS`, so this is a direct cross-ROM call path rather than an A-line dispatch.

The target routine saves three registers and then reads the low byte of the caller's first 32-bit argument. It explicitly distinguishes values `3`, `4`, and `5`. In each corresponding path it prepares status `0x56` before executing mode-specific Small ROM work. This closes the previously unresolved end-to-end observation: the main Manager `0x18` handler itself does not locally construct `0x56`, but the Small ROM routine that it directly calls does.

No vendor semantic names are assigned to modes `3`, `4`, or `5`.

## Consequences for A420 and A3DC

This correction removes the only claimed firmware caller of syscall A420 that had been inferred from Manager selector `0x18`. The independently reconstructed A420 handler and its A420-to-A3DC mechanical wrapper remain valid as standalone ABI findings, but **they are not part of the demonstrated Manager `0x18` -> Small ROM path**.

Accordingly:

- the historical direct-xref result for syscall A420 remains zero for JSR/JMP/BSR encodings in the canonical NEO 2013 main ROM;
- `manager_dispatcher_local0_context_ptr` is withdrawn as an A420 argument interpretation derived from Manager `0x18`;
- the search for an A3DC helper that performs the Small ROM transition is terminated as based on the false chain;
- A3DC remains independently reconstructed for its own callers and contracts;
- no A-line slot is opened, closed, or renamed solely from this cross-ROM address coincidence.

## Verification

A private regression revalidates both canonical NEO 2013 images, the main selector call shape, the unique main-ROM literal, the Small ROM entry object, the fact that its first opcode is not `A420`, its jump to `0x00401642`, the target's `3/4/5` dispatch, status `0x56` construction, and terminal return. Final run: **26/26 PASS / OVERALL PASS**.

Dynamic/emulator-first validation remains `ESPECIFICADA / NO EJECUTADA`.

Status: **CERRADO A** for the structural correction `address 0x00400420 != opcode A420` and for the static cross-ROM call path. A420's standalone contractual return remains `DESCONOCIDO`; A470 remains the demonstrated ABI frontier.
