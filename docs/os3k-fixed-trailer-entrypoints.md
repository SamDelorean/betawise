# OS3K NEO fixed trailer entrypoints

This note records functional conclusions from correlated analysis of the fixed 0x80-byte OS3K trailer. It intentionally omits firmware bytes and vendor-private disassembly.

## Scope

The fixed trailer contains four absolute entrypoint jumps followed by revision metadata. The entrypoints discussed here are **not A-line ABI slots**.

The observations below were reproduced on canonical NEO images where stated. Exact source-level/vendor names have not been recovered, so descriptive names are used only where the mechanical behavior supports them.

## Slot 1 — bootstrap/startup entry

NEO 2005 and NEO 2013 expose homologous slot-1 objects of exactly `0x286` bytes. Each has one `RTS`, at the final instruction, and eleven absolute subroutine calls at identical relative offsets.

The routine installs several 68k vector-table entries (`0x28`, `0x10c`, `0x110`, `0x114`, `0x118`) and performs early hardware/runtime initialization. No ordinary in-ROM caller was found; the fixed trailer is the demonstrated entry route.

Classification:

- object boundaries and vector/hardware operations: **confirmed**;
- role as primary bootstrap/startup entry: **strong inference**;
- original vendor symbol: **unknown**.

## Slot 2 — DragonBall quiesce helper

The NEO 2005 and NEO 2013 implementations are the same `0x20`-byte operation except for a relocated RAM flag. Mechanically they:

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

## Slot 4 — installed identity/name equality comparator

On canonical NEO 2013, slot 4 is a compact boolean comparator. It reads the installed secondary identity string and compares it byte-for-byte with the `name` field of the trailer revision record. Independent protocol tooling identifies revision-record offset `+0x06` as `name`, while the updater/Small-ROM segment mapper independently establishes the installed secondary-segment placement.

The mechanical contract is:

- compare the two NUL-terminated strings for exact equality;
- return `1` when both strings match and terminate at the same point;
- return `0` on a differing byte or when only one string terminates;
- ignore bytes after the first NUL in either storage object.

The canonical NEO 2013 package demonstrates the intended data relationship directly: the revision-record name and the secondary installed identity carry the same `System 3 Neo` identity string (including its fixed padding before the terminator). A byte stored after the secondary string terminator does not participate in the comparison.

A complete absolute-reference search finds no ordinary in-ROM `JSR` caller of this entrypoint. Its demonstrated references are the fixed/repeated trailer jumps. Therefore the equality contract and the identity/name objects being compared are **confirmed**, but the external policy that consumes the boolean result and the original vendor symbol remain **unknown / insufficient evidence**. In particular, this note does not assign a speculative vendor name such as `version_check`.

## Revision record

Independent host-side tooling decodes the revision record with revision-major and revision-minor bytes followed by a fixed `name` field and build-date text. This independently anchors the field used by slot 4 without relying on inferred firmware naming.

## Structural consequence

The fixed-trailer entrypoints are boot/runtime infrastructure, not evidence of additional A-line syscalls. The demonstrated A-line frontier therefore remains unchanged at A470.
