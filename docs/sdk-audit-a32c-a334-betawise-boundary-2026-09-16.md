# SDK audit: A32C-A334 BetaWise publication boundary (2026-09-16)

## Scope

Audit A32C-A334 using the original BetaWise structure as publication authority:

`historical BetaWise source/history ↔ syscall.c veneer ↔ os3k.c wrapper (if any) ↔ os3k.h public surface`

Reconstructed focused headers are not publication authority.

## Evidence

Historical BetaWise filesystem-era `syscall.c` at commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` already contains:

- A32C as neutral `SYS_A32C`
- A330 as private veneer `_OS3K_getchar`
- A334 as neutral `SYS_A334` with the historical comment `getchar with echo`

The current branch preserves the same three veneer identities.

Current `os3k.c` implements public `getchar()` itself by scanning the keyboard and translating keys. It does not expose `_OS3K_getchar` directly and does not wrap A334.

Current `os3k.h` publishes:

```c
uint32_t SYS_A32C(void);
int getchar(void);
int32_t SYS_A334(void);
```

## Conclusion

There is no safe declaration repair in A32C-A334.

- A330 follows a demonstrated BetaWise raw/private-veneer -> public-wrapper pattern: `_OS3K_getchar` stays private while `getchar()` is the public SDK function.
- A32C and A334 are already on the public compilation surface under neutral names. Their presence does not recover historical vendor/public identities and must not be used as precedent to promote other neutral `SYS_Axxx` entries.
- Renaming A32C or A334 would require independent provenance and is outside this safe audit lot.
- Removing either existing neutral declaration would be a source-compatibility change and likewise requires consumer/build evidence.

Classification:

- A32C: `PUBLIC_COMPILATION_SURFACE / BETAWISE_NEUTRAL_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`
- A330: `BETAWISE_PRIVATE_VENEER / PUBLIC_WRAPPER_GETCHAR`
- A334: `PUBLIC_COMPILATION_SURFACE / BETAWISE_NEUTRAL_NAME / HISTORICAL_PUBLIC_IDENTITY_UNPROVEN`

No syscall is reopened and no ABI closure status changes.

Documentation-only audit; no m68k build is required and no `BUILD_VALIDATED` status is created.
