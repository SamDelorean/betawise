# A164 / index89 — TranslateKeyToChar — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
char TranslateKeyToChar(KeyMod_e key);
```

`TranslateKeyToChar` is the public A-line syscall at index 89 (`A164`). BetaWise exposes the historical signature above, and the canonical firmware confirms a single `KeyMod_e` argument in one 32-bit ABI slot with the translated character returned in `D0.B`.

## Confirmed behavior

The routine is more than a direct keycode table lookup:

1. Reject physical keycodes above `0x50` with character `0`.
2. Normalize two modifier-pair masks (`0x0C00` and `0x3000`) before translation. The present public `KeyMod_e` header does not name every constituent bit, so no missing vendor name is invented here.
3. Give a private layout/modifier helper the first opportunity to translate the normalized key.
4. Otherwise index the key's character record. Records contain a base character, a shifted character, then zero-terminated `(modifier signature, character)` byte pairs for additional combinations.
5. Apply the firmware's private uppercase mapping where Caps Lock semantics require it. This mapper includes the AlphaSmart extended character set and is distinct from the byte-lowercase/case-fold service at `A160`.
6. Return `0` when no valid character mapping exists.

Examples confirmed from the NEO mapping records include ordinary base/shift pairs such as `a/A`, `1/!`, space and carriage return. The Applets key has no normal character in this table.

## Layout correlation

The independent `ioma8/neo-re` layout-patcher targets the canonical NEO 2013 `A164` entry and patches a character-override hook that returns to the live translation path. This independently corroborates the recovered role of `A164` as the layout-sensitive key-to-character routine.

## Generation comparison

AS3000 2005 and NEO 2005 have the same semantic body after the character-record table pointer is relocated.

NEO 2013 adds one real pre-translation gate. It checks a runtime-selected applet header for ID `0xA002`; `neo-re` independently identifies `0xA002` as Calculator. When that applet is selected, firmware bypasses the private layout helper and proceeds directly to the generic character-record lookup. Other states follow the common layout-helper path. The exact vendor name of the runtime selector remains unknown.

After removing only this NEO-2013-specific gate and normalizing relocations/branch-call displacements, all three canonical bodies normalize identically.

## Callers

Direct absolute firmware callers found during the private audit:

- AS3000 2005: 4
- NEO 2005: 4
- NEO 2013: 6

The callers construct a full ABI slot from the `KeyMod_e` word and consume the result as a byte. Existing TextBox and getchar paths corroborate the public semantics: zero means no character; nonzero is consumed as translated text input.

A previously documented `BACKSPACE 0x03 -> 0x08` conversion in the getchar path is deliberately **not** attributed to `A164`: that conversion is performed by the caller.

## Regression status

Static regression: **EXECUTED — 61/61 PASS**.

It verifies canonical ROM hashes, vector entries, handler boundaries, argument/return mechanics, modifier masks, layout-helper and uppercase-helper relationships, caller/reference counts, NEO-2013 applet gate, representative character records and normalized three-generation equivalence.

Dynamic keyboard/layout regression: **SPECIFIED / NOT EXECUTED**. A future probe should sweep keycodes, modifier combinations and layouts on AS3000 and NEO and explicitly preserve the NEO-2013 Calculator-specific behavior.

## Confidence

- **CONFIRMED:** identity, public ABI, byte return, keycode bound, modifier normalization, character-record format, Caps Lock postprocessing, caller behavior and the NEO-2013 `0xA002` gate.
- **STRONG INFERENCE:** the private helper reached before generic lookup is the firmware's layout/modifier-special translation path; the NEO-2013 global used by the gate is a runtime/current-applet selector.
- **UNKNOWN:** original vendor names for the private helpers, the source-level name of the unnamed `0x2000` modifier bit and the exact vendor name of the NEO-2013 runtime selector.

Raw firmware, full disassembly, ROM offsets and private audit corpus remain outside the public repository.