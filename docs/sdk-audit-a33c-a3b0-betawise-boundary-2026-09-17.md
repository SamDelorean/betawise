# SDK audit: A33C-A3B0 BetaWise publication boundary (2026-09-17)

## Scope

Audit the mechanically closed C-library range A33C-A3B0 against the original BetaWise SDK structure. Reconstructed focused headers are evidence only and do not define public-header architecture.

## Result

No safe SDK source/header correction is required in this range.

The current `syscall.c` maps A33C-A3AC directly to the standard C-library symbols `atoi`, `atol`, `sscanf`, `fgetc`, `fprintf`, `_OS3K_fputc`, `fscanf`, `memchr`, `memcmp`, `memcpy`, `memmove`, `memset`, `printf`, `rand`, `scanf`, `sprintf`, `srand`, `strcat`, `strchr`, `strcmp`, `strcpy`, `strlen`, `strncat`, `strncmp`, `strncpy`, `strrchr`, `strstr`, `tolower`, `toupper`, and `ungetc` at A3B0. The only private veneer in the block is `_OS3K_fputc`; BetaWise correctly supplies the public `fputc` adaptation in `os3k.c`.

`os3k.h` directly declares the corresponding public C-library surface through `int ungetc(int c, FILE *stream);`. This matches the BetaWise organization: direct veneers are declared directly; the private `_OS3K_fputc` veneer is hidden behind the existing public wrapper.

The canonical reconstruction continuation classifies A33C-A3B0 as `CERRADA`. The focused `ungetc` closure independently records `int ungetc(int c, FILE *stream)` and explicitly states that the existing `os3k.h` prototype matches the reconstructed contract. No limited-evidence entry is promoted by this audit.

## Important boundary

A338/`abort` is outside this conclusion despite lying adjacent to the C-library range: it remains `CERRADA_CON_LÍMITE_DE_EVIDENCIA` for the exact vendor C prototype. Its existing `abort(void)` declaration is compatibility surface and must not be treated as proof that limited-evidence contracts may be promoted.

## Build status

Documentation-only audit. No m68k build is required and no new `BUILD_VALIDATED` status is created.
