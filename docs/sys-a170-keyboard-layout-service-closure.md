# A170 / index92 — `SYS_A170` — source-first closure

Status: **MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
uint32_t SYS_A170(uint8_t prompt,
                  uint8_t *layout_out,
                  char *description_out);
```

`SYS_A170` is the A-line syscall at index 92 (`A170`). The historical BetaWise comment describes it as a keyboard-layout get/prompt service; the firmware confirms that description while exposing a richer three-argument contract.

## Confirmed behavior

The canonical AS3000 2005, NEO 2005 and NEO 2013 implementations share the same public behavior.

- `prompt == 0` skips the interactive screen.
- Any nonzero `prompt` enters the keyboard-layout selection UI.
- `layout_out` is optional. When non-NULL, it receives the current internal layout byte.
- `description_out` is optional. When non-NULL, firmware constructs a human-readable description of the current layout using unbounded string operations; callers must provide enough storage.
- The common exit sets `D0.L` to `1`. No alternate failure value was found, so this should not be interpreted as a meaningful success/failure boolean without additional evidence.

## Internal layout encoding

Firmware control flow fixes the previously unresolved internal numbering:

| Internal value | Layout |
|---:|---|
| `3` | QWERTY |
| `0` | Dvorak |
| `2` | Right (one hand) |
| `1` | Left (one hand) |

The interactive menu presents choices 1 through 4 in the user-facing order QWERTY, Dvorak, Right and Left. The corresponding key handlers write the internal values above.

This mapping is also relevant to the previously reconstructed A168 key-layout remapper, whose internal layout byte can now be named without guessing.

## Interactive path

With a nonzero `prompt`, the handler uses the existing OS3K display and keyboard services to present the selection screen, drain and scan keyboard input, accept choices 1–4, update the global layout state, display the resulting layout message and wait for acknowledgement.

The handler composes those operations from existing public system services plus one private localized-resource helper. The private helper is deliberately left unnamed.

## Callers

The private firmware audit found exactly one direct absolute OS caller in each canonical ROM. All three prepare the same arguments:

```c
SYS_A170(1, NULL, NULL);
```

This independently confirms both argument order and the firmware's own use of A170 as an interactive change-layout service when outputs are not needed.

## Generation comparison

No semantic difference was found among AS3000 2005, NEO 2005 and NEO 2013 for the public A170 contract. Absolute state/resource/helper locations differ, but the prompt/query/output behavior and the `3/0/2/1` layout encoding are stable across all three images.

## Regression status

Static regression: **EXECUTED — 87/87 PASS**.

It verifies the three canonical ROM hashes, A-line index resolution, public handler boundary, three ABI arguments, optional outputs, common return value, layout-number display logic, physical key comparisons, exact internal state writes, expected public helper calls, direct-caller counts, and the `(1, NULL, NULL)` argument sequence in every canonical ROM.

Dynamic UI/layout regression: **SPECIFIED / NOT EXECUTED**. A future probe should exercise noninteractive query plus all four interactive choices, then verify the layout byte, generated description and downstream behavior of A168/A164.

## Confidence

- **CONFIRMED:** index92/A170 identity, three-argument ABI, prompt zero/nonzero behavior, both optional outputs, internal layout encoding, interactive choices and state writes, common `D0.L=1`, one direct OS caller per generation and equivalent public behavior across all three ROMs.
- **STRONG INFERENCE:** the historical conceptual description “get/prompt keyboard layout” accurately summarizes the service.
- **PROVISIONAL:** original vendor typedefs/names for arguments and return type.
- **UNKNOWN:** original name of the private localized-resource helper and whether the exact textual formatting of `description_out` was ever part of a documented public contract.

Raw firmware, full disassembly, ROM offsets and private audit artifacts remain outside the public repository.
