# A42C / index 267 — source-first ABI trace

Status: **MECHANICS RECONSTRUCTED (A) / SOURCE_FIRST / RETURN CONTRACT UNKNOWN**.

## Source correlation

Historical BetaWise material and `ioma8/neo-re` map index 267 to the neutral placeholder `SYS_A42C`. No independent vendor symbol or semantic prototype has been recovered.

## Generation split

- AS3000 2005 and NEO 2005 do not expose a callable handler at this slot.
- NEO 2013 exposes the callable implementation at runtime `0x0043430E`.

## Confirmed physical ABI

The NEO 2013 handler reads three 32-bit stack slots:

1. `arg1`: full 32-bit base/pointer-like value;
2. `arg2`: only the low byte is loaded, then **sign-extended to a word** before a signed multiply by `0x012C`;
3. `arg3`: full 32-bit value passed to a private helper.

The selected address is mechanically `arg1 + 4 + sign_extend_8(low8(arg2)) * 0x012C`; the handler passes the selected record's `+0x34` address to the helper together with `arg3` and two zero arguments.

The sign extension is a real firmware property. Calling `arg2` merely an unsigned byte selector would lose information about the observed address calculation.

## Helper / result mechanics

The private helper at runtime `0x00442634` calls a lower private routine at `0x00432730` and finishes with `MOVE.W D6,D0`. A42C does not restore D0. It copies `D0.W` to saved register D7 immediately before the register restore, thereby updating the condition codes from the low word while leaving D0 itself in place.

Thus a meaningful low 16-bit result is mechanically present, but neither signedness nor semantic meaning is established. Because there is no direct firmware caller and no independent prototype or result consumer, no callable C return type is published.

## Callers / xrefs

A whole-ROM direct search finds no `JSR`, `JMP`, `BSR.W`, or `BSR.B` to A42C. The official applet corpus provides no structural late-slot caller.

## Adversarial conclusions

- a two-argument ABI is rejected: the third external slot is explicitly loaded and pushed;
- treating `arg2` as a simple unsigned selector is rejected: the firmware sign-extends it before the stride calculation;
- a normalized 32-bit result is rejected: only the helper's low word is deliberately defined at return;
- `void` is not adopted as a positive contract because D0 is intentionally left live and its low word drives the final condition codes.

`RETURN_CONTRACT=UNKNOWN` and semantic identity remain evidence boundaries, not implementation blockers. The neutral `SYS_A42C` name is retained and no callable header is exposed.

## Regression state

A fresh static regression against all three canonical ROMs passed **33/33** assertions, including canonical hashes, generation-specific slots, handler/body hashes, physical argument access, signed selector expansion, record stride, helper call shape, helper result mechanics, and negative direct-xref scans.

Dynamic/emulator-first regression: **specified, not executed**.

Raw ROM bytes, extensive disassembly, and private reverse-engineering artifacts remain outside the public repository.
