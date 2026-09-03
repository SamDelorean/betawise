# SYS_A218 — clipboard tag getter (source-first closure)

Status: **CLOSED — firmware-confirmed**

A218 is index 134 in the OS3K A-line table. No reliable original vendor-facing symbol has been recovered from the historical/source corpus, so the neutral name `SYS_A218` is retained.

## Contract

```c
uint8_t SYS_A218(void);
```

**CONFIRMED:** the handler takes no arguments and returns the low byte of the shared clipboard metadata/tag field. It performs no helper calls and has no state-changing side effects.

Only the low byte is contractual. The firmware implements the return with a byte-sized load into D0 and does **not** explicitly clear the upper 24 bits of D0; callers and wrappers must therefore consume the result as `uint8_t`, not as a clean 32-bit scalar.

## Source-first correlation

- `os3k/syscall.c` maps index 134 to the deliberately neutral `SYS_A218`; unlike A20C/A210/A214, no inherited BetaWise public name is present.
- The reconstructed `file_clipboard_state.h` already modeled A218 as a byte-valued getter; this audit treated that header as a hypothesis, not primary proof.
- Searches of the available historical/project corpus did not recover a more authoritative public symbol for A218.
- The adjacent clipboard family provides the semantic anchor: A20C writes the shared tag as part of setting clipboard content, while A214 clears it.

## Firmware verification

The three canonical ROMs were independently hash-checked and the A218 handler was re-extracted from each image. In AS3000 2005, NEO 2005, and NEO 2013 the handler has the same two-instruction shape: a byte load from the generation-specific clipboard-tag global into D0 followed by return. The global address moves between generations, but the ABI and behavior do not.

No stack arguments, helper calls, descriptor writes, or branches occur in the handler.

## Callers and refutation

A direct absolute-JSR search found no A218 callers in AS3000 2005 or NEO 2005 and one in NEO 2013. That NEO 2013 caller immediately compares the returned **byte** against value `4`; on equality it invokes A214 (`ClipboardClear`). This independently validates both the byte return width and the interpretation as a clipboard state/tag value rather than a length, capacity, pointer, or boolean.

A raw search for the A218 trap word found no direct in-ROM trap-word callers in the three canonical images. This is a negative result for direct encodings only; it does not exclude indirect dispatch paths.

Refutation attempts considered alternative interpretations as clipboard length, capacity, allocation status, and boolean presence flag. They fail against the primary code: A218 reads only the shared one-byte tag global, and the NEO 2013 caller compares that byte with `4`, so the field is not reducible to boolean presence.

## Cross-generation result

- **AS3000 2005:** same byte-getter contract.
- **NEO 2005:** same byte-getter contract.
- **NEO 2013:** same byte-getter contract; additionally supplies the concrete internal caller that compares the tag with `4` before conditionally clearing the clipboard.

No behavioral incompatibility was found among the three generations.

## Confidence classification

- `uint8_t SYS_A218(void)`: **CONFIRMED**.
- Reads the shared clipboard tag/metadata byte: **CONFIRMED**.
- No arguments / no side effects / no helper calls: **CONFIRMED**.
- `4` is a real internally tested tag value: **CONFIRMED**.
- Human-readable symbolic meaning of tag value `4`: **UNKNOWN**.
- Original vendor-facing function name: **UNKNOWN**; `SYS_A218` remains intentional.

## Regression

Static regression was **EXECUTED: 25/25 PASS** against the three canonical ROMs. It verifies ROM identities, exact handler extent/shape, byte-sized global load, return instruction, direct-caller counts, absence of raw A218 trap-word occurrences, and the NEO 2013 compare-with-4 / conditional-A214 caller sequence.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**. A future emulator/device probe should seed distinct tag values, call A218, verify byte truncation behavior at the wrapper boundary, and confirm that no clipboard descriptor/storage field changes.

This document intentionally publishes the functional contract and reproducible conclusions only. Canonical firmware images, raw extraction artifacts, and extended disassembly remain private.
