# SDK audit — umbrella/app selection-mask reconciliation — 2026-09-15

Scope: `sdk/abi-automation`; safe header/docs reconciliation only.

## Finding

The earlier umbrella-exposure inventory treated `os3k/applet_selection_mask.h` as a candidate for inclusion from `os3k/os3k.h`. That is not a real visibility gap.

`os3k.h` already declares the complete closed A260–A270 selection-block mask family directly:

- `SYS_A260(uint32_t applet_index)`
- `SYS_A264(uint32_t applet_index)`
- `SYS_A268(uint32_t applet_index)`
- `SYS_A26C(uint32_t applet_index)`
- `SYS_A270(uint32_t applet_index)`

Those declarations match `applet_selection_mask.h`. Therefore adding that subheader to the umbrella solely for visibility would duplicate declarations and is not justified.

## Audit disposition

- Remove `applet_selection_mask.h` from the set of presumed umbrella visibility gaps.
- Keep the dedicated header as useful modular documentation/source surface; no ABI change is implied.
- Continue reconciling candidate subheaders contract-by-contract against direct declarations in `os3k.h` before proposing includes.
- No syscall was reopened.
- No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry was promoted.
- No header or veneer change was made in this lot.
- No m68k compilation is required for this documentation-only correction; nothing is marked `BUILD_VALIDATED`.

This supersedes only the part of the earlier applet-subheader inventory that treated A260–A270 as absent from the umbrella. It does not make any claim about other candidate families, which must be checked independently.
