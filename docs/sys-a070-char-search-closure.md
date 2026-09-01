# A070 / index28 — SYS_A070

Status: source-first closure, confidence A.

BetaWise labels this syscall only as `SYS_A070` with the comment `strchr equivalent`. Direct firmware inspection across the three canonical ROMs shows that all three handlers are byte-identical and implement a NUL-terminated character search with a two-argument ABI compatible with:

```c
char *SYS_A070(const char *str, int c);
```

The first argument is a pointer loaded from `4(SP)`. Only the low byte of the second argument is consumed. The loop tests the current source byte for NUL before comparing it with the requested byte. If a non-NUL match is found, the matching pointer is returned; reaching the terminator returns `NULL`. There is no NULL-pointer guard for `str`.

## Important compatibility correction

`SYS_A070` is **not exactly ISO C `strchr`**. For `c == 0`, standard `strchr` returns a pointer to the terminating NUL. A070 checks the terminator before performing the comparison and therefore returns `NULL` instead. For nonzero search bytes, its behavior matches the usual first-occurrence search over a NUL-terminated string.

## Cross-generation result

The complete 30-byte handler is identical in AS3000 2005, NEO 2005, and NEO 2013. Functional behavior and the `c == 0` divergence are therefore common to all three audited generations.

The conservative public symbol remains `SYS_A070`; no stronger vendor name has been recovered.

Static regression: executed/pass. Dynamic probe: not executed.
