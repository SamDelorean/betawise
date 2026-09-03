# SYS_A274 source-first closure

## Status

`A274/index157` is **source-first closed (confidence A)** for the portable mechanical contract:

```c
void SYS_A274(void);
```

The original modern public/vendor symbol has not been recovered. Historical AlphaSmart source from 1999-2000 does, however, contain `void PrintSelectPrinter(void)`, and the modern System 3 handler preserves the same distinctive interaction model. `PrintSelectPrinter` is therefore recorded as a historical predecessor/name anchor, not asserted as a proven exported modern symbol.

## Source-first correlation

The early `PrintModule.c` implementation identifies `PrintSelectPrinter(void)` as the routine that lets the user select the printer connected to the AlphaSmart. It clears the display, renders the localized printer-selection prompt, builds a dialog menu, marks the current printer with `+`, registers Escape and Return as exit keys, initializes the cursor to `selected + 1`, and on Return stores `DialogMenuGetCursorItemNumber() - 1`. The early public header `PrintModule.h` also declares `void PrintSelectPrinter(void)` and defines the original five printer IDs.

This source evidence was treated only as an anchor. The contract below was revalidated directly against all three canonical System 3 ROMs.

## Canonical firmware validation

The three canonical firmware hashes were rechecked before analysis:

- AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

The A-line vector resolves A274 to a real handler in all three images. Fresh handler bytes exactly match the independently retained instruction-by-instruction 68k extractions, with contiguous decoding through the terminating `RTS`.

No handler reads caller arguments. D0 is helper/scratch state and is not normalized into a caller-visible semantic result before return. The portable ABI is therefore `void SYS_A274(void)`.

## Confirmed behavior

All three generations perform the same core sequence:

1. prepare/clear the display and obtain the localized printer-selection instruction;
2. initialize the Dialog API;
3. enumerate a fixed printer-record table with a 40-byte stride;
4. read the printer-name resource ID from record offset `+0x22`;
5. render the selected record with `+` and the others with a space;
6. register Escape (`0x48`) and Enter/Return (`0x40`) as dialog exits;
7. initialize the dialog choice to the stored zero-based selected index plus one;
8. run the dialog;
9. Escape exits without changing the stored selection;
10. Enter/Return obtains the current one-based choice, subtracts one, and commits the resulting zero-based candidate subject to generation-specific validation.

The key-code interpretation is independently consistent with the current BetaWise `Key_e` values (`KEY_ESC = 0x48`, `KEY_ENTER = 0x40`) and with the historical `ICODE_ESCAPE` / `ICODE_RETURN` control flow.

## Generation differences

### AS3000 2005

The handler presents seven records. In addition to the five printer families retained by NEO, the table includes Apple ImageWriter and Apple StyleWriter entries. Its acceptance path contains an extra dependency check around the StyleWriter case. If that dependency is unavailable, System 3 displays the retained StyleWriter-font warning and does not commit the candidate selection.

### NEO 2005 / NEO 2013

Both handlers present five records and share the same compact acceptance model. ImageWriter and StyleWriter are absent from the enumerated selection table. After the common applet lookup side effect, the accepted zero-based choice is written directly to the generation-specific selected-printer state byte.

The two NEO handlers are mechanically equivalent apart from normal relocation of code/data addresses.

## Callers and negative search

A complete direct absolute-JSR scan of the canonical System 3 images finds one internal AS3000 caller and no direct NEO callers (`1/0/0`). The AS3000 call site invokes the handler without preparing stack arguments.

A renewed survey of representative official SmartApplet binaries from both generations finds A274 only inside the contiguous A-line syscall stub table; no independent executable SmartApplet call site was established. This supports treating A274 primarily as a System-owned UI service rather than a commonly called SmartApplet API.

## Refutation attempt

The earlier working hypothesis that A274 was related to keyboard-layout selection is rejected. Firmware resolves record `+0x22` values to printer-name resources and uses the localized printer-selection prompt. Historical `PrintSelectPrinter` source independently reproduces the same menu semantics. The printer-selection interpretation survives the cross-source refutation test.

## Regression

Static regression is **EXECUTED: 89/89 PASS**. It covers all three canonical hashes, vector targets, exact handler bounds, byte-for-byte agreement with the retained disassembly, control-flow anchors, fixed record counts and stride, selected-state/table references, printer resource access, marker construction, Escape/Enter behavior, choice conversion, the AS3000 warning resources, direct-JSR counts, and the negative SmartApplet caller survey.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**. Emulator/device tests should verify menu rendering, initial selection, cancellation, accepted selection, and the AS3000 StyleWriter dependency-warning branch.

## Confidence classification

- **CONFIRMED:** `void SYS_A274(void)` portable mechanical ABI.
- **CONFIRMED:** interactive printer-selection function and core dialog semantics.
- **CONFIRMED:** AS3000 seven-record versus NEO five-record generation difference.
- **CONFIRMED:** Escape/Enter behavior and zero-based stored selection.
- **STRONG INFERENCE:** historical `PrintSelectPrinter` is the direct genealogical predecessor/name of the modern handler.
- **UNKNOWN:** original modern exported/vendor symbol, if any, for trap A274.

ROM bytes, extensive disassembly, private addresses/xrefs and workpapers remain outside the public repository.