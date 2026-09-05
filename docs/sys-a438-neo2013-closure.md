# SYS_A438 — NEO 2013 mechanical closure

Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**.

`A438` / index 270 is callable only in the NEO 2013 vector among the three canonical OS3K generations examined. The equivalent AS3000 2005 and NEO 2005 slots contain non-pointer/unresolved values and therefore do not receive a callable contract.

## Reconstructed contract

```c
char *SYS_A438(char *dst);
```

The NEO 2013 handler is a distinct 0x16-byte wrapper at runtime `0x0043496A` (file `+0x02496A`). It creates a second argument pointing at firmware-owned storage at `$00012C70`, then invokes the independently source-first-revalidated NEO 2013 `strcpy` handler at `0x0043672A`.

The stack order is decisive: `$00012C70` is pushed first, then the caller's sole external pointer slot is pushed. Therefore the helper receives `external_arg1` as `dst` and `$00012C70` as `src`. Mechanically, A438 performs:

```c
return strcpy(dst, (const char *)0x00012C70);
```

Because the `strcpy` helper returns the original destination in `D0.L` and A438 does not modify `D0` after the call, A438 returns exactly `dst` as a full pointer value.

No capacity/count argument, NULL check, bounds check, or alternate copy path exists in this wrapper. The semantic meaning and capacity of the firmware-owned source buffer are not recovered, so no vendor/function name beyond neutral `SYS_A438` is assigned.

## Source-first revalidation — 2026-09-05

Correlation was performed before accepting the old mechanical result. Historical BetaWise and `ioma8/neo-re` independently map syscall index 270 only to the neutral placeholder `SYS_A438`; neither provides a vendor name or semantic prototype. The public OS3K header material does not provide an A438 prototype. The helper identity is not inferred from similarity: A38C/`strcpy` was independently closed under the source-first audit and the helper bytes used by A438 reproduce that handler exactly.

All three canonical ROMs were freshly materialized and their complete SHA-256 values reproduced before extraction. The NEO 2013 A438 body was re-extracted at `0x0043496A`, length `0x16`, SHA-256 `e1dce54eff2e86dbf882576bdea0a01f636575281e30faaa41728242a8c87f42`. A deterministic fresh 68k decode reproduces the six-instruction wrapper, including the single external full32 slot, the two-slot call to `strcpy`, cleanup of eight bytes, and no write to `D0` after the helper returns.

The generation comparison was also reproduced: AS3000 2005 and NEO 2005 do not expose a callable handler at index 270; NEO 2013 does. A fresh aligned firmware scan finds zero direct `JSR`/`JMP`/`BSR` xrefs to A438. `$00012C70` has exactly three aligned absolute references in the NEO 2013 ROM, one in A438; this is enough to establish shared firmware-owned state but not enough to name that state.

Adversarial checks reject the reverse copy direction because of the actual stack order and reject a `void` return because the independently reconstructed `strcpy` result (`dst` in `D0.L`) is propagated unchanged. A semantic/vendor name for A438 or for `$00012C70` remains unsupported.

The earlier comprehensive static regression remains **EJECUTADA / 24/24 PASS**. A fresh source-first directed primary regression executed on 2026-09-05 completed **21/21 PASS**. Dynamic regression remains **ESPECIFICADA / NO EJECUTADA**.
