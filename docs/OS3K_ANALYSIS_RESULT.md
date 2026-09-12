# OS3K base-analysis result — 2026-09-12

Documentation-only consolidation completed in
[os3k-runtime-memory-storage.md](os3k-runtime-memory-storage.md). No closed ABI
analysis was restarted, no inventory status was promoted, and no SDK or emulator
implementation was changed.

- **Evidence reviewed:** ABI reconstruction index and 2026-09-11 consolidation;
  current File API reference and historical reconstruction log; file core,
  space-accounting, identity/dynamic, open/close, clipboard buffer/state/capacity
  and packed-record cursor/search/writer closures; applet runtime, settings,
  selection/mask and password/token-group closures; source-first A198/A19C,
  A1C0/A1C4, A1B4/A1B8 and selection backing-store notes. Structural startup and
  Manager installation notes supply separate RAM anchors and programming-context
  boundaries. Evidence links are attached to claims in the canonical map.
- **Facts consolidated:** located runtime globals and stack anchors; `0x48`
  descriptors, `min_size`, live mirrors, namespace/token/context distinctions,
  descriptor creation/removal and renumbering; partial writes and immediate
  runtime mutation; close-only detach; clipboard sharing; contextual 512-byte
  reclaim accounting; distinct packed-record cursor edge cases.
- **Unresolved RAM questions:** complete region ownership and sizes; descriptor
  table/storage-arena roots and initialization; stack/heap separation; relocation
  and teardown lifetimes; cold/warm reconstruction and retained-power domains.
- **Unresolved persistence/storage questions:** ordinary file physical medium,
  backing-copy/dirty/commit paths, reset/off retention, write ordering, atomicity
  and recovery. RAM storage pointers, size snapshots, readback and FileClose do
  not establish durability. Manager install/update programming is a separate
  context, not an ordinary-file commit contract.
- **AS3000/NEO differences:** relocated runtime addresses and stack anchors;
  state masks `0x0D` versus NEO 2013 `0x1D`; A210 filtered-read defect in both
  2005 versions versus the 2013 correction; display backing strides 40 versus
  132; longer 2013 Manager finalizer with unresolved added purpose. Earlier
  AS3000 source remains a separate fixed-capacity/byte-identity historical layer.
- **Emulator implications:** preserve guest runtime topology, live mirrors,
  partial writes, allocator thresholds and version-specific behavior; validate
  with synthetic state. Host save-state retention is not device persistence.
- **BetaWise implications:** explicit namespace/open lifecycle, actual-count
  checking, mirror unbinding and A5 ownership; folder restoration alone does not
  restore an active file. SDK publication remains a separate handoff.
- **Spreadsheet implications:** bounded versioned serialization and capacity/
  partial-write handling can proceed; stage before destructive replacement when
  feasible and verify runtime bytes by readback. Atomic replacement and durable
  save guarantees require further evidence; clipboard/high-water are not proven
  durable backups.
- **Exact files changed:** `docs/os3k-runtime-memory-storage.md` (created) and
  `docs/OS3K_ANALYSIS_RESULT.md` (replaced). Continuity is recorded here within the
  two-file change boundary; no index, headers, implementation, applets, MAME or
  private evidence files were modified.
- **Validation:** `git diff --check` PASS (exit 0); local Markdown file-link
  targets checked. No firmware/static-regression suite was rerun and no dynamic
  test was executed. Build gates are not applicable to this documentation-only
  change; no `BUILD_VALIDATED` claim is made.
- **Recommended next frontier:** locate initialization writers and exact bounds
  of descriptor-table roots and the file-storage-end pointer, starting with
  AS3000 2005; separate demonstrated cold/default and warm/resume behavior.
  Close that ownership unit before widening to NEO or physical persistence.
