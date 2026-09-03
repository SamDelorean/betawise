# A1C8 / FileOpen — source-first closure

Status: mechanically and semantically closed against the three canonical System 3 firmware generations. `FileOpen` is retained as the inherited BetaWise name; an independent original vendor symbol has not been recovered.

## Contract

```c
uint16_t FileOpen(uint16_t file_id, uint8_t reset_position);
```

Only the low word of the first ABI slot and the low byte of the second are consumed. The normal return is the descriptor's canonical 16-bit token, zero-extended; unresolved input returns 0.

## Confirmed behavior

1. Resolve `file_id` through the common File API resolver.
2. If resolution fails, return 0 before invoking A1CC. The previously active context is therefore preserved.
3. On success, invoke A1CC to detach the prior active File API context.
4. Prepare/synchronize the resolved descriptor and install it as the active descriptor.
5. Obtain the canonical token associated with descriptor offset `+0x44` for the returned value.
6. Read `reset_position` exactly once. If its byte is exactly `1`, clear descriptor `+0x14`, already reconstructed as the file cursor. Any other byte preserves the cursor.
7. Return the canonical token zero-extended in `D0.L`.

The firmware does not implement separate read/write/create/truncate interpretations for the second argument. `reset_position` is therefore a descriptive name for its directly observed effect, not a recovered vendor enum.

## Evidence summary

The public handler is exactly `0x130` bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013). The essential CFG is equivalent across all three; raw differences are relocations of globals/helpers/tables plus diagnostic metadata.

Direct absolute System 3 JSR callers: **11 / 11 / 11**. Independently traced official 2012 SmartApplet callers use only values 0 and 1 for the second argument: 17 calls use 1 and 8 calls use 0 across AlphaWord Plus, Calculator, ControlPanel, KeyWords, and AlphaQuiz. Calculator includes a call using reserved File API token `0x00CB` with reset value 1.

## Refutation

- The second argument is not demonstrated to be a read/write/create/truncate mode; only `== 1` has an effect, namely cursor rewind.
- Value 0 does not contain an explicit append or read-only branch.
- Resolution failure does not close the active context because the A1CC call lies exclusively on the success path.
- The normal return is not a signed status code: firmware returns a zero-extended canonical token word; 0 denotes no resolution.

## Confidence

- **CONFIRMED:** ABI, common resolver, failure-context preservation, A1CC success-only path, active-descriptor installation, canonical token return, exact `==1` cursor-reset behavior, and equivalent semantics across the three canonical firmware generations.
- **STRONG INFERENCE:** inherited functional name `FileOpen`; descriptive argument name `reset_position`.
- **UNKNOWN:** original vendor symbol and public names/semantics of private preparation helpers.

Static regression against the three canonical images: **56/56 PASS**. Dynamic/emulator verification remains specified separately and is not claimed as executed here.
