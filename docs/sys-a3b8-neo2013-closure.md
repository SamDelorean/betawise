# A3B8 / index 238 — generational closure

Status: **mechanically closed** from primary firmware and official-caller evidence. The original vendor symbol/name remains unknown, so the neutral `SYS_A3B8` label is retained.

## Platform availability

- AlphaSmart 3000 (2005): the index-238 table slot contains `0x05060708`, not a demonstrated firmware code pointer.
- NEO (2005): the same slot contains `0x05060708`, not a demonstrated firmware code pointer.
- NEO (2013): index 238 points to runtime `0x0043ECEE` (file offset `0x02ECEE`) and has a real handler.

The old-firmware marker is recorded mechanically and is **not** labeled `reserved`, `unimplemented`, or assigned vendor semantics.

## NEO 2013 mechanical contract

The handler reads four physical 32-bit stack slots. Only these widths have observable effect: low 16 bits of slot 1, full pointer value of slot 2, low 16 bits of slot 3, and low 8 bits of slot 4. Its neutral raw ABI is therefore:

```c
/* NEO 2013 only; vendor name and argument semantics unknown. */
uint8_t SYS_A3B8(uint16_t arg1, void *arg2, uint16_t arg3, uint8_t arg4);
```

The handler calls private helper `0x0043EEB6` with those four values. If the helper returns zero, A3B8 returns immediately. If it returns nonzero, A3B8 invokes another private dependency with literal argument `1` and retries. The retry counter permits at most six helper attempts. The final helper status is returned only in `D0.B`; the upper bits of `D0` are not part of the demonstrated contract.

No semantic names are assigned to the arguments, status values, helper, or the secondary private dependency.

## Handler and callers

The NEO 2013 handler is exactly `0x50` bytes long, ending at the immediately following A3B4 handler (`0x0043ED3E`). SHA-256 of the exact A3B8 handler bytes is:

`495fb94ec6eaef43abcf182c5ba94b95d421afa4a0866cb0806f81a5306ca143`

The official SmartApplet corpus was checked 41/41. Of the 30 applets with A-line tables, 16 use the extended table containing A3B4/A3B8/A3BC and 14 use legacy tables ending at A3B0; 11 applets are structural negatives. Exactly one executable A-line A3B8 caller was found: NEO ControlPanel.

That callsite confirms four functional inputs and byte-sized return consumption. The apparent cleanup of `0x14` bytes does **not** imply five A3B8 arguments: one older stack slot is retained beneath the four functional slots and is removed by the same cleanup. The observed functional values include slot 1 = `0x0300`, slot 4 = `3`, with pointer-like values in slots 2 and 3. The caller copies `D0.B` and tests it; it does not consume the upper return bits.

## Private evidence

Full ROM bytes, the NEO 2013 disassembly, reused helper disassembly, caller manifest, exact source hashes, stack reconstruction, and regression output remain in private Drive workpapers. Static regression verifies the three canonical ROM hashes, old/new table-slot values, handler boundary/hash, helper call, ControlPanel hash/callsite, retained-stack-slot distinction, four-argument shape, and byte return. Result: **OVERALL PASS**. Dynamic/emulator regression has not been executed.

## Adversarial conclusions

- Rejected: assuming A3B8 is portable across AS3000/NEO 2005 because neighboring libc calls are portable.
- Rejected: interpreting `0x05060708` as a demonstrated old-firmware code pointer.
- Rejected: deriving five parameters from the caller cleanup size without reconstructing retained stack state.
- Rejected: publishing a full-long return when only `D0.B` is consumed and deliberately staged.
- Rejected: inventing a subsystem name, retry meaning, error enum, or meaning for the secondary private call.

A3B8 should therefore be treated as a neutral NEO-2013-specific raw ABI until stronger symbol or subsystem evidence is recovered.
