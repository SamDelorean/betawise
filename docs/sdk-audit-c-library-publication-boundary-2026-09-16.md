# SDK audit: C-library publication boundary (2026-09-16)

## Scope

Audit the callable C-library block after A32C/A334 using the original BetaWise architecture as authority:

`syscall.c veneer ↔ os3k.c wrapper/adaptation ↔ os3k.h public declaration`

Reconstructed focused headers are not publication authority.

## Current BetaWise structure

`syscall.c` exposes the standard-library veneers A338-A3B0 under their C names (`abort`, `atoi`, `atol`, `sscanf`, `fgetc`, `fprintf`, `fscanf`, `memchr`, `memcmp`, `memcpy`, `memmove`, `memset`, `printf`, `rand`, `scanf`, `sprintf`, `srand`, `strcat`, `strchr`, `strcmp`, `strcpy`, `strlen`, `strncat`, `strncmp`, `strncpy`, `strrchr`, `strstr`, `tolower`, `toupper`, `ungetc`).

The only raw/private veneer in this named C-library span that requires BetaWise adaptation is `_OS3K_fputc`; `os3k.c` implements public `fputc()` so stdout can use the BetaWise display/font path while non-stdout streams fall through to the raw veneer. The same source also implements `getchar()` above this span from the private `_OS3K_getchar` path.

`os3k.h` directly declares the complete named C-library surface and declares public `fputc()`, not `_OS3K_fputc`.

## Conclusion

No safe ABI/header inconsistency is demonstrated in the named C-library publication block. The source graph follows the intended BetaWise layering:

- direct named veneer -> direct declaration in `os3k.h` when no adaptation is needed;
- private `_OS3K_*` veneer -> public wrapper in `os3k.c` -> public declaration in `os3k.h` when adaptation is required.

This is a positive control for later audits and reinforces that reconstructed per-family headers must not be introduced as an umbrella/public-header architecture.

The NEO13-only neutral A3B4+ extension block is outside this conclusion; its publication/provenance policy remains separately controlled and no neutral name is promoted here.

Documentation-only audit. No syscall is reopened, no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted, no source/header is changed, and no `BUILD_VALIDATED` status is created.
