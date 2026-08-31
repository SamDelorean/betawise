# ABI trace — A3F0 / index252

Disposition: **MECÁNICA_RECONSTRUIDA A / BLOQUEADO only in `RETORNO_CONTRACTUAL=DESCONOCIDO`**.

## Primary mapping

| Generation | A-line slot | Disposition |
| --- | --- | --- |
| AS3000 2005 | `0x5EA6004E` | non-pointer / no callable contract |
| NEO 2005 | `0x83B2005E` | non-pointer / no callable contract |
| NEO 2013 | `0x00433F60` | callable handler, file offset `0x023F60` |

A3F4 begins at `0x00433FE2`, fixing A3F0 at `0x82` bytes. The handler decodes to 45 instructions with one terminal RTS and SHA-256 `2410257e36a576815e9ab14986f50cc1077cea2e5df2c1038a8f22e99628141a`.

## ABI and helper mapping

A3F0 receives four physical longword slots:

`[arg1_pointer, low8(arg2), low8(arg3), low8(arg4)]`

The handler derives an indexed record from `arg1+4` with stride `0x12C`. It invokes private helper `0x004337B0` with:

`[arg1+4+signext8(arg2)*0x12C+0x11E, signext8(arg3), signext8(arg4), selected_base]`

where `selected_base` is the indexed record base, plus `0x1A` when the third byte lies mechanically in the range 5..7. The sign-extension is implementation behavior only; this trace does not assign external signedness or semantic enums.

## Xrefs

A full aligned NEO 2013 sweep finds one direct absolute JSR to A3F0, at `0x0043E9FA`. Its two branch-specific call frames are:

- `(global[0x00012CA0], 0, 4, 1)`
- `(global[0x00012CA0], 0, 0, 0)`

The caller cleans exactly four slots and immediately executes RTS, so D0 is mechanically propagated one level higher. That wrapper has eleven direct BSR.W xrefs at `0x43E238`, `0x43E2E4`, `0x43E3E0`, `0x43E432`, `0x43E4D4`, `0x43E558`, `0x43E562`, `0x43E62A`, `0x43E6E8`, `0x43E72C`, and `0x43E79C`. None provides positive explicit evidence that fixes A3F0's contractual return type.

## Adversarial return conclusion

D0 preservation alone is not sufficient here. Private helper `0x004337B0` has a null route yielding zero, but on its non-null route it reloads only a low byte after deeper calls and reaches RTS without normalizing the upper 24 bits. A coherent full-width scalar return therefore cannot be inferred from the producer.

The observed usage makes `void` a strong intent hypothesis, but not a confirmed contract. A3F0 therefore remains blocked only on contractual return type/intent.

Static regression: **23/23 PASS**. Dynamic regression: **not executed**.
