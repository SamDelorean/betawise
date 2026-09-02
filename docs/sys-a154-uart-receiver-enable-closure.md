# A154 / index85 — SYS_A154 — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
void SYS_A154(void);
```

The original vendor symbol has not been recovered, so the neutral `SYS_A154` name is retained.

## Confirmed behavior

The public handler is 0x12 bytes in each canonical generation. It performs a read/modify/write of the UART status/control register, sets bit 14, then immediately performs a 16-bit read of the receiver register before returning.

DragonBall defines USTCNT bit 14 as **RX EN**. Its programming model also requires a receiver-register word read after enabling receive operation to initialize FIFO/status state before receive interrupts are used. A154 implements that sequence directly.

Conceptually:

```text
control = USTCNT;
control |= (1 << 14);   // RX EN
USTCNT = control;
(void)URX;              // initialize receiver FIFO/status state
return;
```

No public argument is read and no helper is called.

## Generation comparison

AS3000 2005 and NEO 2005 operate the UART register bank at `F900/F904`. NEO 2013 performs the same sequence at `F910/F914`.

This platform difference is intentional and should not be normalized away. It is particularly important because neighboring A150 remains mapped to F900 in all three canonical generations.

Direct absolute firmware callers found during the private audit: 1 in AS3000 2005, none in NEO 2005 and 3 in NEO 2013.

## Return value

`D0` incidentally contains the modified control word and the final URX test affects condition codes. Direct callers do not use either as a functional result. The recovered ABI is therefore `void`.

## Regression status

Static regression: **EXECUTED — 24/24 PASS**.

It verifies canonical ROM hashes, exact 0x12 handler grammar, RXEN bit set, generation-specific USTCNT/URX addresses, terminal RTS and direct caller counts.

Dynamic UART regression: **SPECIFIED / NOT EXECUTED**. A future probe should begin with receive disabled, invoke A154, verify RXEN is set, and then confirm receiver FIFO/status behavior is initialized without modifying unrelated control bits.

## Confidence

- **CONFIRMED:** index85, zero-argument `void` ABI, set-RXEN operation, mandatory receiver-register read, generation-specific register bank and caller counts.
- **STRONG INFERENCE:** descriptive role “enable/initialize UART receiver”.
- **UNKNOWN:** original vendor function name and the platform rationale for the NEO 2013 UART-bank selection.

Raw firmware, private disassembly and ROM offsets remain outside the public repository.
