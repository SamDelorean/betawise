# OS3K A2FC closure

Status: **mechanically closed**, confidence A.

## Neutral ABI

```c
uint8_t SYS_A2FC(void *state,
                 uint32_t selector,
                 void *arg3,
                 uint32_t *out_value,
                 void *unused_arg5);
```

The name is intentionally neutral. No vendor symbol, semantic enum, flag names, or status names have been recovered.

## Mechanical evidence

The canonical AS3000 2005, NEO 2005, and NEO 2013 ROMs implement A2FC as a 0x254-byte handler with the same control-flow graph. After neutralizing only two absolute JSR operands and one BSR displacement, all three handlers have the same SHA-256:

`525743fc9353c3450247c8789e0ad831311f5b5627ad6769de368bec1c42c003`

The dispatcher uses the low byte of `selector`, accepts thirteen raw selector bytes (`01,07,08,0B,0C,0D,10,11,12,13,1E,1F,20`), and selected routes also compare the complete 32-bit selector against values including `0x00060001` and `0x0006000C`. Therefore the public argument remains `uint32_t` rather than an invented byte-sized enum.

`state` is mutable and is accessed through fields extending at least through an indexed byte area beginning at offset `0x20`. `arg3` is also mutable: observed routes read/write its longword at offset 0 and consume the pointer stored at offset 4. `out_value` is mandatory and is cleared before dispatch; observed routes later write raw values including 1, 8, and 19. The fifth physical stack slot is preserved in the declaration but is not accessed by any of the three handlers.

All reachable exits explicitly set `D0.L` to 0 or 1. The two independent official AlphaWordPlus callers pass five 32-bit slots, clean exactly 0x14 bytes, copy D0, and subsequently test only the low byte. The neutral callable contract therefore exposes `uint8_t`; no vendor boolean meaning is asserted.

## Callers and helpers

A corrected 41/41 SmartApplet sweep finds executable A2FC callers only in AlphaWordPlus 2005 and AlphaWordPlus NEO 2012; the other 39 applets are negative. Direct ROM-wide JSR/JMP references to A2FC are negative.

Two direct helpers used by A2FC are generation-invariant at the byte level. The first is an overlap-safe byte-copy primitive (0x3c bytes, SHA-256 `b574de77a0d80a9160bd4fbbf159df7009b64707c9bd0fdf99f62197f54fe5b4`). The second is a register-argument arithmetic helper (0x52 bytes, SHA-256 `6a887293685ec2811dcf0f35a672b2642727b0c89ee3553f127aac6d498d9894`) used to derive a bounded word stored at `state+0x1e`. A third state-processing callee receives `(state,arg3,out_value)` in one dispatch route; its return is ignored by A2FC. No semantic names are assigned to these helpers.

## Adversarial corrections

An intermediate reading treated the caller's full-long copy of D0 as evidence for a `uint32_t` return. Following the copied register to its consumer shows `TST.B`, so that conclusion was rejected. D0 happens to be fully defined by `MOVEQ`, but the independently observed caller contract is byte-sized.

The fifth caller slot is not promoted to a semantic parameter: exhaustive frame-access analysis finds no read, write, LEA, or PEA of that slot in any generation.

## Regression and privacy

A private static regression revalidates all three canonical ROM hashes, raw and normalized A2FC hashes, dispatcher data, terminal return construction, both invariant helper bodies, caller cleanup, and the byte-sized return consumption. Result: **PASS**.

Full ROM bytes, separate 68k listings, caller workpapers, and the regression script remain private in the project Drive. Dynamic emulator regression remains specified/not executed and is not represented here as completed.
