# A14C / index83 — SYS_A14C — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
typedef void (*SYS_A14C_RxCallback)(uint8_t rx_status, uint8_t rx_data);
void SYS_A14C(SYS_A14C_RxCallback callback);
```

The original AlphaSmart vendor symbol has not been recovered, so the public name remains neutral.

## Confirmed behavior

The canonical AS3000 2005, NEO 2005 and NEO 2013 implementations expose the same 0x4A-byte public handler after normalizing relocated state/helper addresses and the platform UART register mapping.

The handler atomically:

1. stores the supplied function pointer in the UART receive-callback state;
2. if `callback == NULL`, clears the receive interrupt-enable group represented by mask `0xB8` in the low USTCNT shadow byte;
3. otherwise sets that same group;
4. mirrors the shadow byte to the live UART control register when that UART is active;
5. restores the previous processor interrupt mask and returns.

On the DragonBall UART, `0xB8` covers GPIO-delta plus RX-FULL, RX-HALF and RX-READY interrupt enables. The complementary clear mask is `0x47`, preserving the unrelated CTS/TX interrupt enables.

Therefore `NULL` is a supported raw input that unregisters/disables the receive callback path; a non-NULL callback installs it and enables the associated receive interrupt sources.

## Callback ABI

The stored pointer is later dereferenced as code by the UART receive service path. That path reads URX as a 16-bit value, separates the high status byte from the low received-data byte, and invokes the callback with two promoted ABI slots:

```c
callback(rx_status, rx_data);
```

`rx_status` carries the URX FIFO/character-status byte (FIFO full/half/data-ready and overrun/frame/break/parity status); `rx_data` is the received character byte. Representative firmware callbacks test bits in the first argument, independently confirming the argument order.

The callback return value is not consumed.

## Critical-section helpers

The public handler uses a paired System 3 helper sequence. The entry helper saves SR in `D0` and raises the interrupt mask to level 7; the exit helper restores SR from the saved token. This protects the callback pointer and control-shadow update atomically.

The residual `D0` value after the restore is not a functional syscall result. Direct callers do not consume it, so the public contract is `void`.

## Callers and generations

Direct absolute firmware callers found in the private audit: 5 in AS3000 2005, 4 in NEO 2005 and 5 in NEO 2013. Every direct caller installs an in-image code pointer.

NEO 2013 uses a different live UART control-byte mapping (`F911` instead of `F901`) and contains an additional receive-service path that consumes the same callback state. These are real platform differences but do not alter the public ABI.

## Important boundary correction

The large distance from vector A14C to vector A150 does **not** represent the size of the A14C syscall. A14C terminates at its own RTS after exactly 0x4A bytes in all three canonical ROMs. The larger private routine beginning immediately afterward is separate code and is not attributed to the syscall contract.

## Regression status

Static regression: **EXECUTED — 79/79 PASS**.

It verifies canonical ROM hashes, public entry/length, argument slot, callback-global store, NULL/non-NULL masks, active-state mirror, critical-section helpers, direct caller counts and callback-pointer preparation across all three generations.

Dynamic UART regression: **SPECIFIED / NOT EXECUTED**. A future emulator/hardware probe should register a synthetic callback, inject RX data with known status flags, verify `(status,data)` ordering, then unregister with `NULL` and confirm the receive interrupt path is disabled.

## Confidence

- **CONFIRMED:** index83, one callback argument, callback persistence and indirect invocation, two-byte callback ABI, NULL disable/non-NULL enable behavior, USTCNT mask mechanics, atomic update, `void` return, three-generation equivalence with documented hardware relocation.
- **STRONG INFERENCE:** descriptive role “register UART RX callback / enable receive interrupts”.
- **UNKNOWN:** original vendor function and typedef names.

Raw firmware, private disassembly, ROM offsets and extended corpus remain outside the public repository.
