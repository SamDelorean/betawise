# A00C / `PutStringCentered` closure

Status: **mechanical confidence A** for the System 3 ABI.

## Contract

```c
void PutStringCentered(uint8_t row, const char* str);
```

`str` must point to a valid NUL-terminated string. The routine computes a starting column from the active character-column width, positions the cursor on `row` with cursor mode `SHOW`, then writes the string through the A014 raw-string service. No portable return value is contractual.

For the compared System 3 implementations the starting column is:

```text
((column_width - strlen(str)) >> 1) + 1
```

The operation assumes a string length compatible with the available row width. There is no NULL guard or overlength clamp in A00C itself.

## Source genealogy

The early AS3000 `LCDModule.obj` independently retains `LCDCenterStringInRow` and `LCD_StringLength`, providing strong name and algorithm genealogy. That early implementation uses the same conceptual sequence—measure, calculate a centered column, move the cursor with mode `0x0F`, then write—but its arithmetic predates the System 3 `+1` adjustment. The historical object is therefore supporting genealogy, not substituted for the later firmware proof.

## Firmware evidence

A00C/index 3 resolves to:

- AS3000 2005: runtime `0x004CD4CC`, length `0x44`.
- NEO 2005: runtime `0x005D0EA0`, length `0x46`.
- NEO/System 3.15 2013: runtime `0x00421E24`, length `0x46`.

The three handlers were re-extracted from canonical ROMs and disassembled reproducibly with zero unknown opcodes. They agree on argument order and the core algorithm. AS3000 uses a fixed width of 40 columns. NEO reads its current maximum-column byte, derived from the active system-font geometry; the default 6x16 font is explicitly normalized to 40 columns.

The handler consumes `row` as a byte and `str` as a pointer, calls an internal NUL-string length helper, computes the centered column, invokes A004/`_OS3K_SetCursor(row, col, CURSOR_MODE_SHOW)`, then invokes A014/`PutStringRaw(str)`.

## Official callers

Structural xref resolution against official AlphaWord Plus binaries finds five executable A00C callers in the 2005 generation and five in the NEO 2012 generation. Concrete call sites push a string pointer followed by an immediate row; the observed row values cover 1 through 4. This independently confirms the `row, str` order and normal UI usage.

## Cross-generation differences

The callable contract is stable, but two implementation details are deliberately not generalized:

- the early 2000 source object's centering arithmetic omits the later System 3 `+1`;
- AS3000 System 3 hard-codes 40 columns while NEO uses a runtime maximum-column value.

Neither difference changes the public two-argument ABI.

## Regression

The first version of the static harness reported 29/32 because the **test itself** resolved `BSR.W` using the PC after the extension word instead of the 68000 branch base after the opcode. This affected only the three assertions that classified the A004 target; it did not contradict firmware bytes or the disassembly. After correcting the branch-base calculation, the dedicated regression was **executed 32/32 PASS**, exit 0, empty stderr.

A dynamic/emulator probe remains **specified / not executed** and is additional validation rather than a blocker for the static mechanical closure.

Full handler hashes, disassembly listings and regression hashes remain in the private Drive workpaper. Firmware bytes are intentionally not published here.
