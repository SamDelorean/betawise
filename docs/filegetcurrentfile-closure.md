# System 3 / OS3K FileGetCurrentFile — A1A8 SDK consolidation

This note is the developer-facing consolidation record for A1A8, exposed by BetaWise as:

```c
uint16_t FileGetCurrentFile(void);
```

## 0. Consolidation status

A1A8 reached this SDK-consolidation pass after the reverse-engineering process had already closed its mechanical contract with confidence **A**. This pass does not repeat ROM/disassembly work. It audits the delivered contract against the current BetaWise declaration, A-line stub and File API reference, then records the contract that application code should rely on.

Current state:

- mechanical contract: **A**, received from the reverse-engineering process;
- public identity: `FileGetCurrentFile`, historical/name continuity **B (strong)**;
- BetaWise declaration/stub: **implemented and audited**;
- additional wrapper: **not required**;
- dynamic emulator/hardware regression: **pending**.

If later primary evidence changes the upstream mechanical contract, this SDK contract must be reconciled and revised rather than silently preserving an obsolete conclusion.

## 1. Public contract

```c
uint16_t FileGetCurrentFile(void);
```

The function takes no arguments.

It resolves the current active System 3 file descriptor. If one is active, the function returns that descriptor's canonical 16-bit file token. If no active descriptor resolves, it returns `0`.

The operation is read-only: it does not modify file contents, cursor position, sizes, descriptor metadata, namespace selection, live mirrors or the active-descriptor state.

Equivalent developer model:

```text
descriptor = resolve_current_active_descriptor()
if descriptor == NULL:
    return 0
return descriptor->canonical_file_token
```

The canonical token is the 16-bit value reconstructed at descriptor offset `+0x44`.

## 2. BetaWise integration

BetaWise already matches the reconstructed ABI:

```c
uint16_t FileGetCurrentFile(void);
```

`os3k/syscall.c` maps A-line index 106 directly to `FileGetCurrentFile`, corresponding to trap A1A8. Therefore the public SDK symbol is the direct System 3 service; there is no BetaWise wrapper layer to reconcile for this function.

No behavioral code change is required by this consolidation pass.

## 3. Relationship to the active-file model

`FileGetCurrentFile` queries the global active descriptor used by System 3 operations that do not take an explicit file token.

Typical lifecycle:

```text
FileSetFolder(...)   select namespace
FileOpen(...)        install active descriptor
FileGetCurrentFile() obtain canonical active token
FileClose()          detach active descriptor
```

After a successful `FileOpen`, `FileGetCurrentFile()` should report the canonical token of the installed descriptor. After `FileClose`, it should return `0` because the active descriptor has been detached.

The function itself does not open, close, rewind, flush, commit or otherwise mutate a file.

## 4. Mechanical evidence received from the reverse-engineering process

The upstream reconstruction compared A1A8 directly in:

- AlphaSmart 3000 System 3, November 2005;
- AlphaSmart NEO System 3, November 2005;
- AlphaSmart NEO System 3, July 2013.

The three implementations are logically equivalent apart from relocation/debug differences.

The reverse-engineering process also identified ten official 2012 SmartApplet callers: seven in AlphaWord Plus, one in AlphaQuiz and two in KeyWords. Several callers zero-extend the returned word and immediately pass it as a file token to later file services. This independently supports the 16-bit token interpretation of the return value.

The detailed research evidence remains in [`file-core-operations.md`](file-core-operations.md). This consolidation note intentionally does not reproduce the private low-level workpapers.

## 5. Historical genealogy and ABI evolution

The original AS3000 source contains a historical `FileGetCurrentFile(void)` that returned the active 1-based `UInt8` file number.

Modern System 3 uses a broader 16-bit token namespace. The historical function therefore provides strong conceptual/name continuity, but its return width and token model are not ABI-identical to the later System 3 service.

The SDK keeps the public name `FileGetCurrentFile` while documenting this evolution explicitly.

## 6. Relationship to standard C / BetaWise runtime

`FileGetCurrentFile` is not an ISO C or hosted-stdio function. It is a System 3 resident service exposed through the BetaWise A-line runtime.

Application model:

```text
SmartApplet C -> BetaWise declaration/stub -> A1A8 -> System 3 file runtime
```

Do not substitute a host `FILE *`, file descriptor, `fileno()`, or other libc/OS file-handle concept for the returned token. The value belongs to the OS3K file-token namespace.

## 7. Safety and preconditions

This is a non-destructive query and is suitable for direct regression testing.

There is no buffer argument and no caller-owned pointer lifetime to manage. The only state precondition relevant to the result is whether an active descriptor currently exists.

A return of `0` means that the current active descriptor did not resolve; callers that require an active file should treat that result as absence/failure rather than inventing a file identity.

## 8. Regression specification — not yet executed

Minimum useful regression cases are:

1. with no active file, `FileGetCurrentFile()` returns `0`;
2. after opening a disposable known file, the return equals the canonical token returned/installed by `FileOpen`;
3. repeated calls return the same token while the active descriptor is unchanged;
4. calling `FileGetCurrentFile()` does not change cursor, sizes, storage or registered mirrors;
5. after `FileClose()`, the result returns to `0`;
6. repeat the observable contract on AS3000 and NEO targets/emulators where available.

These cases are an executable/observable specification only. This document does **not** claim that emulator or hardware regression has already been run.

## 9. Consolidated SDK status

A1A8 is approved for SDK use as:

```c
uint16_t FileGetCurrentFile(void);
```

Formal state:

```text
RECEIVED FROM DEEP REVERSE ENGINEERING — MECHANICAL A
-> CONSOLIDATED
-> IMPLEMENTED/AUDITED
-> PUBLISHED
-> PROBE/REGRESSION PENDING
```
