# ABI trace — A3F0 / index252

Disposition: **MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / `RETORNO_CONTRACTUAL=DESCONOCIDO`**. The unknown return contract is an evidence boundary, not a human-blocking condition.

## Source-first correlation

Historical BetaWise `syscall.c` and `ioma8/neo-re` independently expose index 252 only as the neutral placeholder `SYS_A3F0`; neither recovers a vendor symbol or semantic prototype. No stronger public source anchor was found for this slot, so identity beyond `SYS_A3F0` is intentionally not invented.

## Primary mapping — freshly revalidated

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5EA6004E` | non-pointer / no callable contract |
| NEO 2005 | `0x83B2005E` | non-pointer / no callable contract |
| NEO 2013 | `0x00433F60` | callable handler, file offset `0x023F60` |

The three canonical ROM SHA-256 values were rechecked before analysis. A3F4 starts at `0x00433FE2`, fixing A3F0 at `0x82` bytes. The handler SHA-256 is `2410257e36a576815e9ab14986f50cc1077cea2e5df2c1038a8f22e99628141a`.

## ABI and helper mapping

A3F0 reads exactly four physical longword slots:

`[arg1_pointer, low8(arg2), low8(arg3), low8(arg4)]`

The handler derives an indexed record from `arg1+4` using stride `0x12C`. It invokes private helper `0x004337B0` with:

`[arg1+4+signext8(arg2)*0x12C+0x11E, signext8(arg3), signext8(arg4), selected_base]`

`selected_base` is the indexed record base, plus `0x1A` when the third byte is mechanically in the range 5..7. Sign-extension is implementation behavior only; this trace does not assign external signedness or semantic enums.

## Xrefs and callers

A fresh aligned NEO 2013 sweep finds exactly one direct absolute JSR to A3F0, at `0x0043E9FA`, with no direct JMP or BSR target. The immediate caller has two observed branch-specific frames:

- `(global[0x00012CA0], 0, 4, 1)`
- `(global[0x00012CA0], 0, 0, 0)`

After the JSR it cleans exactly four slots and immediately executes RTS, mechanically propagating D0 one level upward. That wrapper is itself reached by eleven BSR.W sites: `0x43E238`, `0x43E2E4`, `0x43E3E0`, `0x43E432`, `0x43E4D4`, `0x43E558`, `0x43E562`, `0x43E62A`, `0x43E6E8`, `0x43E72C`, and `0x43E79C`. No positive typed consumer was found that fixes A3F0's contractual return type.

## Adversarial return conclusion

D0 preservation does not establish a scalar return contract. Private helper `0x004337B0` has a null route that yields zero, while the non-null route later reloads only a byte into D0 and reaches RTS without full-width normalization. A coherent 32-bit scalar return therefore cannot be justified from the producer.

`void` remains a strong intent hypothesis from observed use, but is not promoted to CONFIRMADO. Accordingly no callable public prototype/header is emitted.

## Regression state

Fresh primary directed regression: **25/25 PASS**. It rechecks all three ROM hashes, all three slot values, A3F4 boundary, handler length/hash, ABI/helper byte anchors, exact direct-xref cardinality, caller cleanup/RTS, eleven wrapper BSR sites, and helper low-byte behavior.

Dynamic/emulator-first regression: **ESPECIFICADA / NO EJECUTADA**.
