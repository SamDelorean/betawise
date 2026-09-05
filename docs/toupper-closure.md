# A3AC / `toupper` closure

A3AC (index 235) is reconstructed as:

```c
int toupper(int c);
```

Status: **CLOSED A / SOURCE_FIRST / PUBLISHED**.

## Source-first anchors

Before touching firmware, the expected identity was recovered from the preserved BetaWise/OS3K material: `os3k/syscall.c` maps index 235 to `toupper`, and `os3k.h` / `os3k.pdf` declare `int toupper(int c)`. Earlier A2CC work had independently encountered the same routine as an uppercase-ASCII helper. These are anchors only; none is treated as primary proof.

## Primary firmware confirmation

A fresh 2026-09-05 pass re-read the private canonical ROMs from Drive and reproduced all three full SHA-256 identities before extracting A3AC:

- AS3000 2005: runtime `0x004DE7D8`, file `+0x01E7D8`
- NEO 2005: runtime `0x005E1138`, file `+0x021138`
- NEO 2013: runtime `0x00437536`, file `+0x027536`

The complete handler is 0x1A bytes and byte-identical across all three ROMs (SHA-256 `517641aebad0e001ab132b7fa7064ebfe864a3d2549e25dc042c8777d1ead5e3`). It consumes one full 32-bit argument. Values in the inclusive ASCII lowercase interval `0x61..0x7A` are returned minus `0x20`; every other 32-bit value is returned unchanged. There are no helper calls, globals, lookup tables or memory writes.

This **CONFIRMS** an ASCII/C-locale implementation of `toupper`. EOF `-1` (`0xFFFFFFFF`) is preserved. A value such as `0x10000061` is also preserved because comparisons cover the complete longword rather than its low byte.

The same fresh pass reproduced 14 / 14 / 15 direct absolute JSR references for AS3000 2005 / NEO 2005 / NEO 2013 and zero direct absolute JMP references.

## Caller correlation

The existing reproducible official SmartApplet corpus remains complete at 41/41: 57 executable A3AC calls in 20 applets, 10 table-bearing negatives, plus 11 structural negatives without the table. Callers construct one 32-bit slot and representative sites consume or preserve `D0.L`, independently supporting an `int`-sized argument/return.

## Adversarial refutation

- `tolower` is rejected because the transformation is lowercase to uppercase by subtracting `0x20`.
- `isupper`, `islower`, or another predicate is rejected because the handler returns the transformed/original value rather than a boolean.
- A byte-only converter is rejected by full-long comparisons and the unchanged `0x10000061` case.
- A locale/table-driven converter is rejected because the handler has no table, helper or global state.
- Projection from A2CC is rejected as proof: the public syscall vector, ABI, bytes and callers are independently validated here.

No attempted refutation displaced `int toupper(int c)`.

## Validation status

The private full static regression previously executed with **OVERALL PASS**, checking canonical ROM identities, exact handler bytes/hash, the 57-call/20-applet corpus, firmware xrefs, neighboring detector controls A3A0=2 / A3A4=11 / A3A8=39, and full-long boundary mechanics.

A fresh targeted primary regression on 2026-09-05 was **EXECUTED / PASS** for the three canonical ROM hashes, handler extraction/hash/identity, and direct JSR/JMP references. The already-reproducible complete corpus regression was intentionally not repeated.

Dynamic emulator-first regression remains **SPECIFIED / NOT EXECUTED**; minimum cases include `` ` ``, `a`, `z`, `{`, uppercase input, zero, EOF `-1`, and a longword whose low byte is lowercase but whose upper bits are non-zero.

The existing `os3k.h` prototype already matches the confirmed ABI. Firmware, raw handler bytes, extensive disassembly and private corpus artifacts remain in Drive and are not published here.
