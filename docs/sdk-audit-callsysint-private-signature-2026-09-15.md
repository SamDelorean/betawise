# SDK audit — CallSysInt private veneer signature drift

Date: 2026-09-15
Branch: `sdk/abi-automation`

## Material finding

Preserving the original BetaWise SDK layering exposed a concrete source-level inconsistency in `os3k/os3k.c`.

The historical BetaWise/master implementation declares the private A2B8 veneer as:

```c
uint32_t _OS3K_CallSysInt(uint32_t unused_zero, SysInt_e info, void* output);
```

and the public wrapper as:

```c
uint32_t CallSysInt(uint32_t unused_zero, SysInt_e info, void* output);
```

On `sdk/abi-automation`, the public wrapper remains `uint32_t`, but the private veneer declaration has drifted to:

```c
uint8_t _OS3K_CallSysInt(uint32_t unused_zero, SysInt_e info, void* output);
```

`syscall.c` still maps A2B8 to `_OS3K_CallSysInt`.

This is a real `.c <-> historical BetaWise structure` inconsistency, independent of any umbrella-header inventory. Even if reconstructed firmware evidence eventually proves that only a low byte is semantically meaningful for some selector paths, narrowing the private C declaration changes compiler-visible return semantics and should not be done silently while the public BetaWise wrapper remains 32-bit.

## Safe disposition

Do not reopen A2B8 merely to rename or reinterpret it. Treat the signature drift as a concrete dependency requiring reconciliation against the already-recorded A2B8 mechanical contract and the original BetaWise wrapper architecture.

The conservative repair candidate is restoration of the historical private declaration to `uint32_t`, unless existing closed ABI evidence explicitly requires the narrower compiler-visible type. Any source change must pass the real m68k build gate before `BUILD_VALIDATED`.

No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted by this finding.

## Status

`AUDIT_FINDING / PENDING_M68K_BUILD_FOR_SOURCE_CHANGE`

No source/header change is made in this audit batch; no `BUILD_VALIDATED` claim.
