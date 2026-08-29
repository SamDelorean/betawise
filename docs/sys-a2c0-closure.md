# SYS_A2C0 mechanical closure

Status: **MECÁNICA_CERRADA / confidence A**.

Neutral raw contract:

```c
uint32_t SYS_A2C0(void *state,
                  uint32_t selector,
                  uint32_t param,
                  uint32_t reserved,
                  uint32_t *status_out);
```

## Evidence summary

The canonical AS3000 2005, NEO 2005 and NEO 2013 handlers preserve the same five-slot physical ABI. `state`, `selector`, `param` and `status_out` are consumed. The fourth slot is present in confirmed callers but no implementation reads its stack position, so the slot is retained as `reserved` rather than removed from the raw ABI.

`status_out` is mandatory in the observed handlers: it is dereferenced without a NULL guard, initialized to zero and used to receive complete 32-bit helper status values.

The handler's contractual D0.L domain is exactly `0`, `1`, or `0x02000006`. Because a full-long sentinel exists, the return must not be narrowed to a byte or boolean. Vendor semantics for selectors, status values and the state structure remain intentionally unnamed.

## Generation differences

AS3000 2005 and NEO 2005 are close but not byte-identical. NEO 2013 has a substantially larger control-flow graph and additional logic involving state fields, while retaining the same five-slot calling convention and final D0.L domain.

## Adversarial checks

The apparent `0x24` stack cleanup in one caller is not evidence of nine A2C0 arguments: it includes stack material from neighboring calls. A second caller independently demonstrates five A2C0 slots with `0x14` cleanup. The fourth slot cannot be eliminated even though unused, because doing so would shift the fifth slot and break the physical ABI.

## Regression status

Regression is **specified, not executed**. Required probes include varying `reserved` with all other inputs fixed, covering return values `0`, `1`, and `0x02000006`, validating 32-bit `status_out` writes, and repeating the checks on all three canonical generations.

Extended disassembly, ROM bytes and private reverse-engineering workpapers remain outside the public repository.
