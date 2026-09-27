# DebugTool-AS2000

Compact AlphaSmart 2000 memory/debug monitor derived conceptually from the
BetaWise `applets/DebugTool` utility.

## Target

- Machine: AlphaSmart 2000
- CPU: Motorola MC68HC11
- Implementation language: 68HC11 assembly
- Integration target: patched AS2000 ROM / DynFS-capable ROM
- Reference implementation: `../DebugTool/DebugTool.c`
- Branch: `as2000-debugtool`

This is **not** a source-level port of the OS3K/68k applet. The BetaWise
DebugTool is the behavioral reference only. The AS2000 implementation must
reuse existing ROM services whenever that produces a smaller result.

## Frozen v0 scope

Only these functions belong to the first implementation:

1. **MEM** — display memory in hexadecimal and ASCII.
2. **EDIT** — edit bytes in hexadecimal or ASCII.
3. **GOTO** — select a bank/address and jump the viewer there.
4. **CALL** — invoke a ROM/debug target with the minimum useful HC11 calling
   interface.
5. **INFO** — display the minimum state needed for diagnostics.

The following are deliberately out of scope for v0:

- disassembler
- breakpoint manager
- single-step execution
- search engine
- elaborate help system
- plugin/app framework
- OS3K compatibility layer
- 68k bus-error emulation
- feature growth not required by the five functions above

## ROM/RAM budget

- Preferred ROM target: **<= 1536 bytes**
- Aggressive ROM target: **<= 1024 bytes**
- Re-evaluation threshold: **> 1792 bytes**
- Preferred additional RAM: **<= 50 bytes**

The build must report the exact code/data size after every meaningful
increment.

## Design rules

1. Reuse proven AS2000 ROM routines before writing local equivalents.
2. Keep the UI native to the AS2000 40x4 display and keyboard.
3. Use bank + 16-bit address rather than the original 32-bit address model.
4. Do not carry the original 256-byte scratch buffer unless a concrete use
   justifies it.
5. Do not reproduce the OS3K message/app lifecycle.
6. Do not duplicate DynFS functionality.
7. Keep all AS2000-specific work isolated in this directory until integration
   is explicitly performed.
8. Every retained feature must have a byte-cost justification.

## Initial work sequence

1. Inventory the original DebugTool functions.
2. Classify each as RETAIN / REWRITE / ROM-REUSE / DROP.
3. Identify the exact stock-ROM entry points needed for LCD, keyboard and
   memory/bank services.
4. Freeze the HC11 calling convention for `CALL`.
5. Produce a byte-level static estimate.
6. Only then create the assembly implementation.
7. Assemble and exercise it in the instrumented AS2000 MAME diagnostic build.

See `PORTING_MAP.md` for the first-pass mapping.

## Controlling documents

The project is controlled by these documents:

- `ARCHITECTURE.md` — frozen CPU/address/calling model and DynFS diagnostic
  role.
- `WORKPLAN.md` — rigid increment-by-increment execution order and stop
  conditions.
- `PORTING_MAP.md` — mapping from the original BetaWise DebugTool behavior.
- `SIZE_BUDGET.md` — ROM/RAM ceilings and measurement rules.

If an implementation idea conflicts with these documents, the frozen v0 scope
and `WORKPLAN.md` take precedence until deliberately revised by the user.

## Integration strategy

The DebugTool core is developed independently of final placement. Absolute ROM
locations, free-RAM workspace, stock-ROM service entry points and DynFS state
addresses are supplied through a late binding layer.

This permits the same core to be used first in the diagnostic/current ROM and
later in the relocated/final ROM without creating a second implementation.
