# ABI trace — A3FC / index255

Status: **MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / `RETORNO_CONTRACTUAL=DESCONOCIDO`**. The unknown return contract and nominal semantics are evidence boundaries, not human-blocking conditions.

## Source-first correlation

Historical BetaWise `syscall.c` and `ioma8/neo-re` independently expose index 255 only as neutral placeholder `SYS_A3FC`; neither recovers a vendor symbol or semantic prototype. Accordingly the neutral name is retained.

## Primary mapping — freshly revalidated

- AS3000 2005: slot `0x5A06004E`, non-pointer/non-callable.
- NEO 2005: slot `0x7F08005E`, non-pointer/non-callable.
- NEO 2013: callable entry `0x0043409C`, file offset `0x02409C`.
- Neighbor A400 is `0x004343A2`, but is **not** the handler boundary: independent private functions lie between A3FC and A400.
- A3FC's first CFG has one terminal RTS at `+0x160`, fixing the handler at `0x162` bytes. The next private function begins immediately at `+0x162`.
- Handler SHA-256: `b039c5eb7824a83caa675ee779cf4ec5a02f85439509b883b81b85f20ad29bb8`.

## ABI and operation

Four physical longword slots are consumed as:

`[arg1_pointer, low8(arg2_index), low8(arg3_selector), low8(arg4_value)]`

The handler uses `record = arg1 + 4 + signext8(arg2) * 0x12C`. For selectors `0..7`, it updates one bit in `record+0x10D` using only `arg4 & 1`. Resolving the internal 8-word dispatch table gives the monotonic mapping:

`selector 0→bit7, 1→6, 2→5, 3→4, 4→3, 5→2, 6→1, 7→0`.

For selector `>7`, no indexed bit mutation occurs. Internal sign extension of arg2 is mechanical behavior only and is not promoted to public signedness.

## Callers and adversarial checks

A fresh aligned NEO 2013 sweep finds exactly two direct absolute JSRs to A3FC, at `0x0043E2DC` and `0x0043E33C`; no direct JMP or BSR.W targets were found. Their concrete call frames are respectively:

- `(global 0x00012CA0, 0, 5, 1)`
- `(global 0x00012CA0, 0, 5, 0)`

Thus the observed firmware pair sets and clears the same selected bit. Neither caller consumes D0 as an A3FC result before subsequent work overwrites or obscures it.

This directly refutes getter semantics, direct selector-as-bit-number semantics, and storage of the whole value byte.

## Return conclusion

D0 is not a coherent full-width scalar producer: selector and value are loaded with byte operations, the invalid-selector path preserves upper bits, and valid paths manipulate only byte state without a final full-width normalization. The two positive callers also ignore D0.

`void` is therefore an **INFERENCIA MUY FUERTE**, but remains unconfirmed without an independent prototype or typed consumer. No callable header is emitted.

## Regression state

Historical extended static regression: **36/36 PASS**.

Fresh primary directed regression: **26/26 PASS**, covering all three ROM hashes/slots, A400 neighbor, independently recovered handler boundary/hash, sole RTS, following private-function boundary, jump table, four ABI loads, stride/flag anchors, exact 2/0/0 direct JSR/JMP/BSR.W cardinality, both concrete caller frames, and absence of immediate contractual D0 consumption.

Dynamic/emulator-first regression: **ESPECIFICADA / NO EJECUTADA**.
