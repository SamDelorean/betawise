# BetaWise / OS3K automation rules

## Scope
- Work only on branch `sdk/abi-automation`.
- Treat `docs/abi-reconstruction-index.md` and the consolidated ABI documentation as authoritative project state.
- Continue from existing source-first reverse engineering; do not restart already closed analysis without new discriminating evidence.
- Prefer one small, mechanically closed task per iteration.

## Evidence discipline
- Preserve the distinction between `CERRADA`, `CERRADA_CON_LÍMITE_DE_EVIDENCIA`, `NO_SYSCALL/NO_CALLABLE`, and unresolved evidence.
- Never promote a limited-evidence contract to a stronger public C prototype without new supporting evidence.
- Keep neutral `SYS_Axxx` names when historical/vendor symbol identity is not independently established.
- Clearly separate observed firmware/device evidence from implementation inference.
- ioma8/neo-re and other external projects are comparative references only; do not copy speculative behavior as fact.

## Legal/publication rules
- Do not add proprietary ROMs, firmware images, application binaries, dumps, Manager distributions, or captured proprietary payloads to Git.
- Publish only independently derived interfaces, documentation, source code, algorithms, addresses, constants, and test scaffolding that are safe to redistribute.

## Build gates
- Before publishing ABI/header changes, run a clean `make -C os3k clean all`.
- Build at least one representative applet against the resulting library.
- Do not call a change `BUILD_VALIDATED` unless both gates pass.
- Do not assume the repository root is safe for parallel `make -j`; `os3k` and `applets` currently have no proven dependency ordering.

## Change discipline
- Prefer closed functions/contracts that do not require wrappers or unrelated file changes.
- Increase difficulty gradually.
- Keep emulator-facing findings documented even when they are not appropriate for the public SDK header.
- Do not expose `A468` or `A46C` as SDK declarations unless a concrete consumer requires them and the evidence supports the exact C contract.
- Update relevant continuity/consolidation documentation with each integrated batch.

## Git safety
- Start each publishing cycle from a clean worktree.
- Fetch `origin/sdk/abi-automation` before pushing.
- If the remote advanced, rebase cleanly; never force-push over concurrent reverse-engineering work.
- Commit and push only coherent, validated changes.
