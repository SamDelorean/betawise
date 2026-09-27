# DebugTool-AS2000 architecture contract

This document freezes the conceptual architecture. It is not a feature backlog.

## Purpose

DebugTool-AS2000 is a compact resident MC68HC11 monitor inspired by the
BetaWise DebugTool and specialized for diagnosing the AlphaSmart 2000 and
DynFS.

It must run with the same core behavior in:

1. the instrumented AS2000 MAME diagnostic environment,
2. the current patched AS2000 ROM used during DynFS development,
3. a later relocated/final ROM image.

Only the integration bindings may change between those environments.

## CPU model

The MC68HC11 is treated natively:

- A and B: 8-bit accumulators
- D = A:B: 16-bit accumulator
- X, Y, SP, PC: 16-bit
- normal CPU address: 16-bit
- extended physical/logical memory identity: bank + 16-bit address

The project must not emulate the original BetaWise 32-bit pointer/calling
model.

## Frozen user-visible functions

### MEM

Physical memory viewer.

- displays the current bank and 16-bit address
- displays hexadecimal bytes and a compact ASCII representation
- represents what is physically visible/readable, not what DynFS believes is
  present

### EDIT

Physical byte editor.

- edits the currently selected physical byte
- does not route writes through DynFS
- exists specifically so physical state can be compared against DynFS logical
  state

### GOTO

Memory navigator.

Minimum accepted syntax/behavior:

- 16-bit address in the current bank
- bank + 16-bit address

No general expression parser, decimal parser, symbolic evaluator, scratch
pointer syntax, or BetaWise 32-bit syntax belongs to v0.

### CALL

Native HC11 routine probe.

- target is a 16-bit executable address
- arguments/state are expressed using native HC11 registers, not six generic
  32-bit arguments
- minimum useful pre-call state: A, B/D, X, Y
- SP may be displayed but is not user-editable in v0
- minimum post-call state: A, B/D, X, Y and CCR when practical
- implementation must use the smallest safe HC11-native call mechanism
- there is one CALL engine; ROM routines and DynFS routines use the same engine

The original BetaWise A-line/system-call mechanism is not reproduced.

### INFO

DynFS-oriented diagnostic state.

INFO exists to compare logical filesystem state against MEM/EDIT physical
state. It may expose only already-existing state required to diagnose DynFS,
such as:

- active slot/file
- logical position
- extent/index
- selected bank/window
- relevant legacy compatibility pointers/state
- selected stable DynFS ABI entry or state

INFO must not become a general telemetry framework.

## Diagnostic model

The five functions have fixed roles:

- MEM = physical truth
- EDIT = physical intervention
- GOTO = physical navigation
- INFO = DynFS logical truth
- CALL = functional probe of ROM/DynFS primitives

This separation is intentional. No function may silently duplicate another.

## Address model

Two concepts must remain separate:

1. CPU address: 16-bit address currently visible to the HC11.
2. Banked address: bank identifier + 16-bit CPU address.

The core must not manufacture a 32-bit flat-address abstraction merely to look
like the BetaWise implementation.

## Stable-core / late-binding rule

The debugger core must not depend on final absolute locations until
integration.

Environment-specific values belong in one binding layer, conceptually:

- debugger entry/exit address
- ROM LCD output/cursor/clear services
- ROM keyboard service
- bank select/read/write mechanism
- optional stock hex/format helper addresses
- DynFS ABI entry addresses
- DynFS diagnostic-state addresses
- debugger workspace RAM base and size

The core must consume symbolic names for these dependencies. Moving the tool
to a later ROM should therefore require changing bindings/placement, not
rewriting MEM, EDIT, GOTO, CALL, or INFO.

## RAM contract

Persistent RAM is budgeted independently from ROM.

Target: <= 50 bytes.

Before integration, the project must produce an exact RAM ledger containing:

- each state variable
- size in bytes
- lifetime
- alignment requirement, if any
- whether the byte can be overlaid/reused
- chosen final address
- evidence that the chosen RAM range is actually free in the target ROM

No final RAM address is to be guessed early.

## DynFS coupling contract

DebugTool may read DynFS state and CALL stable DynFS/ROM primitives, but it
must not own filesystem logic.

No DynFS algorithm is to be copied into DebugTool.

If DynFS layout changes, only bindings and the small INFO decoder are allowed
to require adjustment.

## Portability rule

The first-stage implementation and later-ROM implementation must share the
same core assembly source. Environment differences are resolved through
bindings/constants and, only where unavoidable, tiny adapter stubs.

A second independent implementation for the later ROM is explicitly
forbidden.

## Non-goals

The following remain outside v0 regardless of available ROM space:

- breakpoint manager
- single-step engine
- disassembler
- arbitrary expression evaluator
- memory search
- general scripting
- plugin system
- OS3K compatibility
- 68k ABI emulation
- feature additions that are not necessary to make MEM/EDIT/GOTO/CALL/INFO
  usable for AS2000/DynFS diagnosis
