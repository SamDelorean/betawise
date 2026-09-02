# A150 / index84 — SYS_A150 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
void SYS_A150(void);
```

The original vendor symbol has not been recovered, so the neutral `SYS_A150` name is retained.

## Confirmed behavior

The canonical AS3000 2005, NEO 2005 and NEO 2013 handlers are byte-for-byte identical and only 0x0E bytes long:

```text
new_control = USTCNT & 0xBFFF;
USTCNT = new_control;
return;
```

Mask `0xBFFF` clears only USTCNT bit 14. The DragonBall UART defines that bit as **RX EN**. Clearing it disables the receiver; the hardware definition also specifies that disabling the receiver flushes the receive FIFO.

No public argument is read and no helper is called.

## Return value

`D0` incidentally contains the resulting USTCNT word because it is used as the temporary for the read/modify/write sequence. Direct callers do not consume it as a syscall result, so no return value is part of the recovered ABI.

## Generation comparison

A150 explicitly accesses the USTCNT register at `F900` in all three canonical generations, including NEO 2013. This is intentionally documented because other NEO 2013 UART paths in this ABI region also use the second UART register bank at `F91x`; that difference must not be normalized away.

Direct absolute callers found during the private audit: 1 in AS3000 2005, none in NEO 2005 and 2 in NEO 2013.

## Regression status

Static regression: **EXECUTED — 21/21 PASS**.

It verifies canonical ROM hashes, exact 0x0E handler bytes, the `0xBFFF` mask, F900 read/write behavior, terminal RTS and direct caller counts across all three generations.

Dynamic UART regression: **SPECIFIED / NOT EXECUTED**. A future probe should enable RX, queue receive data, call A150, then verify RXEN is clear and the receive FIFO is flushed while unrelated USTCNT bits remain preserved.

## Confidence

- **CONFIRMED:** index84, zero-argument `void` ABI, exact three-generation handler, RXEN clear, preservation of other control bits, caller counts.
- **STRONG INFERENCE:** descriptive role “disable UART receiver”.
- **UNKNOWN:** original vendor name and the architectural reason NEO 2013 A150 remains on F900 while some neighboring services use F91x.

Raw firmware, private disassembly and ROM offsets remain outside the public repository.
