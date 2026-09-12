# OS3K foundational analysis automation rules

## Scope

- Work only on branch `os3k/base-analysis`.
- This branch is the foundational/transversal OS3K reverse-engineering front.
- Do not use this branch for ordinary BetaWise SDK integration batches.
- Treat `docs/abi-reconstruction-index.md` and consolidated ABI documentation as authoritative existing project state.
- Continue from existing source-first reverse engineering; do not restart already closed analysis without new discriminating evidence.
- Prefer one bounded evidence question per iteration.

## Evidence discipline

- Preserve the distinction between mechanically demonstrated fact, historical/cross-version continuity, implementation inference, and unresolved evidence.
- Preserve `CERRADA`, `CERRADA_CON_LÍMITE_DE_EVIDENCIA`, `NO_SYSCALL/NO_CALLABLE`, and unresolved classifications.
- Never promote limited evidence merely to produce a cleaner model.
- Keep neutral `SYS_Axxx` names when vendor identity is not independently established.
- Keep AS3000, early NEO and NEO 2013 differences explicit.
- ioma8/neo-re, Isotherm and other external projects are comparative references, not automatic authority.

## Private evidence

- Local ROMs, firmware, Ghidra projects, Manager distributions, dumps and captures may be READ as evidence when available.
- Never add them, extracted proprietary payloads, or extensive proprietary disassembly to Git.
- Publish only independently derived documentation, interfaces, algorithms, addresses, constants, synthetic tests and other redistributable project work.

## Cross-project discipline

- Record emulator, SDK, hardware and application implications as handoffs.
- Do not modify MAME repositories from this branch.
- Do not modify BetaWise SDK headers or implementation unless a later explicitly authorized task changes this boundary.
- Do not merge or rebase `sdk/abi-automation` automatically into this branch.

## Git safety

- Start from a clean worktree.
- Fetch `origin/os3k/base-analysis` before work and before publishing.
- Abort if local and remote have diverged.
- Never force-push.
- Commit and push only coherent evidence-preserving work.
