# AS3000 System 3 file storage — physical-layer reconciliation

This note reconciles the AS3000 physical-storage evidence introduced on `master`
by commit `54f8c5181d36988e289ce8dbf254d1e6c7b6bff2` with the later three-generation
allocator closure already present on `sdk/abi-automation`.

It is intentionally documentation-only. It does not change a syscall ABI,
public header, vendor name, closure state, or build-validation state.

## Safe evidence imported from the AS3000 physical-layer note

The production AS3000 memory map provides 256 KiB of ordinary writable SRAM at
`0x00000000..0x0003ffff` and Flash at `0x00400000..0x004fffff`. The reconstructed
File API mutates payload through ordinary byte copy/move/zero-fill operations and
`FileClose` does not perform a Flash program/erase or persistence flush. Together
with the documented CR2032-backed memory retention, this supports the physical
conclusion that ordinary System 3 file payload is held in battery-backed SRAM,
not copied to Flash on close.

The same source also independently corroborates the System 3 dynamic allocator:
the 2005 System 3 Addendum documents a 512-character minimum file size (`0x200`),
consistent with the reconstructed allocator floor.

## Superseded boundary in the older note

The original `master` note correctly marked the exact AS3000 allocator pool and
physical placement/compaction mechanics as unresolved *at the time it was
written*. Those statements must not be imported as current limitations. The
later three-generation allocator closure (`ad557fcf4efbf07fcf0ff3f398a71a7987b7a41c`)
provides the stronger/current allocator-mechanics evidence and takes precedence
where the documents differ in reconstruction completeness.

Accordingly:

- retain the SRAM-versus-Flash persistence conclusion;
- retain the independent `0x200` UI/manual corroboration;
- retain the warning not to project NEO addresses onto AS3000;
- do not reintroduce the older note's now-superseded allocator-mechanics TODOs;
- do not infer vendor API names or public SDK prototypes from physical-storage
  evidence.

## SDK/ABI consequence

None directly. This evidence constrains implementation/emulation and documents
why File payload pointers refer to mutable retained memory on AS3000. It does not
justify changing `os3k.h`, `os3k.c`, `syscall.c`, a raw contract, or any closure
classification.

No m68k build is required for this documentation-only reconciliation, and this
note does not assert `BUILD_VALIDATED`.
