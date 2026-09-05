# A3B8 / index 238 — source-first generational closure

Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**.

`SYS_A3B8` remains a project-neutral label. No independently reproducible vendor symbol or historical prototype has been recovered from the available BetaWise/reference material, so firmware and official callers define the narrow raw ABI below.

## Platform availability

Fresh revalidation against the three canonical ROMs confirms the generational split:

- AlphaSmart 3000 (2005): index 238 contains `0x05060708`, not a demonstrated firmware code pointer.
- NEO (2005): index 238 also contains `0x05060708`, not a demonstrated firmware code pointer.
- NEO (2013): index 238 points to runtime `0x0043ECEE` (file offset `0x02ECEE`) and has a real handler.

The 2005 marker is recorded mechanically. It is not labeled `reserved`, `unimplemented`, or assigned vendor semantics, and no callable A3B8 contract is asserted for those ROMs.

## NEO 2013 raw ABI

The handler consumes four physical 32-bit stack slots. Only these widths affect execution: low 16 bits of slot 1, full pointer value of slot 2, low 16 bits of slot 3, and low 8 bits of slot 4. The narrowest justified declaration is therefore:

```c
/* NEO 2013 only; vendor name and argument semantics unknown. */
uint8_t SYS_A3B8(uint16_t arg1, void *arg2, uint16_t arg3, uint8_t arg4);
```

This is a width-preserving raw ABI, not a recovered semantic type signature.

The handler calls private helper `0x0043EEB6` with the four normalized values. A zero helper status exits immediately. A nonzero status causes a second private dependency to be called with literal argument `1`, followed by another helper attempt. The byte counter allows at most six helper attempts. The final helper status is deliberately returned via `MOVE.B D7,D0`; only `D0.B` is contractual.

No semantic names are assigned to the arguments, status values, retry action, helper, or secondary private dependency.

## Handler and callers

Fresh primary revalidation reproduces the NEO 2013 handler at file offset `0x02ECEE`, length `0x50`, ending exactly where A3B4 begins at `0x02ED3E`. SHA-256 of the exact handler bytes is:

`495fb94ec6eaef43abcf182c5ba94b95d421afa4a0866cb0806f81a5306ca143`

The archived deterministic 68k decode remains byte-correlated to that fresh handler and shows the four effective input widths, helper call, retry path, six-attempt bound, and byte return.

The full official SmartApplet regression remains **EJECUTADA / PASS**: 41/41 corpus objects classified; 16 NEO applets have the extended A3B4/A3B8/A3BC table, 14 have legacy tables ending at A3B0, and 11 are structural negatives. Exactly one executable A-line A3B8 caller is present: NEO ControlPanel.

Fresh materialization of that ControlPanel object reproduces its canonical SHA-256, so the archived caller reconstruction remains tied byte-for-byte to current primary evidence. It demonstrates four functional A3B8 inputs and byte-sized return consumption. The apparent cleanup of `0x14` bytes does **not** prove five A3B8 arguments: one older stack slot is retained underneath the four functional slots and is removed by the same cleanup.

A fresh source-first directed regression covering canonical ROM hashes, per-generation slot values, handler boundary/hash, effective-width loads, helper call, retry dependency, retry limit, byte return, and ControlPanel identity is **EJECUTADA / PASS**. Dynamic/emulator regression remains **ESPECIFICADA / NO EJECUTADA**.

## Confidence classification

- **CONFIRMADO:** no demonstrated callable A3B8 handler in the two 2005 canonical ROMs; real implementation in NEO 2013.
- **CONFIRMADO:** four functional stack slots with effective widths `low16 / pointer / low16 / low8`.
- **CONFIRMADO:** helper status drives a bounded retry path; maximum six helper attempts.
- **CONFIRMADO:** only `D0.B` is part of the demonstrated return contract.
- **INFERENCIA FUERTE:** A3B8 is a private hardware/service transaction wrapper with a recovery action between failed attempts.
- **DESCONOCIDO:** vendor name, subsystem name, argument meanings, status meanings, retry meaning, and identity/semantics of the secondary private dependency.

## Adversarial conclusions

- Rejected: treating A3B8 as portable/libc merely because it follows the portable C-library region.
- Rejected: interpreting `0x05060708` as a valid old-firmware code pointer.
- Rejected: deriving five parameters from cleanup size without reconstructing retained stack state.
- Rejected: publishing a full-long return when the handler deliberately stages and the caller consumes only `D0.B`.
- Rejected: inventing a subsystem name, argument names, retry semantics, or status enum.

Full ROM bytes, extensive disassembly, helper listings, caller reconstruction, and regression workpapers remain private in Drive.
