# A3BC / index 239 — generational closure

Status: **mechanically closed** from primary firmware, firmware-caller, official-corpus, and regression evidence. The vendor symbol/name and the semantic meaning of the backing byte remain unknown, so the neutral `SYS_A3BC` label is retained.

## Platform availability

- AlphaSmart 3000 (2005): the index-239 table slot contains `0x83848788`, not a demonstrated firmware code pointer.
- NEO (2005): the same slot contains `0x83848788`, not a demonstrated firmware code pointer.
- NEO (2013): index 239 points to runtime `0x0043DACA` (file offset `0x02DACA`) and has a real handler.

The old-firmware marker is recorded mechanically and is **not** labeled `reserved`, `unimplemented`, or assigned vendor semantics.

## NEO 2013 mechanical contract

A3BC reads no stack arguments. It delegates to a private byte getter and returns immediately. A direct firmware caller invokes A3BC and tests `D0.B` immediately, establishing the external width independently of the helper implementation.

```c
/* NEO 2013 only; vendor name and byte semantics unknown. */
uint8_t SYS_A3BC(void);
```

The upper bits of `D0` are not part of the demonstrated contract: the private getter writes only `D0.B`, leaving the upper bits residual, and the demonstrated firmware caller consumes only the byte.

## Handler, backing state, and callers

The NEO 2013 A3BC handler is exactly `0x08` bytes long at runtime `0x0043DACA` / file offset `0x02DACA`. SHA-256 of the exact handler bytes is:

`cca0b14256241ab7c00bc1a88c952d1d044674466f3a74afd5823b34ffdea792`

Its private getter is also `0x08` bytes long and reads one byte from a mutable global. SHA-256 of that helper is:

`dda45819112d6ce88c5b5984fa875ebd64a09a67b00839deaa4fa75b63a26bfe`

Firmware analysis found exactly one direct absolute JSR to A3BC in NEO 2013 and no direct absolute JMP or direct BSR.W. The call is followed immediately by `TST.B D0`, which confirms the byte-sized contract. No stack input is read by A3BC.

The backing global has observed byte writers for values zero and one, but that is insufficient to assign a boolean/status/vendor meaning. No semantic name or enum is published.

The official SmartApplet corpus was checked 41/41. The 16 applets with the extended A-line tail containing A3BC were rematerialized and rehashed; none has an executable A3BC caller. The 14 legacy table-bearing applets terminate before A3BC, and the remaining 11 are structural negatives. Official-app caller count is therefore 0/41.

## Private evidence and regression

Full ROM bytes, private disassembly, exact global references, caller reconstruction, applet hashes, and regression source/output remain in Drive workpapers. Static regression validates all three canonical ROM hashes, the old/new slot values, handler/helper boundaries and hashes, direct firmware byte-return consumption, observed byte writers, and the extended-app corpus. Result: **OVERALL PASS**. Dynamic/emulator regression has not been executed.

## Adversarial conclusions

- Rejected: assuming A3BC is callable on AS3000/NEO 2005 merely because NEO 2013 implements it.
- Rejected: interpreting `0x83848788` as a demonstrated old-firmware code pointer.
- Rejected: publishing `uint16_t` or `uint32_t` return types from residual upper `D0` bits.
- Rejected: inventing implicit parameters; the complete handler reads none.
- Rejected: naming the backing byte as a boolean, status flag, subsystem state, or vendor-specific concept from only its observed 0/1 writers.

A3BC should therefore be treated as a neutral NEO-2013-specific byte getter until stronger symbol or subsystem evidence is recovered.
