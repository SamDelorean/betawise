# A368 / `memset` closure

Status: **mechanical A / published**.

Reconstructed contract:

```c
void *memset(void *ptr, int value, size_t num);
```

Primary reconstruction uses the three canonical OS3K ROMs. A368 resolves to AS3000 2005 `0x004DDBA6` (`file+0x01DBA6`), NEO 2005 `0x005E02C4` (`file+0x0202C4`), and NEO 2013 `0x004366C2` (`file+0x0266C2`). The contractual handler is byte-identical across generations: 0x28 bytes, 15 instructions, terminal `RTS` at +0x26, SHA-256 `47e59bb2285295faa52629cf6025e5ca804e01a41ff0edf3c2d2550ab85414ee`.

The handler consumes three physical 32-bit slots. Slot 1 is the destination pointer. Only the low byte of slot 2 is used as the fill value. Slot 3 is the full 32-bit byte count. It computes the end pointer as `ptr + num`, enters the loop condition before the first store so `num == 0` performs no write, stores exactly `num` bytes, and explicitly returns the original destination pointer in `D0.L`.

The official SmartApplet sweep used the corrected PC-index detector over the complete 41-applet corpus: 100 executable A368 callers were found in 21 table-bearing applets; 9 table-bearing applets were negative and 11 applets were structural negatives without the relevant table. Direct firmware absolute-JSR counts are 22 / 30 / 34 for AS3000 2005 / NEO 2005 / NEO 2013.

Adversarial checks reject nearby alternatives. `memcpy`/`memmove` require a source pointer that A368 does not consume; string semantics are absent; a void byte-fill helper is inconsistent with the deliberate `D0.L = ptr` return. The historical BetaWise `memset` mapping therefore agrees with, but is not the basis of, the reconstruction.

Private static regression against the canonical ROMs and the official caller corpus executed with **OVERALL PASS**. Dynamic/emulator regression remains specified but was not executed. Firmware bytes and detailed disassembly remain private in Drive.
