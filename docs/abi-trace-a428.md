# A428 / index 266 — source-first ABI trace

Status: **MECHANICS RECONSTRUCTED (A) / SOURCE_FIRST / RETURN CONTRACT UNKNOWN**.

## Source correlation

Historical BetaWise material and `ioma8/neo-re` independently map index 266 to the neutral placeholder `SYS_A428`. No vendor symbol or independent public prototype has been recovered, so the placeholder name is retained.

## Generation split

- AS3000 2005 and NEO 2005 do not expose a callable handler at this slot.
- NEO 2013 exposes the callable implementation at runtime `0x004347D0`.

## Confirmed physical ABI and mechanics

The NEO 2013 handler consumes one 32-bit stack slot as a full pointer to a writable output buffer.

Its private state selects a record index, computes a record with stride `0x12C`, and follows one of two output paths:

1. a special record state copies one byte to `out[0]`;
2. otherwise a private source string is selected, `strlen` is called, only the low 8 bits of that length are used as the `strncpy` count, and `out[low8(length)]` is explicitly NUL-terminated.

This low-byte truncation is firmware behavior and must not be silently normalized away.

The handler deliberately finishes with `MOVE.B D7,D0`, so `void` is not a faithful description of the machine-level result. Only `D0.B` is stable; the upper 24 bits are not uniform across paths. With no direct firmware caller or independent prototype consuming the result, a public `uint8_t` return remains an inference rather than a confirmed callable contract.

## Callers / xrefs

A whole-ROM search of the canonical NEO 2013 firmware finds no direct `JSR`, `JMP`, `BSR.W`, or `BSR.B` to the A428 handler. The official applet corpus likewise supplies no structural caller for this late slot.

## Adversarial conclusions

- `void` is rejected as a positive contract because the handler deliberately defines `D0.B`.
- a full-width scalar return is rejected because the upper 24 bits are path-dependent.
- a string-only output contract is rejected because one path writes a single byte without the general string-copy sequence.
- full-width `strlen` use is rejected: both copy count and terminator position use only its low byte.

Therefore the public callable prototype remains intentionally unpublished. `RETURN_CONTRACT=UNKNOWN` is an evidence boundary, not an implementation blocker.

## Regression state

A fresh static regression against all three canonical ROMs passed **36/36** assertions, covering hashes, vector slots, handler boundary/hash, instruction coverage, CFG branches, helper targets, physical argument access, record fields, low-byte length behavior, final `D0.B`, and negative direct-xref searches.

Dynamic/emulator-first regression: **specified, not executed**.

Raw ROM bytes, extensive disassembly, and private reverse-engineering artifacts remain outside the public repository.
