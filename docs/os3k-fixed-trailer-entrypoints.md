# OS3K NEO fixed trailer entrypoints

This note records functional conclusions from correlated analysis of the fixed 0x80-byte OS3K trailer. It intentionally omits firmware bytes and vendor-private disassembly.

## Scope

The fixed trailer contains four absolute entrypoint jumps followed by revision metadata. The entrypoints discussed here are **not A-line ABI slots**.

The observations below were reproduced on canonical NEO 2005 and NEO 2013 images. Exact source-level/vendor names have not been recovered, so descriptive names are used only where the mechanical behavior supports them.

## Slot 1 — bootstrap/startup entry

NEO 2005 and NEO 2013 expose homologous slot-1 objects of exactly `0x286` bytes. Each has one `RTS`, at the final instruction, and eleven absolute subroutine calls at identical relative offsets.

The routine installs several 68k vector-table entries (`0x28`, `0x10c`, `0x110`, `0x114`, `0x118`) and performs early hardware/runtime initialization. No ordinary in-ROM caller was found; the fixed trailer is the demonstrated entry route.

Classification:

- object boundaries and vector/hardware operations: **confirmed**;
- role as primary bootstrap/startup entry: **strong inference**;
- original vendor symbol: **unknown**.

## Slot 2 — DragonBall quiesce helper

The NEO 2005 and NEO 2013 implementations are the same 0x20-byte operation except for a relocated RAM flag. Mechanically they:

1. set bit 7 in a RAM state byte;
2. clear DragonBall `TCTL1`;
3. clear DragonBall `TCTL2`;
4. clear `RTCIENR`;
5. preserve only the low nibble of `PESEL`;
6. return.

The MC68VZ328 register map independently identifies the involved registers. Thus timer/RTC interrupt quiescing and the Port-E select update are confirmed, while the meaning of the RAM flag and the higher-level policy remain unresolved.

## Slot 3 — re-entry/restart trampoline

The slot-3 implementations reset the execution environment by loading the system stack pointer, clearing `A6`, and jumping to a main initialization/re-entry routine.

Both NEO generations have exactly seven direct internal callers of this trampoline. Four caller sites are immediately guarded by a 16-bit sentinel comparison against `0x2675`; three are guarded by an 8-bit sentinel comparison against `0x52`. The referenced RAM locations move between generations while the control pattern is preserved.

Classification:

- stack/A6 reset trampoline, caller set, and guard families: **confirmed**;
- role as controlled system re-entry/restart path: **strong inference**;
- source-level meaning of the sentinels and original vendor symbol: **unknown**.

## Slot 4 and revision record

Separate analysis established that the fourth entry compares the installed secondary identity payload with the name field in the trailer revision record. The comparison mechanics are confirmed; the source-level name of the entrypoint remains unknown.

## Structural consequence

These fixed-trailer entrypoints are boot/runtime infrastructure, not evidence of additional A-line syscalls. The demonstrated A-line frontier therefore remains unchanged.
