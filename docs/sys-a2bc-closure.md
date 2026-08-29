# OS3K A2BC closure

Status: **mechanically closed, confidence A**.

## Neutral contract

```c
int32_t SYS_A2BC(void *state,
                 uint16_t selector,
                 uint16_t flags,
                 uint32_t value,
                 uint16_t coord_x,
                 uint16_t coord_y,
                 uint16_t extent_x,
                 uint16_t extent_y);
```

The original vendor routine name and semantic names for its status values have not been recovered. `SYS_A2BC` and the parameter names above are deliberately neutral/descriptive.

## Primary evidence

The contract was reconstructed independently from the canonical AS3000 2005, NEO 2005 and NEO 2013 System 3 ROMs. Exact private disassemblies remain outside this repository.

| Generation | Runtime entry | Exact handler length | Literal SHA-256 |
| --- | --- | ---: | --- |
| AS3000 2005 | `0x004D4548` | `0x42A` | `5e0a5115d04137df27f6b311ab633e4d9c58bf6808aeae4966154f703957df4a` |
| NEO 2005 | `0x005D6746` | `0x4F6` | `c96591e4d40575f9cdf15a2a9542dcf9abbfca1d976ee37b85067ff2936ede1b` |
| NEO 2013 | `0x00427E04` | `0x50A` | `2b5ac08b7ec698a06877f4a69409fce19273e918f66658e8db2a517d72188801` |

Each implementation reaches a common local epilogue that loads a 32-bit status from its stack frame into `D0` and returns. The larger NEO implementations are genuine generational variants rather than relocation-only copies.

## ABI evidence

There is one equivalent direct internal caller per canonical ROM. Each caller pushes eight 32-bit slots, calls A2BC, removes exactly `0x20` bytes, copies the complete `D0` longword and branches on zero/non-zero. The observed call is structurally equivalent to:

```text
[state_pointer, 0x00CB, 1, 0, 0, 0, 0x28, 3]
```

The handlers independently consume all eight arguments. `state` and `value` are used as full 32-bit values. `selector`, `flags`, and the four coordinate/extent fields consume the low 16 bits of their physical slots.

The return width is not inferred merely from the caller. A2BC maintains an explicit 32-bit local status, initializes it to zero, assigns full constants such as `0x01000001`, `0x02000001` and `0x02000004`, stores complete helper results, and finally returns the entire longword in `D0`. Some helper results are tested with signed longword comparisons, supporting the `int32_t` contract.

## Coordinate-domain difference

AS3000 validates two position-plus-extent pairs against a `40 x 4` domain. NEO 2005 and NEO 2013 accept a `264 x 66` domain. When NEO receives values inside the legacy ranges it scales the horizontal coordinate/extent by 6 and the vertical coordinate/extent by 16. Exact legacy full-span values at zero origin are expanded from 40 to 264 and from 4 to 66.

This difference is intentionally preserved; the NEO handlers are not described as mechanically identical to the AS3000 handler.

NEO 2013 additionally materializes `flags` bits 2, 3 and 4 into state, while bits 0 and 1 participate in control flow across the reconstructed generations.

## Status and side effects

Normal success leaves status zero. Explicit full-width non-zero statuses observed in the handlers include:

- `0x01000001` on one setup/format path;
- `0x02000001` when the coordinate/extent domain is invalid;
- `0x02000004` when a selector/flags-related validation does not produce the expected result.

Other complete helper results can be propagated. No vendor enum or prose meaning is assigned to these numeric values.

A2BC initializes and mutates the object referenced by `state`; error paths may perform cleanup before returning. The 32-bit `value` argument is stored without truncation in the state object.

## Adversarial checks

The following alternatives were rejected by primary evidence:

- fewer than eight arguments: the caller removes 32 argument bytes and every slot is consumed by the handler;
- byte- or word-sized return: full 32-bit constants and helper results are returned, and the caller consumes `D0.L`;
- one common AS3000/NEO implementation: the NEO coordinate conversion and NEO 2013 flag handling are real code differences;
- using A2C0 as a size boundary: A2C0 is located in a non-contiguous firmware region and was not used to delimit A2BC.

Searches of the current BetaWise/neo-re references did not provide an independent A2BC vendor name or prototype.

## Regression status

Emulator-first regression is **specified, not executed**. The private test plan covers the canonical caller, zero success, both explicit validation statuses, full signed helper-status propagation, flag paths, NEO 2013 extra flags, AS3000 legacy bounds, NEO native bounds, 6x/16x legacy conversion, and preservation of the full 32-bit `value` field.

Firmware bytes and extensive disassembly are intentionally not published here.
