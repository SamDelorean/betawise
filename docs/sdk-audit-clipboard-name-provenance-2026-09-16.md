# SDK audit: Clipboard symbol provenance and public-header consequence

## Material finding

The names `ClipboardSet`, `ClipboardGet`, and `ClipboardClear` are not names invented by the 2026 ABI reconstruction headers.

Git history shows that commit `1cb31c638ddc9bdcf7dc4aa19ab02fd5b5f0f5df` (2022-02-14, Kirk Meyer/isotherm, `Some new syscall research, mostly filesystem`) changed only the `syscall.c` mappings for A20C/A210/A214 from neutral `SYS_A20C`, `SYS_A210`, `SYS_A214` to `ClipboardSet`, `ClipboardGet`, `ClipboardClear`. The same commit touched `os3k.h` only for unrelated USB message enum additions; it did not add Clipboard declarations there.

The focused `os3k/file_clipboard_buffer.h` was created much later, in commit `5870b7774f387db834c9a0577846e08bb0daddd2` (2026-08-27), specifically to publish the mechanically reconstructed A20C-A214 ABI while retaining those pre-existing BetaWise symbols.

Therefore the present state has a traceable historical asymmetry:

- symbol identification in `syscall.c`: 2022;
- mechanically closed C signatures: 2026;
- public declarations in `os3k.h`: still absent.

This strengthens the classification of A20C/A210/A214 as a narrow SDK declaration-surface inconsistency. It does **not** establish that every reconstructed `SYS_Axxx` veneer should be public, and it does not justify turning reconstructed focused headers into public subheaders.

## Integration consequence

Under the preserved BetaWise architecture, the repair candidate remains direct declarations in the File/clipboard portion of `os3k.h`, using the closed signatures as evidence:

```c
int32_t ClipboardSet(const uint8_t* buffer, uint32_t count,
    uint8_t clipboard_tag);
int32_t ClipboardGet(uint8_t* buffer, uint32_t count,
    uint8_t read_variant);
void ClipboardClear(void);
```

No wrapper in `os3k.c` is indicated by the current evidence: `syscall.c` already exports the public symbols directly. This is the same architectural class as other direct veneers, not the `_OS3K_*` + wrapper class.

## Safety/build status

No source/header change is made in this lot. A future `os3k.h` edit is `PENDING_M68K_BUILD` and requires `make -C os3k clean all` plus a representative applet clean build before `BUILD_VALIDATED`.

No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted and no closed syscall is reopened.
