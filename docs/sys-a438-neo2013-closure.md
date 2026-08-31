# SYS_A438 — NEO 2013 mechanical closure

Status: **MECÁNICA_CERRADA A / PUBLICADO**.

`A438` / index 270 is callable only in the NEO 2013 vector among the three canonical OS3K generations examined. The equivalent AS3000 2005 and NEO 2005 slots contain non-pointer/unresolved values and therefore do not receive a callable contract.

## Reconstructed contract

```c
char *SYS_A438(char *dst);
```

The NEO 2013 handler is a distinct 0x16-byte wrapper at runtime `0x0043496A` (file `+0x02496A`). It creates a second argument pointing at firmware-owned storage at `$00012C70`, then invokes the already reconstructed NEO 2013 `strcpy` handler at `0x0043672A`.

The stack order is decisive: `$00012C70` is pushed first, then the caller's sole external pointer slot is pushed. Therefore the helper receives `external_arg1` as `dst` and `$00012C70` as `src`. Mechanically, A438 performs:

```c
return strcpy(dst, (const char *)0x00012C70);
```

Because the `strcpy` helper returns the original destination in `D0.L` and A438 does not modify `D0` after the call, A438 returns exactly `dst` as a full pointer value.

No capacity/count argument, NULL check, bounds check, or alternate copy path exists in this wrapper. The semantic meaning and capacity of the firmware-owned source buffer are not recovered, so no vendor/function name beyond neutral `SYS_A438` is assigned.

## Evidence summary

Static regression was executed directly against all three canonical ROMs and completed **24/24 PASS**. It verifies the three ROM SHA-256 values, slots 269–271, A438 body bytes/hash, the known `strcpy` helper bytes/hash, stack ordering, cleanup, return propagation, and a full direct JSR/JMP/BSR scan.

Direct firmware xrefs to A438 in NEO 2013: **0**. The absolute address `$00012C70` has three aligned absolute references in NEO 2013, including A438, but those references are not sufficient to assign a semantic name to the shared state.

Dynamic regression was not executed; the mechanical contract is determined by exact wrapper bytes plus the independently reconstructed `strcpy` contract.
