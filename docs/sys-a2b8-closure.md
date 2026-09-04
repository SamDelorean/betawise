# A2B8 system-internal dispatcher closure

## Status

`A2B8` is mechanically closed with **confidence A** across the canonical
AlphaSmart 3000 2005, NEO 2005 and NEO/System 3.15 (2013) ROMs.

BetaWise has historically exposed this trap as `_OS3K_CallSysInt`. That symbol
is useful corroborating evidence from the 2021 BetaWise tree, but it is not
presented here as a recovered original vendor symbol.

## Raw ABI contract

```c
uint8_t _OS3K_CallSysInt(uint32_t unused_zero, SysInt_e info, void *io);
```

Mechanically:

- three 32-bit argument slots are present;
- the first slot is not read by any of the three compared handlers;
- the low word of the second slot selects the service;
- the third slot is a pointer whose direction and payload depend on the
  selector, so a single narrower pointee type would be misleading;
- only `D0.B` is contractual: `1` means the selector was accepted and `0`
  means it was rejected.

The upper 24 bits of `D0` are **not** part of the raw return contract. In
particular, rejection paths clear only `D0.B`. A NEO 2013 caller copies `D0.B`
immediately after the call, independently confirming the byte-sized return.

This corrects the older BetaWise raw forward declaration that used `uint32_t`
for `_OS3K_CallSysInt`. The higher-level `CallSysInt` wrapper may continue to
widen the byte result for source compatibility, but it must do so from a
byte-typed raw declaration.

## Generational dispatch

The service family evolves substantially while retaining the same three-slot
ABI.

- **AS3000 2005** supports the old subset `0x0000`, `0x0300`, `0x0400`,
  `0x0500`, `0x0600`.
- **NEO 2005** supports selector indices `0x00..0x06` (encoded as
  `0x0000..0x0600`).
- **NEO 2013** uses a sparse 24-entry dispatcher accepting
  `00..10, 20, 30, 40, 50, 80, 90, A0` in the high selector byte.

The existing `SysInt_e` names are historical BetaWise names. They are retained
where their mechanics corroborate them; unnamed selectors remain numeric rather
than receiving invented meanings.

### Directly corroborated examples

The following behaviors are mechanically established and align with the
historical family:

- `0x0300` follows the row/font-height route;
- `0x0400` returns the generation-specific display-height quantity;
- `0x0500` returns the generation-specific display-width quantity;
- `0x0600` follows the row-count route;
- NEO 2013 `0x0700` returns a system-font pointer;
- NEO 2013 `0x0800` follows the ROM-size query route.

The AS3000 constants are specifically `0/0x0300 -> 1`, `0x0400 -> 4`,
`0x0500 -> 40`, and `0x0600 -> 4`. This corrects an earlier provisional
workpaper summary that misread two branch targets.

Some selectors use `io` as an output, while others read a scalar byte from the
same pointed storage before calling a setter/helper. Consequently `void *io`
is the conservative cross-selector contract.

## Handler boundaries and fingerprints

Exact closed handler lengths and literal SHA-256 fingerprints are:

- AS3000 2005: `0x3E` bytes,
  `9732f69efcbd78a6a074520c42d2ca1836e02b0e7cd512a5f839eba427a8c490`;
- NEO 2005: `0xAC` bytes,
  `e46275015ba1ad29f12778a84bf04497b037b976e35a4ffcf770761c37cc8189`;
- NEO 2013: `0x22A` bytes,
  `47ddf92a34efb2c1b74c17a67d363ddc50db1a7625ce9ec2c95013d043820ec4`.

These implementations are intentionally **not** normalized to one common hash:
the dispatcher genuinely grows across generations rather than differing only by
relocations.

## Caller evidence

NEO 2013 contains seven direct absolute JSR callers to the raw handler. They
construct three argument slots, pass zero in the first slot, use selectors such
as `0x0800`, `0x0900`, `0x0E00` and `0x0F00`, and provide an output/context
pointer in the third slot. At least one caller consumes `D0.B` immediately.

No absolute JSR/JMP caller to the implementation was established in the
AS3000/NEO 2005 ROMs; this does not negate the trap ABI.

## Source-first re-audit — 2026-09-04

The closure was re-audited from the canonical firmware rather than accepted from
its historical BetaWise identity. All three canonical ROM hashes were
revalidated, vector index 174 was re-read independently, handler boundaries were
re-extracted, and the literal handler fingerprints above reproduced exactly.
Direct absolute JSR scanning also reproduced the prior caller result: `0/0/7`
for AS3000 2005 / NEO 2005 / NEO 2013, including the same seven NEO 2013 call
sites.

Historical BetaWise source continues to corroborate `_OS3K_CallSysInt` and the
`SysInt_e` family, while a fresh nominal search recovered no independent vendor
symbol. User/Manager manuals are useful for user-visible properties reached by
individual selectors, but they do not define this internal dispatcher's raw
stack ABI; firmware therefore remains the controlling evidence for argument
widths, return width and selector dispatch.

No material contradiction was found. The raw `uint8_t` return, unused first
slot, selector-dependent `void *io`, and genuine generation-specific dispatch
remain confirmed.

## Adversarial conclusions

Rejected alternatives:

- **32-bit raw return** — rejected because failure paths define only the low
  byte and a real caller consumes `D0.B`.
- **meaningful first argument** — rejected for the compared handlers; no read of
  the first caller slot was found.
- **uniform `uint32_t *` output** — rejected because selector-specific paths use
  the third argument as both input and output with different payload forms.
- **one cross-generation implementation** — rejected; the three CFGs have
  materially different dispatch breadth.

## Regression status

A source-first static structural regression was **EXECUTED: 15/15 PASS**. For
each of the three canonical ROMs it independently checked the full-ROM SHA-256,
vector[174] target, handler SHA-256, terminal RTS boundary, and expected direct
JSR count.

The emulator-first dynamic regression remains **SPECIFIED / NOT EXECUTED**. It
should verify at minimum:

1. an unsupported selector returns low byte `0` without treating upper `D0`
   bits as contractual;
2. a supported simple query returns low byte `1` and the expected
   generation-specific output;
3. changing the first argument away from zero does not affect observed handler
   behavior;
4. setter-style selectors read the intended low byte from the pointed scalar;
5. NEO 2013 sparse-selector rejection does not execute a case body.

## Publication boundary

This document contains reconstructed contract and fingerprints only. ROM bytes,
verbatim disassembly, complete private xrefs and helper corpora remain in the
private Drive workpapers.
