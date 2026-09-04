# SYS_A2C0 mechanical closure

Status: **MECÁNICA_CERRADA / confidence A**.

Neutral raw contract:

```c
uint32_t SYS_A2C0(void *state,
                  uint32_t selector,
                  uint32_t param,
                  uint32_t reserved,
                  uint32_t *status_out);
```

## Evidence summary

The canonical AS3000 2005, NEO 2005 and NEO 2013 handlers preserve the same five-slot physical ABI. `state`, `selector`, `param` and `status_out` are consumed. The fourth slot is present in confirmed callers but no implementation reads its stack position, so the slot is retained as `reserved` rather than removed from the raw ABI.

`status_out` is mandatory in the observed handlers: it is dereferenced without a NULL guard, initialized to zero and used to receive complete 32-bit helper status values.

The handler's contractual D0.L domain is exactly `0`, `1`, or `0x02000006`. Because a full-long sentinel exists, the return must not be narrowed to a byte or boolean. Vendor semantics for selectors, status values and the state structure remain intentionally unnamed.

## Generation differences

AS3000 2005 and NEO 2005 are close but not byte-identical. NEO 2013 has a substantially larger control-flow graph and additional logic involving state fields, while retaining the same five-slot calling convention and final D0.L domain.

## Adversarial checks

The apparent `0x24` stack cleanup in one caller is not evidence of nine A2C0 arguments: it includes stack material from neighboring calls. A second caller independently demonstrates five A2C0 slots with `0x14` cleanup. The fourth slot cannot be eliminated even though unused, because doing so would shift the fifth slot and break the physical ABI.

## Source-first re-audit — 2026-09-04

The earlier closure was revalidated independently from the three canonical ROM images. Whole-ROM SHA-256 values were rechecked, vector index 176 reproduced the documented entries `0x004D4E62`, `0x005D712C` and `0x0042882C`, and the exact handler slices reproduced the established lengths/fingerprints: AS3000 `0x5A8` / `6277981d533d5a1bc0769bd48bebe382c7ce2a0b45dbb4f32a362815ab31bb34`, NEO 2005 `0x59E` / `5f60b73c6e5e8b0f33f297fa3f1e4e271c779a505db6b5873eb1c7428325e3ab`, and NEO 2013 `0x8D6` / `cf62783caa9e29b3e216263b106077432d6f8908db8a66855230a7c209b7570c`.

A fresh absolute-JSR scan reproduced exactly two callers per generation: AS3000 `0x004C9006` and `0x004C9062`; NEO 2005 `0x005C8848` and `0x005C88A4`; NEO 2013 `0x0041900A` and `0x00419066`. The resulting structural regression was **EXECUTED: 12/12 PASS** (vector target, handler fingerprint, terminal RTS and caller count for each ROM).

The source-first historical search remains negative for an independent BetaWise/neo-re vendor name or prototype. Official user/Manager manuals do not specify this low-level five-slot state/event ABI or its raw status domain, so no user-facing semantic label is projected onto `selector`, `param`, `reserved`, `status_out`, or `0x02000006`.

No contradiction was found: the physical reserved fourth slot remains confirmed unused in these three ROMs, while D0.L remains a full-width `uint32_t` contract with domain `{0, 1, 0x02000006}`.

## Regression status

Static source-first structural regression is **executed, 12/12 PASS**.

Emulator-first regression is **specified, not executed**. Required probes include varying `reserved` with all other inputs fixed, covering return values `0`, `1`, and `0x02000006`, validating 32-bit `status_out` writes, and repeating the checks on all three canonical generations.

Extended disassembly, ROM bytes and private reverse-engineering workpapers remain outside the public repository.
