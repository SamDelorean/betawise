# SYS_A450 / index 276 — ABI trace

Status: **mechanically reconstructed (A), source-level identity blocked**.

## Availability

- AlphaSmart 3000 (2005): slot 276 is `0xF0F1F2F3`; no callable contract is claimed.
- NEO (2005): slot 276 is `0xF0F1F2F3`; no callable contract is claimed.
- NEO (2013): slot 276 resolves to runtime `0x0044331C`.

The NEO 2013 handler is a 108-byte wrapper that uses five 32-bit stack slots plus a 6-byte result channel through `A1`. It first invokes `SYS_A448` with a selector and capacity 2. If the returned aggregate state is 0 or 8 and at least one descriptor was produced, it passes the first descriptor to `SYS_A444` for bounded byte decoding into the caller's destination buffer.

## Observed call shape

Mechanical stack use establishes:

1. slot 1: opaque pointer-like input forwarded to `SYS_A448`;
2. slot 2: NUL-terminated exact selector forwarded to `SYS_A448`;
3. slot 3: writable byte-buffer destination forwarded to `SYS_A444`;
4. slot 4: only the low 16 bits are consumed, sign-extended as the decoding bound;
5. slot 5: writable pointer-like output forwarded to `SYS_A444`.

`A1` points to a 6-byte output aggregate. No sixth stack slot is demonstrated.

## Official callers

A full official-corpus sweep finds three executable callers, all in the NEO Accelerated Reader applet. Their exact selector strings are `/q`, `/q1`, and `/q2`; each supplies an effective bound of 1024 and distinct output buffers. The other 15 NEO applets whose extended A-line tables physically contain A450 have no executable caller. Legacy/table-negative applets cannot call this slot through the same table layout.

The repeated `/q*` literals support a **path-like structural selector** interpretation, but no vendor/source-level function name is recovered. Names such as `GetElementText` or `GetPath` would be speculative.

## Return discipline

The handler does not normalize `D0`: paths that stop after `SYS_A448` inherit that subcall's `D0`, while paths that execute `SYS_A444` inherit its `D0`. All three recovered callers ignore/overwrite `D0`. Therefore no independent scalar return contract is claimed.

## Publication boundary

This trace intentionally omits ROM bytes and extended disassembly. A callable C header is not published because the source-level classification of the 6-byte `A1` result channel (for example hidden structure return versus proprietary convention) remains unresolved.
