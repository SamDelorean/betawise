# SDK audit correction — A2B8 CallSysInt raw/private signature

## Status

`SUPERSEDES_AUDIT_FINDING` for `docs/sdk-audit-callsysint-private-signature-2026-09-15.md`.

No ABI status changes. A2B8 remains `CERRADA` with confidence A. No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted.

## Correction

The previous audit note treated the change of the private `_OS3K_CallSysInt` declaration from historical `uint32_t` to `uint8_t` as an unresolved signature drift. That conclusion was incorrect because it compared the branch against historical BetaWise source without first reconciling the already-closed raw A2B8 contract.

`docs/sys-a2b8-closure.md` is explicit: the raw trap contract is

```c
uint8_t _OS3K_CallSysInt(uint32_t unused_zero, SysInt_e info, void *io);
```

Only `D0.B` is contractual; upper D0 bits are not part of the raw return contract. The closure also explicitly records that the historical BetaWise `uint32_t` raw forward declaration was incorrect and that the public `CallSysInt` wrapper may retain `uint32_t` for source compatibility by widening the byte result.

The current branch therefore has the intended layering:

- `syscall.c`: A2B8 veneer symbol `_OS3K_CallSysInt`;
- `os3k.c`: private declaration returns `uint8_t`, matching the closed raw ABI;
- `os3k.c` / `os3k.h`: public `CallSysInt(...)` returns `uint32_t`, preserving historical BetaWise source compatibility.

This is a concrete example of the audit rule now in force: preserve BetaWise SDK structure and source compatibility, but do not preserve a historical raw declaration when the mechanically closed ABI has already demonstrated it incorrect. Wrapper and veneer return types need not be identical when the wrapper deliberately adapts/widens the raw contract.

## Action

Do **not** restore `_OS3K_CallSysInt` to `uint32_t`. No source/header change is required for A2B8 from this finding. The previous `PENDING_M68K_BUILD_FOR_SOURCE_CHANGE` recommendation for restoring this declaration is withdrawn.

This correction is documentation-only. No m68k build was executed in this audit lot and no new `BUILD_VALIDATED` claim is made.
