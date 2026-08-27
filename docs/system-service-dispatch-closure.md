# A25C global-service dispatcher closure

## Status

`A25C` is mechanically closed with **confidence A** for the compared
AlphaSmart 3000 and NEO System 3 ROMs. No trustworthy original public/vendor
symbol has been recovered, so the neutral BetaWise name remains:

```c
int32_t SYS_A25C(uint32_t action_mask, KeyMod_e key);
```

“Global-service dispatcher” is a functional description, not a recovered
historical name.

## ABI contract

`action_mask` occupies a 32-bit argument slot. Bits 0, 1, 2 and 3 are tested
independently and in that order, so one call may request more than one service.

`key` occupies the low word of the second 32-bit argument slot and is consumed
only when bit 3 is set.

The caller-visible signed 32-bit return is:

- `0` when the bit-3 route is not executed;
- `8` when the bit-3 helper recognizes the requested special action;
- `-9` when that helper does not recognize it; this path also performs System
  3 housekeeping.

Bits 0–2 pump global communication/connection services. Their private internal
names have not been recovered and are deliberately not promoted to public API
names.

## Cross-ROM behavior

The AlphaSmart 3000 2005 and NEO 2005 handlers have the same mechanical shape.
The NEO/System 3.15 handler retains the ABI and return contract while adding one
extra stage to the bit-0 service path.

## Caller evidence

System 3 callers use bit 3 to process special-key actions, including applet
navigation paths. The independent bit tests are established by the handler
itself; this document does not assign invented semantic names to bits 0–2.

## Safety and validation

This service mutates or advances global System state. It is not a pure query.
Callers should pass only the documented low four mask bits and a valid
`KeyMod_e` value when bit 3 is selected.

Dynamic regression in an emulator or physical device remains pending. The
mechanical ABI closure does not imply that regression has already been run.

## Publication boundary

This specification is written from the reconstructed contract. ROM bytes,
verbatim disassembly, complete xrefs and private workpapers are intentionally
excluded.
