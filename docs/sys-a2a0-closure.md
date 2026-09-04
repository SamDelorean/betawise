# SYS_A2A0 closure

## Status

`A2A0` is closed under the current **source-first** methodology with confidence **A** for its ABI and mechanical behavior. No vendor symbol or API name has been recovered, so the neutral symbol `SYS_A2A0` is retained.

```c
uint8_t SYS_A2A0(uint8_t selector);
```

The physical caller ABI uses one 32-bit slot, but the handler consumes only the slot's low byte.

## Source-first correlation

Historical AS3000 material independently documents a state-sensitive IrDA LAP stack and an API whose discovery operation is only legal in the appropriate LAP state and returns byte-sized status codes such as media-busy and incorrect-state. Contemporary AlphaWord release notes independently confirm that IrDA was an operational AS3000 file-transfer transport.

That history is consistent with the already reconstructed A28C/A290/A294/A298/A29C/A2A0 cluster, its shared major-state checks, byte-sized status domain, indexed per-channel/per-entry storage, and synchronous polling. It therefore supports **IrDA/transport subsystem membership as a strong inference**.

It does **not** recover the original A2A0 symbol, the meaning of selectors `0..4`, or a vendor name for the stride-eight records. In particular, the constant `0x80` passed by A2A0 into A2A4 is not assigned a protocol name without an independent historical declaration.

## Cross-ROM evidence

The handler is exactly 0xC8 bytes in all three canonical System 3 images and ends immediately before `A2A4`:

- AS3000 2005: runtime `0x004C52AC`, file `+0x0052AC`
- NEO 2005: runtime `0x005C52CE`, file `+0x0052CE`
- NEO 2013: runtime `0x00415742`, file `+0x005742`

The literal handler SHA-256 values differ only because of relocated absolute operands. After neutralizing eight relocated references, the normalized SHA-256 is:

`3df3834353c58306dea0ce1d712be7c16d1d9cd56bc617b3c78b3d90132f3609`

No control-flow, constant, argument-layout or error-policy differences were observed between generations.

## Reconstructed behavior

1. A shared major-state byte must be `2` or `3`; otherwise the routine returns `0x15`.
2. `selector` must be less than `5`; otherwise it returns `0x16`.
3. The selector addresses a shared table as `table_base + selector * 8`. The first byte of that record must equal `1`; otherwise the routine returns `0x19`.
4. A cross-ROM internal helper is invoked before starting the operation. That helper waits for a shared busy/status byte to clear, with a three-tick byte-sized deadline and detection of major-state change. It returns `0` when the busy byte clears and `0x14` otherwise. `A2A0` maps any non-zero result from this helper to `0x20`.
5. Two shared parameter/state bytes are written with values `2` and `1`.
6. `A2A0` calls the implementation of `A2A4` with four physical arguments equivalent to `(0x80, selector, &global_param, 2)`. It tests only `D0.B`; a non-zero `A2A4` result is propagated.
7. After successful `A2A4`, the selected table record's first byte is cleared.
8. The routine then waits synchronously for the shared busy/status byte to clear while the major state remains `2` or `3`. The deadline is the current byte-sized tick plus 10.
9. A major-state change out of `2/3` returns `0x15`; an uncleared busy byte at the deadline returns `0x1B`; successful clearing returns `0` in `D0.B`.

The helper described above is 0x3E bytes in all three ROMs and has normalized SHA-256:

`6608ce47ba1a9ae43917d0dbe5b7e6fd96f2afe2a4f4c07e7cb49a42abede901`

## Callers

Exactly three absolute JSR callers were found per canonical ROM, with no absolute JMP callers:

- AS3000: `0x004E42B8`, `0x004E437E`, `0x004E447A`
- NEO 2005: `0x005E6B6E`, `0x005E6C34`, `0x005E6D30`
- NEO 2013: `0x0043CF98`, `0x0043D05E`, `0x0043D15A`

The callers pass concrete selector values `2`, `2`, and `1` respectively. Each call accounts for exactly one 32-bit stack slot. All three internal callers ignore the return value; the byte-sized return contract is established by the handler's deliberate `D0.B` comparisons, byte-only success definition, local status domain, and byte-only propagation from `A2A4`.

Searches for `A2A0` / `SYS_A2A0` in the historical BetaWise and `ioma8/neo-re` references did not recover an independent prototype or vendor name.

## Adversarial checks

The following alternatives were rejected from primary evidence:

- `A2A0` is **not** argument-free: all three caller families push one slot and the handler reads its low byte.
- The public selector is **not** 32-bit in meaning: upper bytes of the physical slot are never consumed.
- `0x14` from the pre-operation helper is **not** propagated; it is converted to `0x20` at this layer.
- A non-zero `A2A4` result **is** propagated rather than translated.
- The stride-8 table and selector are mechanically demonstrated, but their vendor semantics are not; no semantic name is published.
- The historical IrDA/LAP evidence does not justify renaming A2A0 as a disconnect/close/release primitive or assigning names to selectors.
- The A2A0-to-A2A4 interval is a complete function, not code-plus-data: all internal branches converge on the final register restore and `RTS` immediately before the A2A4 entry.

## Evidence classification

- **CONFIRMED:** one byte-valued selector argument; byte return; selector bound `<5`; state/precondition checks; stride-eight record access; pre-helper behavior; exact A2A4 invocation shape; record clearing; synchronous completion wait; three-generation equivalence.
- **STRONG INFERENCE:** membership in the IrDA/transport state machine shared with the surrounding A28C-A29C family.
- **UNKNOWN:** original vendor symbol; selector meanings; record names/fields beyond mechanically observed offsets; protocol meaning of A2A4 flag `0x80` in this call.

## Validation status

An emulator-first regression is **specified but not yet executed**. It covers invalid major states, selector bounds, inactive table records, pre-helper timeout/state change, A2A4 error propagation, table-state clearing, ten-tick completion timeout, tick wraparound, byte-only selector consumption and the byte-only return contract.

Full ROM bytes and correlated disassembly remain private workpapers in Drive.
