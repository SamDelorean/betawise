# SYS_A29C closure

## Status

`A29C` is mechanically closed with confidence **A**. No vendor symbol or API name has been recovered, so the neutral symbol `SYS_A29C` is retained.

```c
uint8_t SYS_A29C(void);
```

## Cross-ROM evidence

The handler is 0x40 bytes in all three canonical System 3 images:

- AS3000 2005: runtime `0x004C526C`, file `+0x00526C`
- NEO 2005: runtime `0x005C528E`, file `+0x00528E`
- NEO 2013: runtime `0x00415702`, file `+0x005702`

In each image the epilogue ends immediately before `A2A0`. The three control-flow graphs are identical; differences are limited to relocated absolute operands. After neutralizing the helper target and shared-state operand, the normalized SHA-256 is:

`71ce44170d8e38a19937e42fbe0b7e486228d5b010cd818aa5c10b29845f01da`

## Reconstructed behavior

`SYS_A29C` takes no caller arguments. It invokes one internal helper with a zero argument and observes the helper result in `D0.B`.

1. If the helper returns `0x14`, the helper call is retried.
2. A non-zero helper result other than `0x14` is propagated unchanged as the low-byte return value.
3. If the helper returns zero, the handler polls a shared state byte while it is `7` or `4`.
4. Shared state `1` produces success (`0`).
5. Any other final shared-state value produces `0x1F`.

The helper itself is cross-ROM equivalent after relocation normalization. It returns `0x13` when the shared state is outside its accepted 2/3 states, returns `0x14` for a transient condition when called with zero, and otherwise initiates a state transition from 2 or 3 into 4 or 7 and returns zero.

## Callers

Exactly two absolute internal JSR callers were found per canonical ROM, and no absolute JMP callers:

- AS3000: `0x004E4482`, `0x004E44A6`
- NEO 2005: `0x005E6D38`, `0x005E6D5C`
- NEO 2013: `0x0043D162`, `0x0043D186`

Neither caller prepares arguments for `A29C`. Both ignore its return value. That does not make the reconstructed API `void`: the handler deliberately constructs and propagates a byte-sized status result, while the high bits of `D0` are not contractual.

A Beamer binary from both the 2005 and later NEO corpus contains `A29C` only once inside the contiguous A-line stub table (`A298`, `A29C`, `A2A0`, ...), not as an independent executable caller.

## Adversarial checks

The following alternatives were rejected from primary evidence:

- `A29C` does **not** take an argument; the zero on the stack belongs to the internal helper call.
- `0x14` is **not** a terminal result at this layer; the handler retries it.
- The shared state is inspected only after helper success; non-zero helper errors are propagated directly.
- States `7` and `4` are polling states, not success states; state `1` is the only observed success state.
- A semantic name such as “disconnect”, “close”, or “shutdown” is not justified by the available nominal evidence and is therefore not published.

## Validation status

An emulator-first regression is specified but not yet executed. It should cover repeated `0x14`, direct propagation of helper errors, transitions `7/4 -> 1`, terminal non-1 states returning `0x1F`, zero caller arguments, and the byte-only return contract.

Full ROM bytes and correlated disassembly remain private workpapers in Drive.
