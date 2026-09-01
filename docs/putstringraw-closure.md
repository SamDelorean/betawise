# A014 / `PutStringRaw` closure

Status: **mechanical confidence A** for the System 3 ABI.

## Contract

```c
void PutStringRaw(const char* str);
```

`str` must point to a valid NUL-terminated byte string. The handler emits every nonzero byte by calling A010 / `PutChar` and stops at the first `0x00`. The empty string emits nothing. A null pointer is not safe because the firmware dereferences the input immediately. No portable return value is contractual.

## Source genealogy

The System 3 SDK/BetaWise declarations retain the `PutStringRaw` identity and the same one-pointer prototype. Earlier AS3000 LCD source contains both a non-NUL `LCDSetString` convention and an explicitly NUL-terminated `LCDSetCString` path. That historical split was treated only as genealogy: the System 3 firmware itself resolves the modern contract in favor of NUL termination.

## Firmware evidence

A014 / index 5 resolves in the three canonical ROMs to:

- AS3000 2005: runtime `0x004CD4B0`, 0x1C bytes / 12 instructions.
- NEO 2005: runtime `0x005D0E84`, 0x1C bytes / 12 instructions.
- NEO/System 3.15 2013: runtime `0x00421E08`, 0x1C bytes / 12 instructions.

The three handlers are structurally equivalent. Each saves A4, loads the single pointer argument, tests the current byte, zero-extends one byte into D0, pushes one 32-bit ABI slot, calls the generation's A010 / `PutChar`, removes that slot, and repeats until NUL.

There is no length argument, pre-scan, character-set conversion, special control-code handling, or NULL guard in A014 itself. Rendering and cursor side effects are therefore exactly those produced by the corresponding A010 implementation, whose internal behavior differs between AS3000 and NEO.

## Official callers

Structural xref resolution against official AlphaWord Plus binaries finds 111 executable references to the A014 veneer in the 2005 build and 114 in the NEO 2012 build. This independently confirms A014 as a heavily used string-output service rather than a coincidental byte pattern.

## Refutation checks

The historical non-NUL `LCDSetString` convention does **not** carry forward to A014: the modern firmware explicitly terminates on byte zero. The handler also refutes a specialized bulk-string helper, since it delegates character by character to A010. No NULL-safe path or meaningful return contract is present.

## Regression

A dedicated static regression was **executed 33/33 PASS**, exit 0, empty stderr. It checks canonical ROM identity, index-5 handler resolution, handler shape, single-pointer argument loading, NUL termination, byte zero-extension, exact A010 delegation and the official 111/114 caller census.

A dynamic/emulator probe remains **specified / not executed** and is additional validation rather than a blocker for static mechanical closure.

Full handler hashes, long disassembly listings, regression source/output and firmware bytes remain private in Drive.
