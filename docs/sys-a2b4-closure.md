# SYS_A2B4 closure

`SYS_A2B4` is mechanically closed across the canonical AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3.15 (2013) firmware images.

```c
void SYS_A2B4(uint32_t *out_0, uint32_t *out_1, uint32_t *out_2);
```

## Confirmed contract

- Three 32-bit stack slots are consumed, each as a required pointer.
- The routine copies one internal 32-bit value to each output pointer.
- The three internal values are read-only from the routine's perspective.
- No pointer validation or NULL guard is present.
- D0 is not modified by the complete handler; there is no public return value.
- The handler is 0x20 bytes in all three firmware generations.
- After neutralizing only the three relocated internal addresses, all three handlers are byte-equivalent (normalized SHA-256 `56af2e7eb22656066d578191adc1abb75a784b9f95f20bedafaf0ae76e1498b8`).

The original vendor names and semantics of the three internal values remain unknown, so neutral output names are retained rather than guessing a subsystem-specific API name.

## Cross-version result

No control-flow, argument-layout, width, or side-effect differences were found between AS3000 2005, NEO 2005, and NEO 2013. The only handler differences are the relocated addresses of the three internal 32-bit values.

## Caller evidence

No direct JSR/JMP references to the implementation were found in the three canonical ROMs. Targeted scans of wireless-related SmartApplets did not establish an executable caller outside the standard A-line stub table. The contract is nevertheless explicit from the complete handler: all three stack arguments are loaded and dereferenced unconditionally.

## Validation status

An emulator-first regression is specified but not yet executed. It should preload three distinct internal values, verify exact independent copies to the three outputs, confirm no mutation of the source values, and verify that D0 is not part of the contract.

Firmware bytes, long disassembly listings, and private workpapers remain outside the public repository.
