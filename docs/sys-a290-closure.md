# System 3 / OS3K closure — A290

This note closes the mechanical ABI of trap A290 across the November 2005 AlphaSmart 3000 ROM, November 2005 NEO ROM, and July 2013 NEO/System 3.15 ROM. The original public/vendor symbol and field semantics have not been recovered, so the SDK retains the neutral `SYS_A290` name and structural field labels.

## Reconstructed public form

```c
typedef struct __attribute__((packed)) {
    uint32_t field_0;
    uint8_t field_4;
    uint8_t field_5;
} SYS_A290_Record;

uint8_t SYS_A290(SYS_A290_Record records[6], uint8_t optional_out32[32]);
```

The first argument is a writable table of exactly six packed records, each six bytes long. The second argument may be `NULL`; when supplied, exactly 32 bytes are copied to it from an OS-owned block. Only `D0.B` is contractual on return.

## Preconditions, return values and effects

A relocated subsystem-state byte must equal `1`. If it does not, A290 returns `0x15` without calling its worker. The worker's non-zero byte errors are propagated through `D0.B`; observed worker error values include `0x12` and `0x13`. Success returns byte value `0`.

The handler invokes the worker on the 36-byte table, then scans the six entries for duplicate `field_0` values. A first duplicate causes another worker pass and rescan. A duplicate surviving a later pass causes the later record to be cleared (`field_0`, `field_4`, and `field_5` all become zero). This proves a normalization/deduplication contract, but does not establish what the records represent.

When `optional_out32 != NULL`, the handler copies exactly 32 bytes from a relocated OS block. `NULL` is explicitly valid and is used by an internal caller.

## Cross-ROM evidence

The complete handler is 0xe4 bytes in all three ROMs:

| Firmware | Handler runtime |
| --- | ---: |
| AS3000, November 2005 | `0x004c5026` |
| NEO, November 2005 | `0x005c5048` |
| NEO/System 3.15, July 2013 | `0x004154bc` |

Only six bytes differ between the three bodies, all belonging to three relocated absolute operands: the subsystem-state byte, worker target, and 32-byte output-block base. After masking only those relocation bytes, the normalized handler SHA-256 is `2838302b285315765574bb5c91afc0ab757894bdd1681ef42bbbc9cbcf733f74`.

The worker is likewise structurally equivalent across the three ROMs. Its targets are `0x004c3b62`, `0x005c3b84`, and `0x00413ff8`; it initializes/clears the 36-byte table, operates on the same subsystem state, exposes byte-sized errors, and returns success through `D0.B`.

## Caller evidence

Each ROM contains exactly one equivalent absolute JSR caller of A290 and no absolute JMP or direct A290 opcode outside the common stub mechanism. The caller passes `NULL` as the second argument and a fixed 36-byte table as the first, discards two 32-bit argument slots after return, and copies only `D0.B` for status handling.

After success it iterates exactly six records of stride six and consumes `field_0` from each non-empty entry. That 32-bit field is subsequently passed unchanged to A294. This independently confirms the table layout, the byte-sized return contract, and the mechanical A290→A294 relationship.

## Historical and adversarial review

Historical BetaWise material and the modern `neo-re` reference do not provide an independent recovered public symbol or prototype for A290. Numeric proximity to the confirmed IrDA initializer A28C and the downstream A294 operation is compatible with an IrDA discovery/listing interpretation, but that interpretation is not promoted into the public API because no independent nominal evidence identifies the records or their fields.

A generic opaque first argument is rejected because the handler explicitly indexes six six-byte records. A mandatory second argument is rejected because the official internal caller passes `NULL`. A clean 32-bit return is rejected because both the handler and caller define/consume only the low byte.

## Regression status

An emulator-first regression is specified but has not been executed. It should cover: subsystem state other than `1` returning `0x15`; worker errors `0x12`/`0x13` propagating as bytes; success with `optional_out32 == NULL`; a valid output buffer receiving exactly 32 bytes; unique records surviving normalization; duplicate records forcing the second pass and clearing later duplicates; and preservation of the rule that only the low byte of `D0` belongs to the ABI contract.
