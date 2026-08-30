# A3AC / `toupper` closure

A3AC (index 235) is mechanically reconstructed as:

```c
int toupper(int c);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004DE7D8`, file `+0x01E7D8`
- NEO 2005: runtime `0x005E1138`, file `+0x021138`
- NEO 2013: runtime `0x00437536`, file `+0x027536`

The complete handler is 0x1A bytes and byte-identical across all three ROMs (SHA-256 `517641aebad0e001ab132b7fa7064ebfe864a3d2549e25dc042c8777d1ead5e3`). It consumes one full 32-bit argument. Values in the inclusive ASCII lowercase interval `0x61..0x7A` are returned minus `0x20`; every other 32-bit value is returned unchanged. The operation has no helper calls, globals, tables or memory writes.

This is an ASCII/C-locale implementation of `toupper`. In particular, `-1` (`0xFFFFFFFF`) is outside the lowercase interval and is preserved unchanged. A value such as `0x10000061` is also preserved: the comparisons operate on the complete longword, not only its low byte.

## Caller correlation

The official SmartApplet corpus is complete at 41/41. The physical A-line target (`A378 + 0x1A`) yields 57 executable A3AC calls in 20 applets; 10 table-bearing applets are negative and the remaining 11 corpus members are structural negatives without the table.

Callers construct one 32-bit slot. Representative sites consume or preserve `D0.L` with full-longword moves, comparisons or immediate return. This independently agrees with the handler's full-long argument comparisons and full-long return.

Direct firmware xrefs are 14/14/15 absolute JSRs for AS3000 2005 / NEO 2005 / NEO 2013, with no direct JMP or BSR entry use.

## Adversarial alternatives

- `tolower` is rejected because the only transformation is lowercase to uppercase by subtracting `0x20`.
- `isupper`, `islower`, or another predicate is rejected because the handler returns the transformed/original value, not a boolean.
- A byte-only converter is rejected by the full-long comparisons and by the `0x10000061` case remaining unchanged.
- A locale/table-driven converter is rejected because the handler contains no lookup, helper or global state; its mapping is explicitly ASCII-only.

The same routine had previously been identified as an internal uppercase-ASCII helper while reconstructing A2CC. That earlier correlation is corroboration only; this closure independently revalidates the A3AC vector, ROM bytes, ABI, callers and return.

The historical BetaWise `int toupper(int c)` declaration is secondary corroboration only. The contract follows from ROM mechanics and callers.

## Validation status

Static regression over all three canonical ROMs and the complete official SmartApplet corpus executed with **OVERALL PASS**. It checks canonical ROM hashes, exact handler bytes/hash, firmware xrefs, the exact 57-call/20-applet corpus, neighboring detector controls A3A0=2 / A3A4=11 / A3A8=39, full-long mechanics and boundary vectors.

Dynamic emulator-first regression is specified but not executed; its minimum matrix includes `` ` ``, `a`, `z`, `{`, uppercase input, zero, EOF `-1`, and a longword whose low byte is lowercase but whose upper bits are non-zero.

Status: **MECÁNICA_CERRADA A / PUBLICADO**. The existing `os3k.h` prototype already matches the reconstructed contract, so no header modification is required.
