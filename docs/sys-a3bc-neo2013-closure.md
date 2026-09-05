# A3BC / index 239 — source-first generational closure

Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**.

`SYS_A3BC` remains a project-neutral label. No independently reproducible vendor symbol or semantic name for the backing byte has been recovered, so the public contract is intentionally width-only.

## Platform availability

Fresh revalidation against the canonical ROMs confirms:

- AlphaSmart 3000 (2005): index 239 contains `0x83848788`, not a demonstrated code pointer.
- NEO (2005): index 239 contains `0x83848788`, not a demonstrated code pointer.
- NEO (2013): index 239 points to runtime `0x0043DACA` / file `0x02DACA` and has a real handler.

No callable A3BC contract is asserted for the 2005 ROMs, and the marker is not assigned `reserved`/`unimplemented` vendor semantics.

## NEO 2013 raw ABI

The complete A3BC handler reads no stack arguments. It delegates to a private byte getter and returns immediately. A direct firmware caller independently establishes the external width by executing `TST.B D0` immediately after the call.

```c
/* NEO 2013 only; vendor name and byte semantics unknown. */
uint8_t SYS_A3BC(void);
```

This is a width-preserving raw declaration. The private getter writes only `D0.B`; upper `D0` bits are residual and are not part of the demonstrated contract.

## Handler, backing state, and callers

Fresh extraction reproduces the exact 8-byte handler:

`JSR 0x0043E86E ; RTS`

Handler SHA-256:

`cca0b14256241ab7c00bc1a88c952d1d044674466f3a74afd5823b34ffdea792`

The delegated getter is also exactly 8 bytes and reads one mutable global byte into `D0.B`. Getter SHA-256:

`dda45819112d6ce88c5b5984fa875ebd64a09a67b00839deaa4fa75b63a26bfe`

Fresh firmware xref scanning reproduces exactly one direct absolute JSR to A3BC in NEO 2013. The immediate sequence is `JSR A3BC ; TST.B D0 ; BNE.W ...`, confirming byte-sized caller consumption independently of the helper implementation.

Fresh references to the backing address reproduce a `MOVE.B #1` writer, a `CLR.B` writer, and the getter. That proves mutable byte-sized storage, but it is insufficient to label the value as a boolean, status flag, subsystem state, or vendor concept.

The existing full SmartApplet regression remains **EJECUTADA / PASS**: 41/41 corpus classification, including 16 extended-table applets, 14 legacy tables, and 11 structural negatives; no executable A3BC SmartApplet caller was found. A fresh directed source-first regression covering canonical slots, handler/helper bytes and hashes, firmware caller width consumption, and global byte references is also **EJECUTADA / PASS**. Dynamic/emulator regression remains **ESPECIFICADA / NO EJECUTADA**.

## Confidence classification

- **CONFIRMADO:** callable implementation only in NEO 2013 among the three canonical ROMs.
- **CONFIRMADO:** zero physical arguments.
- **CONFIRMADO:** external return contract is `D0.B` only.
- **CONFIRMADO:** backing storage is one mutable byte with observed zero/one writers.
- **DESCONOCIDO:** vendor function name and semantic meaning of the backing byte.

## Adversarial conclusions

- Rejected: interpreting `0x83848788` as a valid 2005 handler pointer.
- Rejected: publishing `uint16_t`/`uint32_t` from residual upper `D0` bits.
- Rejected: inventing implicit parameters; the complete handler accesses none.
- Rejected: promoting observed zero/one writes to a boolean/status/vendor name.

Full ROM bytes, private disassembly, exact global-reference details, corpus hashes, and regression workpapers remain private in Drive.
