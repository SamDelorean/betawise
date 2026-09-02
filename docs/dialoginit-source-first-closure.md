# OS3K A0F0 / DialogInit — source-first revalidation

Status: **CONFIRMED (mechanical / static)**.

Public contract:

```c
void DialogInit(bool single, uint8_t row_first, uint8_t row_last, uint8_t col);
```

This entry revalidates the older Dialog API reconstruction against the three canonical System 3 ROMs rather than treating the previous BetaWise/Ghidra naming as primary evidence.

## Correlation anchors

The historical SDK material exposes the four-argument `DialogInit` prototype above. The earlier Dialog API reconstruction also identifies the shared state model used by `DialogDraw`, `DialogRun`, `DialogSetChoice`, and the choice getters. Those sources are treated as hypotheses/anchors; the conclusions below are determined from the ROM handlers.

## Firmware result

A0F0 is present at index 60 in all three canonical A-line tables. Its entry points are:

- AS3000 2005: `0x004CAE1C` (file offset `0x00AE1C`)
- NEO 2005: `0x005CA974` (file offset `0x00A974`)
- NEO 2013: `0x0041B158` (file offset `0x00B158`)

The exported handler is compact: `0x4C` bytes on AS3000/NEO 2005 and `0x52` bytes on NEO 2013. All three consume four byte-sized values from the low byte of successive 32-bit ABI stack slots: `single`, `row_first`, `row_last`, and `col`.

Confirmed common effects are:

- item count is cleared;
- exit-key count is cleared;
- `first_visible` is initialized to 1;
- `current_choice` is initialized to 1;
- `row_first`, `row_last`, and `col` are stored as bytes;
- `columns` becomes 1 when `single != 0`, otherwise 0;
- the routine returns without producing a public return value.

The identity of `current_choice` is cross-checked against A0FC / `DialogSetChoice`, which writes its byte argument directly to the same state byte initialized to 1 by A0F0. The other 1-initialized byte is therefore the 1-based `first_visible` state used by the drawing/navigation model.

## Generation difference

NEO 2013 performs one additional `CLR.B` of a private dialog-state byte (`0x00004350`) immediately before return. The 2005 AS3000 and NEO handlers do not contain this write. No public semantic name is assigned to that byte because its role is not required to establish the A0F0 ABI contract.

This is a real firmware-generation difference, not a relocation artifact.

## Callers and regression

Direct absolute JSR xrefs to A0F0 were found in the canonical images:

- AS3000 2005: 4
- NEO 2005: 5
- NEO 2013: 6

Concrete callers use the expected four-slot calling pattern; common examples pass geometry values together with `single` as 0 or 1.

A reproducible static regression rechecked canonical SHA-256 hashes, vector resolution, the complete A0F0 opcode grammar, ABI stack-byte offsets, branch-controlled `columns` assignment, A0FC identification of `current_choice`, the NEO 2013 extra clear, and direct JSR presence. Result: **67/67 PASS**.

Regression status is **EXECUTED STATICALLY**. No claim is made that the full visual/dialog behavior matrix has been executed dynamically on hardware or an emulator.

## Correction to the older closure

The earlier broad Dialog API document correctly described the public four-argument signature and the `single -> columns` behavior, but its A0F0 summary listed only `current_choice -> 1`. Direct firmware revalidation shows that A0F0 initializes **two** separate one-based state bytes: `first_visible -> 1` and `current_choice -> 1`. The broad closure should be read with this correction.

## Confidence

- `DialogInit` identity and four-argument ABI: **CONFIRMED**
- common state writes listed above: **CONFIRMED**
- `first_visible` / `current_choice` distinction via A0FC correlation: **CONFIRMED**
- NEO 2013 extra private-byte clear: **CONFIRMED**
- semantic name/purpose of the extra NEO 2013 byte: **UNKNOWN**
