# A2E0 mechanical closure

Status: **mechanical confidence A**. Original vendor name unknown.

## Neutral contract

```c
uint32_t SYS_A2E0(void *state);
```

A2E0 consumes exactly one physical 32-bit stack slot. The argument is a mandatory mutable state pointer. One equivalent internal caller per canonical ROM pushes exactly one pointer, invokes A2E0, cleans four stack bytes, and ignores the returned register.

## Handler evidence

- AS3000 2005: runtime `0x004D4972`, file `+0x014972`, length `0x4E`, raw SHA-256 `207990394fc7fb980372802ca85347c354c9728eb5b5d34065958ede6a0f9c43`.
- NEO 2005: runtime `0x005D6C3C`, file `+0x016C3C`, length `0x4E`, raw SHA-256 `0994caf314def05f2e092908181bfc64421e280a3a9189cea1cfa2e250d2cbde`.
- NEO 2013: runtime `0x0042830E`, file `+0x01830E`, length `0x58`, raw SHA-256 `7a715246a47b977900c9e134a5952600fd3c01490d12860c13544e747b1472d8`.

AS3000 and NEO 2005 differ only in two absolute helper relocations. Neutralizing those operands gives common SHA-256 `ebd3a368aafa2e9fecb4ab46e71de2ddd6b9017191251419f9f2ccd6fdd00511`. NEO 2013 adds two real preparation calls before the otherwise equivalent core, so that generational difference is not normalized away.

Each function has one RTS and all local branches remain within the established handler body.

## Control flow and return

A non-zero word at `state+0x4A` goes directly to an explicit `D0=0` exit.

When the guard is clear, A2E0 first calls a correlated helper with the physical shape equivalent to `[state, 0x0200]`. That helper has a single functional exit and converges on `MOVEQ #0,D0` in all three canonical ROMs. Therefore A2E0's apparent branch that could propagate a non-zero first-helper result is unreachable with the compared implementations.

A2E0 then invokes a second helper with three values equivalent to `[zero_extend(*(uint16_t *)state), state+0x18 value, 0]`. The correlated helper clamps its computed return to a non-negative range before returning. A2E0 tests that result as signed and contains a defensive literal `0x02000004` path for a negative value. Because the correlated helper has already clamped negative results away, that branch is not reachable in the three canonical ROMs. The reachable path returns zero.

Thus the exact observed `D0.L` domain is the singleton:

```text
0x00000000
```

A2E0 nevertheless remains `uint32_t` rather than `void`: the firmware deliberately propagates/tests helper statuses and contains a 32-bit defensive status literal. This is an intentional status contract even though the presently reachable domain collapses to zero.

## State effects and generation differences

The first helper mutates fields in the supplied state object, so the argument is not `const`.

AS3000 2005 and NEO 2005 share the same external body modulo relocations. NEO 2013 adds two preparation calls before the common core when the `+0x4A` guard is clear. The one-argument ABI and reachable return value remain unchanged.

## Caller evidence

Equivalent direct absolute callers are present at:

- AS3000: `0x004C9074`
- NEO 2005: `0x005C88B6`
- NEO 2013: `0x00419078`

Each caller uses `PEA 8(SP)` to pass one local/context pointer, executes A2E0, removes four bytes, and does not consume `D0`.

## Adversarial review

- The presence of literal `0x02000004` alone is not evidence that it is reachable; helper return constraints refute that interpretation for the compared firmware.
- The function is not declared `void` merely because its reachable result is always zero.
- No second argument is introduced; neither handler stack access nor concrete callers support one.
- `state` is not marked read-only because the helper chain mutates it.
- No descriptive vendor name is inferred from neighboring system calls.

## Regression status

Emulator-first regression is **specified, not executed**. Cases should cover the `state+0x4A` guard, helper state mutation, lower/upper clamp cases in the second helper, explicit `D0.L==0`, and the extra NEO 2013 preparation path.

## 2026-09-04 source-first re-audit

The prior private checkpoint was incomplete, so A2E0 was independently revalidated from the canonical ROMs before accepting this closure. Canonical ROM identities and exact handler fingerprints reproduced 3/3. A fresh whole-ROM control-transfer scan found exactly one absolute `JSR` caller in each generation (`0x004C9074`, `0x005C88B6`, `0x00419078`), correcting the earlier private statement that the ROM search was negative. The three caller windows are equivalent: each pushes one pointer with `PEA 8(SP)`, calls A2E0, then removes exactly four argument bytes.

The private correlated AS3000 disassembly was also re-read: helper `0x004D98DE` has a single terminal path converging on `MOVEQ #0,D0`, while helper `0x004E10D2` clamps its result non-negative before returning. This independently confirms that the handler's defensive non-zero propagation and negative-result `0x02000004` branches are unreachable in the compared firmware. NEO helper counterparts preserve those return constraints; NEO 2013's two added preparation calls do not alter the external result.

Static structural/correlation regression: **15/15 PASS** (3 canonical ROM identities, 3 exact handler fingerprints, 3 exact lengths/terminal epilogues, 3 concrete one-slot caller shapes, 3 helper-return constraints). Dynamic/emulator regression remains **specified / not executed**.

Private ROM-derived disassembly and helper workpapers remain outside the public repository.
