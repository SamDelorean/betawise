# OS3K A2C8 closure

## Status

A2C8 is mechanically closed at confidence A from the three canonical OS3K ROMs. The vendor symbol is unknown, so the public API uses the neutral name `SYS_A2C8`.

```c
uint32_t SYS_A2C8(const void *source, void *out_state, uint8_t selector);
```

The dynamic regression suite is specified but has not yet been executed.

## Physical ABI

A2C8 has three 32-bit physical stack slots. The first two are pointers and only the low byte of the third slot is consumed. `out_state` is dereferenced without a NULL check and is therefore mandatory in the observed implementations.

The final D0.L domain is exactly `0` for selectors 0..2 or `0x02000002` for any other selector. The high-order bits are therefore part of the contract; this routine must not be modeled as a byte or boolean return. Signedness cannot be distinguished from the available evidence.

## Common mutations

Before selector dispatch, every implementation:

- clears 18 bytes beginning at `out_state + 0x14`;
- clears the byte at `out_state + 0x2A`;
- stores `source` at `out_state + 0x00`.

Bytes `out_state + 0x26..+0x29` are not initialized by A2C8. An invalid selector still performs the common mutations before returning `0x02000002`; the error path is therefore partially mutating rather than transactional.

## Selector behavior

Selector 0 zeros the longword at +0x04 and words at +0x08/+0x0A/+0x0C/+0x0E. Selector 1 copies the source longword at +0x18 and source words at +0x22/+0x20 into the corresponding output fields. Selector 2 copies the source longword at +0x24 and source words at +0x3C/+0x3E/+0x1C/+0x1E.

No vendor semantic names are assigned to these fields or selector values.

## Generational difference

AS3000 2005 has a 0x9E-byte implementation. NEO 2005 and NEO 2013 have 0xBC-byte implementations that are mechanically identical after two helper relocations.

NEO additionally writes the longword at `out_state + 0x10`. A 0x44-byte helper selects one of source longwords +0x4E/+0x52/+0x56/+0x5A from bits 3:2 of a flags byte. AS3000 does not perform this stage and leaves output +0x10 unchanged.

This platform difference is contractual behavior and must not be normalized away.

## Callers and xrefs

Fresh source-first re-audit corrects an earlier negative-xref statement: **all three canonical ROMs contain four direct `BSR.W` callers to A2C8**. The AS3000 callsites are `0x004D4876`, `0x004D4CEE`, `0x004D6104`, and `0x004D6154`; NEO 2005 uses `0x005D6B38`, `0x005D6FB8`, `0x005D849E`, and `0x005D84EE`; NEO 2013 uses `0x00428212`, `0x004286B8`, `0x00429EF4`, and `0x00429F44`.

The four AS3000 windows are instruction-level homologues of the four NEO callsites after register/relocation differences, so they are executable calls rather than aligned data. The previously documented NEO caller shapes remain valid: confirmed calls use selector 0, some construct a local destination, and others expand a source pointer in-place. None of the corresponding callsites consumes D0 after the call.

No absolute `JSR` or `JMP` caller was found in any generation. Occurrences of the bytes `A2 C8` in inspected official applets belong to the contiguous A-line stub table and are not treated as semantic callsites.

## Regression specification

The emulator-first regression should cover selectors 0, 1 and 2; invalid selectors and partial mutation; preservation of output bytes +0x26..+0x29; NEO helper selection for all four values of bits 3:2; AS3000 preservation of output +0x10; and in-place aliasing demonstrated by callers.

## 2026-09-04 source-first re-audit

Canonical ROM identities and exact handler fingerprints were reproduced 3/3: AS3000 `5f9850ba34bee772e248add7eb2c5e097cf5c600cf8d6e019af059163e666a22`, NEO 2005 `259b3d98675732105381f233522ec41d6a8ed0cfca7c45ec05c4cdc072c2fd17`, and NEO 2013 `75697f7954d18bb86cf07e30a3628f5ae33605461ad0846f4e2168e51302ebf4`. All three bodies retain the explicit full-long invalid-selector sentinel `0x02000002` and the documented handler lengths.

Whole-ROM branch resolution finds **4/4/4 direct BSR.W callers**, correcting the prior AS3000 count from zero to four. This xref correction does not change the raw ABI, selector behavior, return domain, or the AS-vs-NEO `out_state+0x10` difference. Static structural regression: **15/15 PASS** (3 canonical ROM identities, 3 handler fingerprints, 3 exact handler lengths/sentinel checks, 3 direct-BSR target sets, 3 absence-of-absolute-JSR/JMP checks).

Dynamic/emulator regression remains **specified / not executed**.

Firmware, long disassemblies and raw handler bytes remain private workpaper material.
