# SYS_A2A8 closure

## Status

`A2A8` is closed under the current **source-first** methodology with confidence **A** for its ABI and mechanical behavior. No vendor symbol or API name has been recovered, so the neutral symbol is retained.

```c
uint8_t SYS_A2A8(uint32_t context,
                 const uint8_t *payload,
                 uint8_t payload_size);
```

The physical ABI uses three 32-bit caller slots. `context` is consumed as a full longword; `payload` is a pointer; only the low byte of the third slot is consumed as `payload_size`.

## Source-first correlation

Historical AS3000 material independently establishes a state-sensitive IrDA LAP stack and operational IrDA transfers. In the reconstructed firmware family, A28C establishes IrDA/IrCOMM/IrLMP/TinyTP state, A2A4 is the shared payload/request constructor, and A2A8 invokes A2A4 specifically as `(0, 2, payload, payload_size)` before storing an opaque context and waiting for a neighboring completion path.

That correlation supports A2A8 as a **higher-level synchronous request/transaction primitive inside the same IrDA/transport machinery**. The conclusion is stronger than vector adjacency because it rests on an explicit A2A8→A2A4 call, shared state/result globals, and a distinct completion routine that writes the result and clears the pending state.

No historical source recovered the vendor symbol, meaning of selector `2`, meaning of `context`, semantic names for result values `1`/`2`, or the protocol interpretation of `payload[0] & 0x7F`. Those remain unknown and are not promoted into the public API.

## Cross-ROM evidence

The handler is exactly 0x84 bytes and ends at the first common register-restore/RTS:

- AS3000 2005: `0x004C5780..0x004C5803`
- NEO 2005: `0x005C57A2..0x005C5825`
- NEO 2013: `0x00415C16..0x00415C99`

After neutralizing seven absolute relocations, all three bodies are byte-identical with normalized SHA-256:

`e03899336d3f7a785fb071942c6dd73025753640c7282fad3b857df7c1f9eb96`

No control-flow, constant, stack-layout or timeout differences were observed between generations.

## Reconstructed behavior

1. A shared operation-state byte must be zero; otherwise A2A8 returns `0x19`.
2. A second shared gate byte must equal 1; otherwise it returns `0x19`.
3. `payload_size` must be non-zero; zero returns `0x19`.
4. A shared result byte is initialized to 1.
5. A2A8 calls A2A4 with arguments equivalent to `(0, 2, payload, payload_size)`.
6. A non-zero A2A4 result is propagated through `D0.B` without translation.
7. After A2A4 succeeds, the full `context` longword is stored in shared state. A2A8 itself does not dereference that value, so the public contract leaves it opaque rather than declaring it a pointer.
8. The shared operation-state byte is set to `payload[0] & 0x7F`.
9. A byte-sized deadline of current tick + 10 is formed and the routine waits while the operation-state byte remains non-zero.
10. If the deadline is reached first, the operation-state byte is cleared and A2A8 returns `0x1B`.
11. If another internal route clears the operation state before the deadline, A2A8 returns the shared result byte.

A neighboring internal function references the same context/state/result globals, writes the result byte from processed input, and clears the operation-state byte. This independently confirms the completion relationship without establishing a vendor API name.

## Callers

Exactly two absolute JSR sites were found per canonical ROM, with no absolute JMP sites:

- AS3000: `0x004E4248`, `0x004E426C`
- NEO 2005: `0x005E6AFE`, `0x005E6B22`
- NEO 2013: `0x0043CF28`, `0x0043CF4C`

The families are instruction-equivalent after relocation. They pass `A6` as the full-width context, constant payload pointers, and concrete payload sizes `0x21` or `0x1A`. Each caller removes exactly 12 bytes after the call and copies `D0.B` into a byte register. One caller compares results against both 1 and 2, which independently rejects a Boolean return interpretation.

No direct A-line A2A8 opcode was found inside the compared ROMs. Searches in BetaWise/neo-re did not recover an independent prototype or vendor name.

## Adversarial checks

- A2A8 is not argument-free: handler and callers agree on three slots.
- `context` is not published as a pointer: A2A8 only stores it and primary evidence does not show A2A8 dereferencing the supplied value.
- `payload` is not nullable: A2A8 reads `payload[0]` without a null guard.
- `payload_size` is byte-sized in meaning, not 32-bit, and zero is rejected.
- The return is not Boolean: A2A4 errors are propagated, timeout yields `0x1B`, and normal completion returns a shared result byte for which callers explicitly recognize values 1 and 2.
- Source-first correlation does not justify renaming selector `2`, the context value, payload byte 0, or result values 1/2.
- Names such as connect/request/open-session/IrCOMM-negotiate remain unsupported and are not published.

## Evidence classification

- **CONFIRMED:** three-argument ABI; full-width context storage; pointer payload; byte payload_size; preconditions; exact A2A4 `(0,2,...)` call; `payload[0] & 0x7F`; completion/timeout behavior; shared result byte; two callers per ROM; byte return; cross-ROM equivalence.
- **STRONG INFERENCE:** synchronous higher-level transaction/request primitive in the same IrDA/transport state machine as A28C/A2A4.
- **UNKNOWN:** vendor symbol; context semantics; selector-2 semantics; payload format; meanings of result values 1/2; protocol-layer identity.

## Validation status

An emulator-first regression is **specified but not yet executed**. It covers all preconditions, A2A4 error propagation, full-width context storage, `payload[0] & 0x7F`, ten-tick completion and timeout including byte wraparound, asynchronous result/state completion, byte-only return behavior, and cross-ROM equivalence.

Full firmware bytes and correlated workpapers remain private in Drive.
