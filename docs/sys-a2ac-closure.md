# SYS_A2AC closure

## Status

`A2AC` is closed under the current **source-first** methodology with confidence **A** for its ABI and platform-specific behavior. No vendor symbol or API name has been recovered.

```c
int32_t SYS_A2AC(uint8_t *out_byte);
```

The physical ABI uses one 32-bit pointer slot. The common path dereferences it without a null guard.

## Source-first correlation

Searches across the recoverable BetaWise, historical AS3000 material and `ioma8/neo-re` did not recover a symbol, prototype, caller, or named error corresponding to A2AC or signed `-1302`. That negative result is retained explicitly rather than replaced by a guessed hardware/API name.

Historical AS3000 material independently confirms that the platform used IrDA as an active transport, and the surrounding reconstructed family A28C-A2A8 is tied mechanically to IrDA/transport state. Separate source-first work on the serial/UART layer also establishes real AS3000-versus-NEO hardware-control differences in the UART/RS-232/IrDA path. Those facts make a transport-hardware cleanup/reset interpretation of A2AC's AS3000-only handshake **plausible**, but not nominally proven.

Accordingly, subsystem membership is kept as **strong contextual inference** only. The public contract does not rename A2AC as an IrDA reset/shutdown/status API, and `-1302` remains an unlabeled raw error value.

## Platform/generation difference

This block intentionally preserves a real AS3000-versus-NEO difference.

- AS3000 2005: handler `0x004E3F1C..0x004E3F8B`, length `0x70`.
- NEO 2005: handler `0x005E6890..0x005E68BB`, length `0x2C`.
- NEO 2013: handler `0x0043CCBA..0x0043CCE5`, length `0x2C`.

NEO 2005/2013 become identical after neutralizing five absolute relocations; normalized SHA-256:

`4f2b5537bd14ad5789eff5a0138ffa2150ea8d6cd6eaeb23d9d4eb0f5a603ddf`

AS3000 contains a hardware-specific preamble followed by a functional tail equivalent to the NEO implementation. After neutralizing relocations and compensating for AS3000's saved-D7 stack displacement, the common 0x2A-byte tail has canonical SHA-256:

`c1964a7f07d4c5cabb858aca37dd70d8066b3400e3a2ebceb6ea1309cfe1d338`

## Common behavior

1. An internal helper is called and its `D0.B` result is written to `*out_byte`.
2. A second helper is called with a single zero longword argument.
3. A shared longword is cleared.
4. A third helper is called.
5. A shared byte is cleared.
6. `D0` is fully defined as zero and the routine returns.

The three helper families are correlated by instruction position and side effects across ROMs, but their vendor names remain unknown.

## AS3000-only preamble

AS3000 saves D7 and performs a memory-mapped hardware handshake before the common path. It sets bit 1 in a control byte, calls an AS3000-only helper with argument 1, and examines bit 1 of a status byte.

If the adverse status condition remains set, A2AC clears the control bit, defines `D0.L = 0xFFFFFAEA` (signed `-1302`), restores D7 and returns before the common path. Therefore `*out_byte` is not guaranteed to have been written on this path.

If the status permits continuation, the control bit is cleared and execution enters the common path described above, ultimately returning 0.

NEO 2005/2013 do not contain this preamble and have no corresponding `-1302` path in the compared handlers.

## Caller evidence

No absolute JSR/JMP or direct A-line A2AC call was found inside the three canonical ROMs.

A raw scan of nine official SmartApplets — AlphaWord Plus, AlphaQuiz, Beamer, Calculator, ControlPanel, KeyWords, Responder, Wireless File Transfer and Accelerated Reader — finds exactly one A2AC opcode per binary. In every case it lies inside the contiguous A-line stub table (`A28C`, `A290`, ..., `A2A8`, `A2AC`, `A2B0`, ...), not in executable caller code. No additional official caller was established.

The one-pointer ABI is nevertheless direct firmware evidence: NEO reads `4(A7)` and AS3000 reads `8(A7)` only after saving one register, so both consume the same first caller slot.

## Adversarial checks

- A `void` return is rejected because AS3000 deliberately returns signed `-1302` on one path.
- A byte-sized return is rejected because that AS3000 path defines the full 32-bit D0 value.
- `out_byte` is not optional: the common path writes through it without a null check.
- AS3000 and NEO are not presented as byte-identical implementations; the hardware preamble is a real platform difference.
- The raw `-1302` value is documented without inventing a vendor error name.
- The source-first IrDA/transport context does not justify naming the handshake as reset/shutdown/transceiver-disable or assigning semantics to `out_byte`.

## Evidence classification

- **CONFIRMED:** one-pointer ABI; common helper/write/clear sequence; success return 0; AS3000-only hardware handshake and early `-1302`; no guaranteed output write on that early path; NEO absence of the error path; true cross-generation structural difference.
- **STRONG INFERENCE:** A2AC belongs to the same transport/hardware-control domain as the surrounding IrDA family.
- **UNKNOWN:** vendor symbol; meaning of `out_byte`; names of H0/H1/H2; hardware-bit semantics; vendor error name for `-1302`; precise protocol-layer role.

## Validation status

An emulator-first regression is **specified but not yet executed**. It covers the NEO common path, AS3000 normal handshake, AS3000 `-1302` early return, output-byte write/no-write behavior, helper side effects, full-width return semantics and cross-generation tail equivalence.

Full firmware bytes, hardware addresses and correlated workpapers remain private in Drive.
