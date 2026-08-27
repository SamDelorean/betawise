# A274 printer-selection service closure

## Status

`A274` is mechanically closed with **confidence A** for the System 3 ROMs
compared here:

- AlphaSmart 3000 System 3, 9 Nov 2005
- NEO System 3, 9 Nov 2005
- NEO/System 3.15, 11 Jul 2013

No trustworthy original public/vendor symbol has been recovered. The generic
BetaWise name is therefore intentionally retained:

```c
void SYS_A274(void);
```

This document uses **printer-selection dialog/service** only as a functional
description, not as a claim about an original symbol name.

## What the handler does

A274 is the interactive System 3 printer-selection UI. It consumes no caller
arguments and does not establish a caller-visible return contract.

The common flow is:

1. prepare/clear the display and render a localized instruction;
2. initialize a Dialog API menu;
3. iterate a fixed table of 40-byte printer records;
4. resolve the word at record `+0x22` as a localized printer-name resource;
5. add one dialog item per record, using `+` for the currently selected record
   and space for the others;
6. install two dialog event/exit codes, raw `0x48` and `0x40`;
7. set the initial 1-based dialog choice to `selected_index + 1`;
8. draw/run the dialog;
9. raw event `0x48` exits without changing the selected printer;
10. raw event `0x40` obtains `DialogGetChoice()`, subtracts one and treats the
    result as the candidate zero-based printer-record index.

The raw `0x48`/`0x40` event names are not recovered and are deliberately not
added to `Key_e` under invented names.

## ABI

### Arguments

None. All three handlers begin their work without reading caller stack
arguments. An internal AS3000 call site likewise invokes the implementation
without preparing arguments.

### Return

There is no stable semantic return. `D0` is repeatedly reused for results and
scratch values from resource/dialog helpers and is not normalized before
`RTS`. The portable contract is therefore `void`.

## Direct printer-resource evidence

The same localized-resource lookup used by A274 maps the printer-record
resource IDs in the NEO 2005/2013 images as follows:

| Resource | Text |
| --- | --- |
| `0x22` | Apple ImageWriter |
| `0x23` | Apple StyleWriter (I, II, 1200) |
| `0x24` | HP LaserJet/DeskJet, Lexmark Laser |
| `0x25` | HP DeskWriter Printers |
| `0x26` | Canon Printer |
| `0x2A` | Citizen Printer |
| `0x2B` | Epson Printer |

Resource `0x27` is the instruction `Select a printer, then press enter.`.
Resources `0x28` and `0x29` form the StyleWriter-font-not-installed warning,
and `0x31` is the continue prompt used by the AS3000 warning path.

This resource correlation supersedes an early working hypothesis that A274 was
a keyboard-layout selector. The direct record/resource mapping establishes that
A274 is the printer-selection service.

## ROM-specific state and record sets

### AS3000 2005

- handler: `0x004C5B58` (file `+0x005B58`)
- selected-printer index byte: `0x00000D74`
- printer-record table: `0x004E8F92`
- record stride: 40 bytes
- record count: 7
- resource IDs: `0x24, 0x25, 0x26, 0x22, 0x23, 0x2A, 0x2B`

The AS3000 acceptance path is longer than the NEO path. It calls
`AppletFindById(0xA010)`, compares the candidate printer against resource
`0x23` through an internal comparison helper, and can display the
StyleWriter-font-not-installed warning instead of committing the candidate.
When the validation succeeds, the candidate index is written to `0x00000D74`.

### NEO 2005

- handler: `0x005C5BCC` (file `+0x005BCC`)
- selected-printer index byte: `0x00000D70`
- printer-record table: `0x005EECD0`
- record stride: 40 bytes
- record count: 5
- resource IDs: `0x24, 0x25, 0x26, 0x2A, 0x2B`

The ImageWriter and StyleWriter records are absent. On acceptance the handler
computes `DialogGetChoice() - 1` and stores it directly after the retained
`AppletFindById(0xA010)` call.

### NEO 2013

- handler: `0x00416040` (file `+0x006040`)
- selected-printer index byte: `0x00000DA0`
- printer-record table: `0x0044A20E`
- record stride: 40 bytes
- record count: 5
- resource IDs: `0x24, 0x25, 0x26, 0x2A, 0x2B`

The functional path matches NEO 2005 apart from relocated addresses.

The 7-vs-5 generation/platform difference is therefore concrete: the AS3000
menu retains Apple ImageWriter/StyleWriter choices, while the compared NEO ROMs
expose five non-Apple printer families in this table.

## Correlation with already reconstructed APIs

The A274 handlers directly call implementations already identified as:

- `DialogInit` / A0F0
- `DialogAddItem` / A0F4
- `DialogAddExitKey` / A0F8
- `DialogSetChoice` / A0FC
- `DialogDraw` / A100
- `DialogRun` / A104
- `DialogGetChoice` / A108
- `AppletFindById` / A23C

This is useful for the correlated System 3 disassembly: those internal call
addresses can now carry stable symbols rather than remaining raw destinations.

## Caller evidence

AS3000 System 3 contains a direct internal call to the A274 implementation at
runtime `0x004C85A4` (file `+0x0085A4`) without caller arguments.

A raw survey of several official SmartApplet images finds A274 in their linked
A-line stub tables, but did not establish an additional direct SmartApplet call
outside those tables. Current evidence therefore treats A274 primarily as a
System 3 internal UI service.

## Regression specification

The emulator-first regression should verify:

- the menu labels match the ROM-specific printer-record table;
- `+` follows the selected index and the initial choice is `index + 1`;
- raw event `0x48` preserves the selected index;
- raw event `0x40` commits `DialogGetChoice() - 1` on the NEO path;
- the AS3000 StyleWriter/dependency-warning path can block the state write;
- AS3000 exposes seven records while the compared NEO ROMs expose five;
- callers do not depend on a meaningful `D0` result.

This document specifies the regression; it does **not** claim that the
regression has already been executed.

## Naming decision

Upstream BetaWise and the comparative reverse-engineering material do not
recover a reliable modern public name for A274. `SYS_A274` is therefore retained
until primary evidence provides an original symbol. Mechanical closure does not
justify inventing one.
