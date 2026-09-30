> **ARCHIVED — 2026-09-30**  
> This work plan is retained for historical/reference purposes only. The project
> direction changed after Increment 9. Increment 10 and Increment 11 are
> cancelled for this front and must not be resumed unless explicitly
> reactivated.

# DebugTool-AS2000 rigid work plan

This is the controlling execution plan for v0. Work proceeds in order.
No new work item may be inserted unless it blocks the current increment.

## Global rules

1. Scope is frozen to MEM, EDIT, GOTO, CALL and INFO.
2. Work is performed increment by increment; every increment has an explicit
   artifact and exit criterion.
3. A completed increment is not reopened unless a later deterministic test
   proves it incorrect.
4. No optional feature is implemented during v0.
5. Final RAM addresses, final ROM placement and final DynFS pointers remain
   late-bound until the relevant maps are proven.
6. The same core source must serve MAME/current ROM and later ROM.
7. Exact byte counts override estimates.
8. Codex is not part of the automatic/default workflow. It may be used only
   when the user explicitly launches/authorizes a manual Codex task for a
   narrowly defined problem.
9. Tool output and discoveries are recorded in the repository so work does
   not restart from memory or conversation history.
10. Do not broaden an increment because another interesting reverse-
    engineering question is discovered. Record only a blocking fact if needed.

## Tool policy

Use the available tools in this order of relevance:

- GitHub: source of truth, commits, diffs, fixed documentation and code history.
- Existing AS2000 disassembly/Ghidra work: locate and verify stock ROM entry
  points and data references.
- MC68HC11 documentation / assembler listings: instruction behavior, calling
  mechanics and exact byte counts.
- Existing assembler toolchain (ASxxxx/AS or established project assembler):
  authoritative build size and syntax.
- Instrumented MAME diagnostic build: deterministic execution and memory/bank
  observation.
- Existing test ROM / AS2000 emulator path: functional regression.
- Remote development machine and temporary Drive handoff: only when local
  artifacts need transfer/inspection.
- Codex: manual exception only; never an unattended planning or implementation
  dependency.

## Increment 0 — architecture freeze

Artifacts:

- ARCHITECTURE.md
- PORTING_MAP.md
- SIZE_BUDGET.md
- this WORKPLAN.md

Exit criterion:

- architecture, scope, byte ceilings, late-binding rule and DynFS diagnostic
  purpose are written and committed.

No code is written in this increment.

## Increment 1 — dependency inventory

Goal: identify only the external services the five functions require.

Determine and record candidate/verified entry points for:

- LCD clear/output/cursor
- keyboard read/decode
- bank selection / bank-visible window behavior
- safe byte read/write constraints
- debugger entry/exit mechanism
- any stock conversion helper worth reusing
- stable DynFS ABI/state symbols already available

Artifact:

- BINDINGS_INVENTORY.md

Each item must be marked:

- VERIFIED
- CANDIDATE
- UNRESOLVED

Exit criterion:

- every dependency has at least a candidate;
- unresolved items are limited to information that genuinely depends on
  unfinished DynFS/final-ROM work.

Do not implement around an unresolved item.

## Increment 2 — ABI and data-state freeze

Goal: freeze the internal representation before assembly implementation.

Define exactly:

- current bank representation
- current 16-bit address
- cursor and edit mode
- CALL input registers
- CALL captured return registers
- INFO input/state references
- shared temporary/input bytes

Artifact:

- ABI_STATE.md

Also create the first RAM ledger with byte counts but no guessed final
addresses.

Exit criterion:

- persistent RAM target <= 50 bytes;
- every byte has an owner/lifetime;
- CALL convention is unambiguous.

## Increment 3 — binding interface skeleton

Goal: isolate environment-specific dependencies.

Create:

- core symbolic interface/include
- one diagnostic/MAME binding file
- placeholders for current/future ROM bindings

No functional UI yet.

Required behavior:

- assembly succeeds with known/stub bindings
- unresolved final RAM/ROM/DynFS addresses remain explicit symbols, not magic
  constants hidden in core code

Exit criterion:

- core can be relocated/rebound without editing core logic.

## Increment 4 — MEM only

Implement only:

- bank/address state
- memory read
- fixed 40x4 compact dump
- minimal hex formatting
- redraw

Test in instrumented MAME.

Tests:

- known ROM bytes
- known RAM bytes
- bank switch observation where applicable
- display boundary behavior

Record exact ROM/RAM size delta.

Exit criterion:

- MEM deterministically displays known memory correctly;
- no EDIT/GOTO/CALL/INFO behavior is added except internal primitives strictly
  required by MEM.

## Increment 5 — GOTO only

Implement:

- 16-bit address entry in current bank
- bank + address entry
- validation required for the actual memory model

Do not add expression syntax.

Tests:

- same-bank jump
- banked jump
- boundary addresses
- cancel/return path

Record exact size delta.

Exit criterion:

- MEM + GOTO navigate deterministic test locations.

## Increment 6 — EDIT only

Implement byte editing through the existing MEM cursor/view.

Tests:

- RAM byte edit
- nibble/byte correctness
- banked RAM target
- redraw reflects physical result
- protected/invalid target behavior only to the degree required for safe v0 use

Record exact size delta.

Exit criterion:

- edited physical RAM is independently observable in MAME and MEM.

## Increment 7 — CALL only

Implement the native HC11 call probe.

Freeze and test:

- target address load
- A/B(D)/X/Y pre-call state
- call/return preservation needed by DebugTool itself
- captured A/B(D)/X/Y and CCR where practical
- safe return to DebugTool

Start with deterministic tiny test routines in the diagnostic environment,
then one verified stock ROM routine, then one stable DynFS ABI routine when
available.

Record exact size delta.

Exit criterion:

- a known test routine receives the expected register state and returns the
  expected register state;
- the debugger survives the return.

No generic six-argument compatibility layer is permitted.

## Increment 8 — INFO only

Implement the smallest DynFS state view required to diagnose the active
filesystem path.

INFO must read existing state only.

First-stage fields are selected from already-proven DynFS/legacy state; fields
that depend on unfinished structures remain bindings/placeholders rather than
blocking the whole debugger.

Tests:

- compare displayed logical state with instrumented MAME observation
- exercise at least one file/slot transition and one bank/extent-relevant
  transition when those structures are available

Record exact size delta.

Exit criterion:

- INFO exposes enough state to distinguish a logical DynFS pointer/state error
  from a physical bank/RAM error.

## Increment 9 — v0 consolidation

Perform only:

- assemble/link cleanly
- exact size audit
- duplicate-helper audit against stock ROM
- regression of MEM/GOTO/EDIT/CALL/INFO
- removal of dead code/data
- update documentation with actual byte/RAM counts

No new features.

Acceptance:

- <=1536 bytes: accept normal v0
- 1537-1792 bytes: one bounded optimization pass
- >1792 bytes: stop and redesign; do not continue adding code

Exit criterion:

- one reproducible v0 binary/core object and test record.

## Increment 10 — current-ROM integration — CANCELLED / ARCHIVED

This increment is allowed only after v0 consolidation.

Resolve only:

- placement in currently available ROM space
- actual debugger entry/exit hook
- exact free RAM workspace
- actual current DynFS ABI/state bindings

Required RAM proof:

- quantify free RAM range(s)
- document conflicting stock/DynFS users
- assign every DebugTool RAM byte
- verify no overlap in emulator traces/tests

Exit criterion:

- same v0 core runs in the current patched ROM with only binding/placement
  changes.

## Increment 11 — later-ROM relocation — CANCELLED / ARCHIVED

This increment occurs only when the later ROM/memory layout exists.

Do not redesign the debugger.

Update only:

- ROM placement
- binding addresses
- verified free RAM workspace
- changed DynFS state/ABI symbol addresses
- tiny adapter stubs only if a proven interface changed

Repeat the fixed v0 regression suite.

Exit criterion:

- byte-identical or functionally equivalent core source works in the later ROM
  without a second DebugTool implementation.

## Stop conditions

Work stops and reports instead of expanding when:

- a required ROM routine cannot be verified;
- the RAM ledger cannot prove a conflict-free workspace;
- CALL cannot return safely under the frozen convention;
- assembled size exceeds the STOP/REDESIGN threshold;
- implementing the current increment would require a sixth user-visible
  function.

The response to a stop condition is a bounded design correction, not a new
research branch.

## Progress reporting

Every increment report contains only:

1. increment number and status,
2. artifacts changed,
3. verified findings,
4. exact ROM/RAM delta when code exists,
5. blocking item, if any,
6. next increment exactly as defined above.

Percent-complete estimates are secondary; completion is determined by passed
exit criteria.
