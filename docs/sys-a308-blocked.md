# SYS_A308 — forward-sweep disposition

Status: **MECHANICS_RECONSTRUCTED / BLOCKED on contractual return only**.

This note records the public, non-firmware disposition of OS3K A-line block `A308`. It does not publish firmware bytes or an extended disassembly.

## Confirmed mechanics

- The handler consumes one physical 32-bit argument slot as a mutable pointer.
- The handler is mechanically equivalent across the canonical AS3000 2005, NEO 2005, and NEO 2013 ROMs after neutralizing call-target relocations.
- Its observable object accesses include offsets `+0x00`, `+0x24`, `+0x42`, and `+0x46`.
- A normal path explicitly stages zero in `D0`; an error path can propagate a helper-produced longword through `D0`.
- The canonical official SmartApplet corpus was exhaustively checked: 41/41 applets, with six executable A308 callsites in three binaries. All six pass exactly one pointer argument.
- None of those six callsites consumes the value left in `D0` before it is clobbered.
- Direct ROM-wide JSR/JMP/BSR searches to the handler are negative in all three canonical ROM generations.
- Static regression for hashes, handler boundaries, normalized equivalence, positive callers, and negative direct ROM xrefs passes.

## Contractual return

A `void SYS_A308(void *state)` prototype is a **very strong inference**, but it is not independently demonstrated. The residual `D0` mechanics are also compatible with a status-returning implementation whose result happens to be ignored by all known callers.

Following the project evidence policy, caller non-use alone is not treated as proof of `void`. Therefore no callable C prototype is published for A308 at this stage.

The neutral public state remains:

`SYS_A308`: argument ABI reconstructed; **contractual return unknown**.

Reopen during the integral audit if an independent vendor prototype, symbol, or caller that consumes the return becomes available.
