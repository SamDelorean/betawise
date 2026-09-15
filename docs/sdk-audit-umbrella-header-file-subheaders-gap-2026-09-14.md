# SDK audit — umbrella header does not expose closed File API subheaders

Date: 2026-09-14
Branch: `sdk/abi-automation`

## Finding

The reconstructed File API has mechanically closed contracts in dedicated public headers, including:

- `os3k/file_password_state.h` — A1D0, A1D4, A1D8, A1DC
- `os3k/file_dynamic.h` — A1E8, A1EC, A1F0, A1F4, A1F8, A1FC
- `os3k/file_clipboard_edit.h` — A200, A204, A208

The main SDK umbrella header `os3k/os3k.h` currently declares the earlier File API primitives and the password dialogs, but does not include these dedicated headers or duplicate their declarations. Therefore an application that follows the normal single-header SDK pattern (`#include <os3k.h>` / project-equivalent include) cannot see these already-closed contracts without knowing the reconstruction-specific subheader names.

This is a publication/integration gap, not an ABI-reconstruction gap.

## Safety boundary

Only headers whose entries are already `CERRADA` are candidates for umbrella exposure. This finding does **not** authorize adding any `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry to `os3k.h`.

The safest integration is to include the dedicated closed-contract headers from `os3k.h` rather than duplicate prototypes. That keeps one declaration source per raw contract and reduces drift.

## Build status

No m68k SDK build is claimed in this audit run. Adding umbrella includes changes compilation visibility and therefore requires a real toolchain build before it may be called `BUILD_VALIDATED`.

Recommended next bounded action:

1. inventory all dedicated closed-contract headers under `os3k/`;
2. classify each as safe umbrella include vs intentionally private;
3. add only the safe includes;
4. compile the SDK/applets with the m68k toolchain;
5. do not mark the integration `BUILD_VALIDATED` until that build succeeds.
