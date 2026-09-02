# A08C / index 35 — `StringCompare`

Status: source-first closure, firmware revalidated on the three canonical OS3K ROM generations.

## Public contract

```c
char StringCompare(const char *str1,
                   const char *str2,
                   int case_sensitive,
                   size_t num);
```

The historical SDK name and prototype are consistent with the firmware implementation.

- `case_sensitive != 0` compares raw bytes.
- `case_sensitive == 0` normalizes each byte through OS3K A160 before comparing. A160 performs ASCII uppercase-to-lowercase conversion and also contains an OS3K international-character mapping table.
- Equality returns `0`.
- A mismatch returns the low byte of the first differing `lhs - rhs`; the result is not normalized to `-1/0/+1`.
- Only the low byte of `num` is consumed by the firmware.
- `low8(num) == 0` selects comparison through the NUL terminator by loading an internal `0xFF` sentinel.
- Consequently `low8(num) == 0xFF` collides with that sentinel and also behaves as the unbounded/NUL-terminated mode.
- Finite counts therefore use the effective low-byte counter values `1..254`.

## Cross-generation behavior

The A08C handler body is structurally identical across AlphaSmart 3000 2005, NEO 2005, and NEO 2013 after normalizing the relocated A160 address. However, case-insensitive semantics are not identical over every 8-bit input because A160's international mapping data changed between the 2005 ROMs and NEO 2013. In particular, the mapping entry involving byte `0xA6` differs, so callers must not assume that every extended byte folds identically across generations.

## Evidence summary

The canonical firmware images match the project SHA-256 anchors. The A08C handlers are 0xB4 bytes at the independently recovered index-35 vectors, and their normalized SHA-256 is:

`85b2e7db50ff1a082a266eed4077806523b3fc5e24f6dd4eaa64b6ead538e305`

The helper used by A08C is the same A160 handler previously recovered independently at index 88. Its normalized code is identical across the three generations; the associated mapping data contains the generation-specific difference described above.

Official KeyWords applets provide positive semantic callers: five structural A08C calls in the 2005 applet and five in the 2012 applet. These callers pass `case_sensitive = 0`, exercise both bounded and NUL-terminated modes, and consume the return value as an equality predicate. AlphaWordPlus 2005 and NEO 2012 contain the A08C import veneer but no structural calls to it, providing reproducible negative caller results.

A private static regression derived from the confirmed handler/helper structure, canonical ROM hashes, cross-generation A160 behavior, synthetic contract cases, and the official applet xrefs completed **72/72 PASS**. Dynamic 68k probing was not executed and is not required for this static closure.

## Confidence

- Identity / SDK mapping: **CONFIRMED**
- Argument order and effective widths: **CONFIRMED**
- Return behavior: **CONFIRMED**
- `num` low-byte / `0xFF` sentinel behavior: **CONFIRMED**
- A160 dependency and ASCII folding: **CONFIRMED**
- Generation-specific extended-byte behavior: **CONFIRMED**
- Exact historical vendor intent for every international mapping entry: **not asserted**
