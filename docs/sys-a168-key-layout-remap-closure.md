# A168 / index90 — `SYS_A168` — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered raw contract

```c
uint8_t SYS_A168(uint8_t raw_key_index);
```

The original vendor name has not been recovered, so the neutral `SYS_A168` identifier is retained. Semantically, the returned byte is in the OS3K physical/logical key-code domain (`Key_e` values plus `0xFF` / `KEY_NONE`), but the public signature remains byte-typed until an original typedef for this syscall is recovered.

## Confirmed behavior

The AS3000 2005, NEO 2005 and NEO 2013 firmware handlers have the same instruction body after normalizing only relocated table/state addresses.

The handler:

1. consumes the low byte of the first 32-bit ABI argument slot;
2. returns `0xFF` when the input is greater than `0x7F`;
3. uses the input as an index into a 128-byte first-stage table;
4. reads the current keyboard-layout state byte;
5. when the layout state is `3`, returns the first-stage result directly;
6. otherwise indexes a second-stage table as `3 * intermediate + layout`;
7. returns the resulting byte in `D0.B`.

There are no helper calls in the public handler and no explicit layout-range check other than the special `layout == 3` path.

## Keyboard-layout correlation

BetaWise historically comments A168 as converting a key according to keyboard layout. Firmware independently confirms that characterization: a live layout-state byte directly changes the second-stage remap.

Official AlphaSmart documentation describes four user-facing keyboard layouts: QWERTY, Dvorak, Right and Left. This closure deliberately does **not** assign those names to internal values `0..3`, because A168 alone does not establish the numeric correspondence.

## Callers and ABI

Each canonical firmware has two direct absolute callers. Both promote an input byte into a longword ABI slot and consume only the low return byte. One caller explicitly compares the result with `0xFF`, independently corroborating the invalid/no-key sentinel.

## Generation differences

The code path is equivalent across all three canonical generations, but the mapping data is not fully identical:

- NEO 2005 and NEO 2013 use an identical first-stage 128-byte table.
- AS3000 differs from NEO at one first-stage input position.
- NEO 2005 and NEO 2013 have seven byte differences in their second-stage remap tables.

An emulator therefore should preserve generation-specific mapping data rather than treating A168 as one universal table.

## Relationship to `TranslateKeyToChar`

A168 is **not** an alias or wrapper for `TranslateKeyToChar` (`A164`). `A164` has its own public vector and performs character translation with modifier/layout processing. A168 is a lower-level, layout-dependent key-index remap in the keyboard-input pipeline.

## Regression status

Static regression: **EXECUTED — 80/80 PASS**.

It verifies the three canonical ROM hashes, vector resolution, complete public-handler grammar, argument/return width, invalid sentinel, both table stages, layout-dependent branch, direct caller counts, normalized three-generation handler equivalence, and known generation-specific table differences.

Dynamic keyboard-layout regression: **SPECIFIED / NOT EXECUTED**. A future probe should sweep raw inputs `0..127` under all four layouts on AS3000 and NEO, and separately verify that inputs above `127` return `0xFF`.

## Confidence

- **CONFIRMED:** index90/A168 identity, one byte argument, low-byte return, `>0x7F -> 0xFF`, first-stage 128-entry lookup, layout-state dependency, `layout==3` bypass, stride-3 second-stage lookup, two direct callers per generation, code equivalence 3/3, and real generation-specific table differences.
- **STRONG INFERENCE:** conceptual role as raw/physical keyboard index to logical OS3K key-code remap controlled by keyboard layout.
- **PROVISIONAL:** byte-typed public signature pending recovery of any original vendor typedef.
- **UNKNOWN:** original vendor function name and the exact mapping of internal layout values `0..3` to QWERTY/Dvorak/Right/Left.

Raw firmware, complete tables, offsets and disassembly remain in the private audit corpus.