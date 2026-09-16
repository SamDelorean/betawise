# SDK audit: FileGetFileInfo name provenance (A1C4)

## Corrected result

A1C4 is mechanically closed and currently exported/deployed as `FileGetFileInfo` in `syscall.c` and `os3k.h`. A previous audit note classified the spelling as merely reconstruction-era because commit `38b622ce70af52b46282d19a129431f30e3c39a0` performs the veneer rename. That classification was too strong and is superseded by the primary historical evidence already recorded immediately before the rename.

Commit `11b525e24549f97751aa39771e86d30cadc6a619` documents the recovered AS3000 2000 `FileModule.h` prototype:

```c
UInt8_p FileGetFileInfo(
    UInt32_p file_size,
    UInt32_p cursor,
    UInt32_p max_size);
```

The historical implementation returns the active file storage pointer. The modern A1C4 handler preserves that core contract while extending it with an explicit file token, a storage-pointer mirror, and persistent live mirror bindings. The same reconstruction note therefore records:

- ABI/behavior confidence A;
- name continuity confidence B (strong);
- the name comes from primary historical source;
- no later OS3K symbol table containing the textual name has been recovered.

Commit `38b622ce70af52b46282d19a129431f30e3c39a0` subsequently changes the raw veneer from `SYS_A1C4` to `FileGetFileInfo`; it is the integration point for a name whose genealogy had already been established, not the origin of an unsupported descriptive invention.

## Classification

`PUBLIC_COMPILATION_SURFACE / HISTORICAL_AS3000_NAME_RECOVERED / MODERN_OS3K_NAME_CONTINUITY_STRONG_NOT_SYMBOL-TABLE-PROVEN`

This is intentionally different from reconstruction-era descriptive-name cases such as A1A4 `FileDeleteChars`, where the audited rename commit itself supplies no comparable recovered historical declaration.

## Architectural consequence

`FileGetFileInfo` may be retained as a historically grounded SDK name. Its modern five-argument ABI must not be confused with the older three-output AS3000 ABI: name genealogy is strong, while the modern signature is independently reconstructed from OS3K handlers/callers.

The historical name also must not be used as a blanket precedent to assign names to unrelated neutral `SYS_Axxx` contracts. Publication still requires per-contract evidence.

No source/header change is required by this correction.

## Safety/build status

Documentation-only correction of the audit record. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, no source/header change is made, and nothing is newly `BUILD_VALIDATED` or `PENDING_M68K_BUILD`.
