# A06C / index 27 — strlen-equivalent closure

Status: `CLOSED_MECHANICAL / REVALIDATED_SOURCE_FIRST`.

`os3k/syscall.c` labels index 27 conservatively as `SYS_A06C` with the historical comment `strlen equivalent`. No vendor symbol or public SDK prototype was recovered, so the public name remains `SYS_A06C`.

The three canonical firmware handlers are byte-identical and 0x12 bytes long. Their logic is straightforward:

1. Load the single pointer argument from `4(SP)`.
2. Preserve the starting pointer.
3. Advance byte-by-byte until the first NUL terminator.
4. Return `(end - start - 1)` in `D0`.

Therefore the mechanically confirmed contract is:

```c
size_t SYS_A06C(const char *str);
```

with semantics identical to `strlen()` for a valid NUL-terminated C string. There is no NULL guard and no maximum-length bound in the handler.

The handler SHA-256 is identical in AlphaSmart 3000 2005, NEO 2005, and NEO 2013: `46193c2eda4c7b66d5a1fd34b8e73773c52f87f49d0e60439c29c9daa95798f5`.

Validated AlphaWordPlus veneer locations are A06C at `0x122A0` (2005) and `0x12DA8` (NEO 2012). Structural call-resolution using the established `MOVEA.L #disp32,A0` + `JSR (-2,PC,A0.L)` pattern found no executable xrefs to those veneers in either binary; this is recorded as a negative caller search, not as evidence that the syscall is unused globally.

Static regression was executed and passed for canonical ROM hashes, vector location, exact handler bytes/hash, argument load, NUL loop, return calculation, cross-generation identity, and the AlphaWordPlus negative xref search. Firmware bytes and extended disassembly remain private.
