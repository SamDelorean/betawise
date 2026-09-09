# OS3K A338 / index 206 — abort-like runtime re-entry

Status: **PARCIAL_CERRADO / NO_RESUELTO** for the exact C prototype and higher-level semantic names; **CERRADO A mechanically across AS3000 / NEO05 / NEO13**.

This note reconciles the original three-generation A338 closure with the later NEO13 dual-entry re-entry analysis. It records reproducible functional evidence without publishing ROM bytes or private disassembly corpora.

## Source-first identity evidence

Historical BetaWise declares:

- `void abort(void);` in `os3k/os3k.h`;
- `DEFINE_SYSCALL(206, abort);` in `os3k/syscall.c`;
- the wrapper macro emits the nominal A-line word `0xA000 + 4*index`, making index 206 correspond to A338.

The later `ioma8/neo-re` BetaWise SDK copy preserves the same declaration and index mapping. This makes `abort` a reproducible historical API/analyst name for index 206. It is not promoted here to an original AlphaSmart vendor symbol.

The exact C prototype is deliberately **not** promoted from that header alone. The official SmartApplet caller survey found two executable A338 callers, one in the AS3000 KeyWords applet and one in the NEO Keywords applet. Both explicitly push one zero longword before the trap and clean four bytes afterward, while the handler itself performs no caller-stack read. The zero slot may therefore be ignored padding/legacy calling convention or an undocumented argument; the available evidence does not distinguish those alternatives. `void abort(void)` remains **PROTOTIPO_C_BLOQUEADO / NO_RESUELTO**.

## Three-generation canonical firmware closure

The original forward-sweep closure verified the three canonical firmware generations directly:

- AS3000: handler runtime `0x004E0746`, file `+0x020746`;
- NEO05: handler runtime `0x005E30A2`, file `+0x0230A2`;
- NEO13: handler runtime `0x004394A0`, file `+0x0294A0`.

All three implement the same 0x10-byte handler skeleton after relocation of the current-applet/state byte and the generation-specific internal transfer target. The relocation-normalized handler SHA-256 is:

`4de38f1f1ecc29b60db43ffe1cb76393423d42f0e2724a2c9344762aa4a3ffa3`

Mechanically the handler:

1. sets bit 7 of a generation-specific runtime byte;
2. calls an internal trampoline/re-entry entry;
3. has a physical `RTS` after the call in the handler image.

The trampoline replaces the active stack pointer, clears `A6`, and transfers control onward by `JMP`, abandoning the caller frame. Thus the syntactic `RTS` after the call is not reachable through ordinary call/return flow.

The original three-generation static regression was **EJECUTADA / OVERALL PASS for mechanics**. Dynamic execution was **NO EJECUTADA**. The 41/41 official SmartApplet survey found exactly the two callers described above.

## NEO13 dual-entry refinement

A later NEO13-specific regression, `verify_neo13_dual_entry_updater_transition_a338_2026-09-08.py`, was executed against canonical NEO13 SHA-256:

`32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Result: **EJECUTADA: 20/20 PASS**.

This regression refines the NEO13 transfer topology:

- A338 sets `0x35E2.bit7` and reaches entry `0x00410070`;
- `0x00410070` reloads `A7` with the platform stack top, clears `A6`, and jumps to `0x00417914`;
- a distinct nearby entry `0x00410082` performs the same stack/base reset but jumps instead to `0x00417062`;
- a separately verified response path carrying value `0x52` reaches `0x00410082`;
- the canonical NEO13 image contains exactly 11 direct calls to `0x00410070`;
- exactly two sites set `0x35E2.bit7`, one of them A338.

These are physical/control-flow facts. They do **not** justify naming `0x00410070` and `0x00410082` as `updater`, `normal`, `restart`, or similar without independent semantic evidence. Likewise the meaning of `0x35E2.bit7` remains unresolved.

## Reconciled classification

- A338/index206 as a real A-line ABI object in AS3000, NEO05 and NEO13: **CONFIRMADO / CERRADO A mechanically**.
- Three-generation handler equivalence after relocations: **CONFIRMADO**.
- Effective no-return behavior through the verified trampoline/re-entry mechanism: **CONFIRMADO structurally** across the three-generation closure; NEO13 independently revalidated by the 20/20 dual-entry regression.
- Historical API identity `abort`: **INFERENCIA FUERTE** from BetaWise/neo-re source correlation plus matching abort-like non-returning mechanics; not an original vendor-symbol claim.
- Exact C signature `void(void)`: **NO_RESUELTO / PROTOTIPO_C_BLOQUEADO** because the two official callers supply an ignored zero longword despite the historical declaration.
- Semantic name for the bit set by A338 (`0x35E2.bit7` in NEO13): **NO_RESUELTO / EVIDENCIA_INSUFICIENTE**.
- Semantic names for NEO13 entries `0x00410070` and `0x00410082`, and for their final destinations: **NO_RESUELTO / EVIDENCIA_INSUFICIENTE**.

## Falsification results

Two dominant alternatives have been tested:

1. **A338 is a conventional returning function because an `RTS` follows its internal call.** Refuted. The transfer entry replaces the stack pointer before an unconditional jump, so ordinary return through A338's saved caller frame is not structurally available.
2. **The historical `void abort(void)` declaration is sufficient to publish a zero-argument callable prototype.** Not established. Two official executable callers push an extra zero longword and clean it afterward; because the handler ignores stack data, the semantic status of that slot cannot be resolved from the callee alone.

## Traceability and next evidence targets

The earlier forward-sweep note `docs/sys-a338-blocked.md` remains the canonical record of the three-generation mechanical closure and caller/prototype discrepancy. This note adds the later NEO13 dual-entry topology and reconciles it with that already-established cross-generation evidence.

Further work should target only unresolved semantics rather than redoing the three-generation closure:

1. correlate the 11 NEO13 callers of `0x00410070` with independently named source-visible operations;
2. identify readers and additional writers/consumers of the generation-specific bit-7 state and determine its role;
3. reconstruct and correlate the distinct destinations reached from `0x00410070` and `0x00410082` without assigning names ahead of evidence;
4. search historical source/caller material for an explanation of the ignored zero longword at the two official A338 callsites.

No ROM image, extensive disassembly, or Ghidra database is included in this public note.
