# System 3 / OS3K SYS_A248 — A248 SDK consolidation

This note is the developer-facing consolidation record for A248, currently exposed with the neutral name:

```c
uint8_t SYS_A248(void);
```

## 0. Consolidation status

A248 reached this SDK-consolidation pass after the reverse-engineering process had already closed its mechanical contract with confidence **A**. This pass does not repeat ROM/disassembly work. It audits the delivered contract against the current BetaWise declaration, A-line stub and the already-published A248–A258 runtime closure, then records the contract application code may rely on.

Current state:

- mechanical contract: **A**, received from the reverse-engineering process;
- original modern public symbol: **not recovered**;
- public SDK identity: neutral `SYS_A248` retained intentionally;
- BetaWise declaration/stub: **implemented and audited**;
- additional wrapper: **not required**;
- operation type: **read-only global state query**;
- dynamic emulator/hardware regression: **pending**.

If later primary evidence recovers the original symbol or changes the upstream mechanical contract, this SDK record must be reconciled rather than silently preserving the neutral name or an obsolete conclusion.

## 1. Public contract

```c
uint8_t SYS_A248(void);
```

A248 takes no arguments and returns the raw low-byte global state controlling System 3 file-password protection.

Normal System 3 UI and callers use the state as an on/off value:

- `0` — file-password protection disabled;
- nonzero — file-password protection enabled.

One status/control caller compares the value explicitly with `1`, so `0/1` is the normal firmware convention. The trap itself, however, is a raw getter: it does not normalize or clamp the stored byte.

## 2. Return-width contract

The resident handler moves only one byte of the protection-state global into D0. It does not clear or otherwise define the upper 24 bits of D0.

Therefore the portable SDK type is deliberately:

```c
uint8_t
```

Application code must not depend on upper return bits.

## 3. State semantics

The state byte is paired with the global master-password runtime state. Cold/default initialization clears the protection byte while initializing the master-password buffer from the firmware default.

The strongest semantic caller is `PasswordVerifyFileDialog` / A1E0: it queries A248 and bypasses the per-file password challenge path when the returned byte is zero; a nonzero value enables the protected path.

System 3 localized UI also contains explicit ON/OFF status text for file-password protection and flows for enabling/disabling the setting. Together these callers and strings establish the meaning of the byte without requiring a recovered original symbol name.

## 4. Relationship to A258

A248 and A258 operate on the same global byte:

```c
uint8_t SYS_A248(void);
void SYS_A258(uint8_t protection_state);
```

A258 is the raw setter and A248 is the raw getter. The setter performs no built-in authorization or Boolean normalization; higher-level password-gated flows own authorization.

This separation matters for SDK users: reading A248 is non-destructive, while changing the state through A258 mutates security-related runtime behavior and should be tested with restoration of the previous value.

## 5. BetaWise integration

The working branch already exposes the reconstructed ABI in `os3k/password_runtime.h`:

```c
uint8_t SYS_A248(void);
```

`os3k/syscall.c` maps A-line index 146 directly to `SYS_A248`, corresponding to trap A248.

No behavioral source change or wrapper is required by this consolidation pass because the correct byte-width declaration and direct stub were already present.

## 6. Cross-ROM stability

The upstream closure compared the A248 implementation in the three canonical firmware generations used by this project:

- AlphaSmart 3000, November 2005;
- NEO, November 2005;
- NEO/System 3.15, July 2013.

The public contract is stable across all three: no arguments, raw low-byte state return, and no state mutation. Differences are limited to relocated runtime-global addresses.

## 7. Naming policy

The behavior is closed, but the original modern symbol has not been recovered with sufficient confidence. The SDK therefore keeps `SYS_A248` instead of inventing a descriptive API name.

Documentation may describe it as the **file-password-protection state getter**, but that description is not promoted to a source-level symbol until evidence supports the original name.

## 8. Relationship to C and the BetaWise runtime

A248 is not an ISO C or hosted-library facility. It is a direct System 3 resident service exposed by BetaWise through an A-line trap stub.

A host-side preference flag, configuration variable or generic Boolean getter is not semantically equivalent because A248 reads the actual firmware runtime state that gates System 3 file-password behavior.

## 9. Regression specification

A248 is read-only and is therefore suitable for low-risk emulator or hardware regression.

Minimum useful checks:

1. call A248 in a known/default state and confirm the low-byte value expected by System 3;
2. in a controlled test, save A248, set `0` through A258, verify A248 returns `0`, then restore the saved value;
3. repeat with normal enabled state `1`;
4. confirm A1E0 follows the protected/unprotected path consistently with A248;
5. do not treat any upper D0 bits as part of the result.

These are regression requirements, not a claim that the tests have already been executed.

## 10. Canonical status

**RECIBIDA DEL PROCESO PROFUNDO — MECÁNICA A → CONSOLIDADA → IMPLEMENTADA/AUDITADA → PUBLICADA → PROBE/REGRESIÓN PENDIENTE**

For the broader neighboring runtime-services evidence, see [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md). The public prototype is in `os3k/password_runtime.h`.
