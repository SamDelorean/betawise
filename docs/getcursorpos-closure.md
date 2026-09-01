# A008 / `GetCursorPos` closure

Status: **mechanical confidence A** for the System 3 ABI.

## Contract

```c
void GetCursorPos(uint8_t* row, uint8_t* col);
```

Both pointers are mandatory writable outputs. `row` receives one byte of the OS3K logical cursor row and `col` receives one byte of the logical cursor column. The service does not validate null pointers and has no contractual return value.

## Firmware evidence

A008/index 2 resolves in the three canonical System 3 generations to:

- AS3000 2005: runtime `0x004CD6E4`, length `0x16`.
- NEO 2005: runtime `0x005D11D6`, length `0x16`.
- NEO/System 3.15 2013: runtime `0x00422156`, length `0x16`.

All three handlers have the same five-instruction skeleton, apart from relocated global addresses:

1. load the first pointer from stack slot 1;
2. copy the byte-sized row global through it;
3. load the second pointer from stack slot 2;
4. copy the byte-sized column global through it;
5. return.

The handlers only read cursor-state globals and write the two caller-provided bytes. They do not alter cursor position, cursor mode or LCD hardware. They do not synthesize a return in `D0`; the historical `void` prototype therefore governs the callable contract.

## Source genealogy

Historical SDK/debug material declares the same two-pointer prototype. The early AS3000 `LCDModule.obj` independently retains `LCDGetCursorPosition`; its implementation also writes the row and column globals through two pointer outputs. Its classic 4x40 debug assertions are historical implementation detail, not a range-validation promise of the later A008 handler.

## Official callers

Structural xref resolution against official AlphaWord Plus binaries finds:

- 8 executable references to the A008 veneer in AlphaWord Plus 2005;
- 8 executable references in AlphaWord Plus NEO 2012.

Concrete callers pass two local addresses with `PEA d16(SP)`, consistent with the two output-pointer ABI. This evidence is used in addition to, not instead of, the direct firmware proof.

## BetaWise layer

`syscall.c` exports A008 directly as `GetCursorPos`; there is no additional wrapper in `os3k.c`. BetaWise's extended-font path maintains its own `g_CurRow/g_CurCol` state and deliberately avoids relying on A008 there, so the firmware service must not be described as returning BetaWise-private extended-font cursor state.

## Regression

A dedicated static regression was **executed** against the three canonical ROMs and two official AlphaWord Plus generations: **33/33 PASS**, exit 0, empty stderr. It checks ROM and handler identity, table resolution, exact handler skeleton, output order/width, cross-ROM normalization, applet identity, A008 veneers, structural xref counts and concrete two-pointer callers.

A dynamic/emulator probe remains **specified / not executed** and is additional validation rather than a blocker for this static mechanical closure.

Full handler hashes, reproducible disassembly listings and regression hashes remain in the private Drive workpaper. Firmware bytes are intentionally not published here.
