# SDK audit: Clipboard rollback verification (2026-09-16)

## Verified branch state

The previously reported malformed Clipboard integration and rollback have now been independently checked against GitHub rather than assumed from continuity notes.

Commit `398b94545845363bbc651569feebc21ef83483ed` did add the intended A20C/A210/A214 direct declarations, but its patch also removed the continuation backslashes after `.fileUsage = 0` and `.entryPoint = &BwProcessMessage` inside `APPLET_HEADER_BEGIN`. It is therefore invalid as an SDK/header integration commit.

Commit `ca9f6be142123dfd26b8f3c699e348ac95b70cdf` exists and records that regression and the rollback to safe commit `85fd6df2b5a8efef1c6e99f17390afbeaefa71b3`.

The current `sdk/abi-automation` branch has subsequently advanced through documentation-only audit commits. Its current `os3k/os3k.h` blob is `45cc35f1472ed4ff0dfdf90b1ae6a1e118b05d37`.

Direct inspection of that current blob confirms both required safety conditions:

1. `ClipboardSet`, `ClipboardGet` and `ClipboardClear` are absent, so the failed integration did not survive the rollback.
2. `APPLET_HEADER_BEGIN` retains the continuation backslashes after both `.fileUsage = 0` and `.entryPoint = &BwProcessMessage`, so the macro regression is absent from the current branch.

## Audit consequence

A20C/A210/A214 remain the single demonstrated direct-public-declaration repair candidate in this filesystem region under the original BetaWise architecture rule. The corrected edit must be a minimal direct insertion in `os3k.h`; it must not include `file_clipboard_buffer.h` or otherwise adopt reconstructed per-family headers as SDK architecture.

Because `os3k.h` is a complete-file replacement through the repository API, the next source edit must preserve the current blob exactly outside the declaration insertion and must be immediately verified against the resulting commit diff before it is accepted.

The eventual source/header change remains `PENDING_M68K_BUILD` until `make -C os3k clean all` plus a representative applet clean build are actually run with the m68k toolchain. This verification itself is documentation-only and creates no `BUILD_VALIDATED` status.

No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry is promoted and no closed syscall is reopened.
