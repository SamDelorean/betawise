# System 3 / OS3K AppletFindByName — A238 SDK consolidation

This note is the developer-facing consolidation record for A238, exposed by BetaWise as:

```c
uint32_t AppletFindByName(const char *name, uint32_t start_index);
```

## 0. Consolidation status

A238 reached this SDK-consolidation pass after the reverse-engineering process had already closed its mechanical contract with confidence **A**. This pass does not repeat ROM/disassembly work. It audits the delivered contract against the current BetaWise declaration, A-line stub and SmartApplet runtime reference, then records the contract application code should rely on.

Current state:

- mechanical contract: **A**, received from the reverse-engineering process;
- public identity: `AppletFindByName`, name continuity **strong**;
- BetaWise declaration/stub: **implemented and audited**;
- prior `uint8_t` return and `uint8_t start_index` declaration: **superseded** by the delivered 32-bit machine contract;
- `name` is read-only and the canonical declaration is `const char *`;
- additional wrapper: **not required**;
- dynamic emulator/hardware regression: **pending**.

If later primary evidence changes the upstream mechanical contract, this SDK contract must be reconciled and revised rather than silently preserving an obsolete conclusion.

## 1. Public contract

```c
uint32_t AppletFindByName(const char *name, uint32_t start_index);
```

`name` must point to a valid NUL-terminated string. It is read-only. A NULL pointer is not a safe input because the System 3 handler computes `strlen(name)` before any useful NULL guard.

`start_index` is a full 32-bit runtime index slot and is **exclusive**. The search starts at `start_index + 1` and proceeds while the candidate runtime index is below 32. Uninstalled/NULL runtime-table entries are skipped.

For each installed candidate, the service compares:

```text
strncmp(candidate_header->name, name, strlen(name))
```

Therefore the operation is a **prefix search**, not exact string equality.

Equivalent developer model for the ordinary runtime-index domain:

```text
needle_len = strlen(name)
for index = start_index + 1; index < 32; ++index:
    header = runtime_table[index]
    if header != NULL and strncmp(header->name, name, needle_len) == 0:
        return index
return 0
```

The return is the full 32-bit runtime index. Valid ordinary matches remain in the System 3 runtime domain `0..31`; no match returns full `0`.

The operation is read-only. It does not install/select an applet, mutate the runtime table, change A5 context, send a message, or change current/selected applet state.

## 2. Prefix semantics and edge cases

The prefix behavior is contractual and must not be normalized into exact-name matching by a wrapper.

Examples:

- searching for `"Alpha"` may match an installed applet named `"AlphaWord Plus"`;
- an empty string has length zero and therefore matches the first installed candidate after `start_index`;
- `start_index == 31` performs no candidate comparison and returns `0`;
- callers that want to continue searching after a previous match should pass that returned runtime index as the next exclusive `start_index`.

A return of `0` is the normal not-found sentinel for ordinary third-party searches. Application code must not reinterpret the return as a SmartApplet ID: the function searches by applet **name** and returns a **runtime table index**.

## 3. BetaWise integration

The current BetaWise branch already matches the reconstructed ABI:

```c
uint32_t AppletFindByName(const char *name, uint32_t start_index);
```

`os3k/syscall.c` maps A-line index 142 directly to `AppletFindByName`, corresponding to trap A238.

No wrapper layer is required. The public SDK symbol is the direct System 3 resident service.

Earlier BetaWise material used the narrower declaration:

```c
uint8_t AppletFindByName(char *name, uint8_t start_index);
```

That form was sufficient for common runtime indices but did not describe the machine contract. The delivered reverse-engineering result establishes that the handler consumes, increments, compares and returns the index as a full longword. The current declaration also makes the input string const-correct because the handler only reads it.

No behavioral code change is required by this consolidation pass because these corrections had already been incorporated into the working branch before consolidation.

## 4. Mechanical evidence received from the reverse-engineering process

The upstream reconstruction compared A238 in:

- AlphaSmart 3000 System 3, November 2005;
- AlphaSmart NEO System 3, November 2005;
- AlphaSmart NEO/System 3.15, July 2013.

The implementations are structurally equivalent apart from relocated runtime-table and library-helper addresses. All use the 32-entry SmartApplet runtime table and the same `strlen`/`strncmp` search logic.

System 3 internal callers observed by the upstream process pass a zero start value to search the ordinary SmartApplet slots beginning at index 1. The detailed low-level evidence remains in [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md); this consolidation note intentionally does not duplicate the reverse-engineering workpapers.

## 5. Relationship to AppletHeader_t and companion services

The runtime table contains pointers to installed `AppletHeader_t` structures. A238 compares the header name field beginning at `AppletHeader_t + 0x18`, which is the SDK's 36-byte `name` field.

Related services use the same runtime-index namespace:

- `AppletFindById` / A23C searches by exact 16-bit `AppletHeader_t.id`;
- `AppletGetName` / A240 copies an installed applet's header name from a runtime index;
- `AppletSendMessage` / A244 dispatches to a runtime index while managing System 3 applet context.

A238 itself performs only discovery and has no dispatch or selection side effects.

## 6. Relationship to standard C / BetaWise runtime

`AppletFindByName` is not an ISO C or hosted-runtime service. It is a System 3 resident SmartApplet discovery function exposed through the BetaWise A-line runtime.

Application model:

```text
SmartApplet C -> BetaWise declaration/stub -> A238 -> System 3 SmartApplet runtime table
```

Although the firmware uses `strlen` and `strncmp` internally, replacing A238 with a host-side string search would not be equivalent: the authoritative installed-app table, runtime-index ordering, skipped empty slots, exclusive-start behavior and not-found convention belong to System 3.

## 7. Safety and preconditions

A238 is a non-destructive query and is suitable for direct regression testing.

Preconditions:

- `name` must be a valid readable NUL-terminated string;
- do not pass NULL;
- callers should use the documented System 3 runtime-index domain for ordinary iteration.

The function has no caller-owned output buffer and does not retain pointers after return.

## 8. Regression specification — not yet executed

A useful non-destructive regression should verify at least:

1. **Known exact name** — search for the complete name of an installed non-System applet from `start_index = 0`; expect its runtime index.
2. **Prefix match** — search for a unique leading prefix of that name; expect the same runtime index.
3. **Exclusive start** — after obtaining one match, call again with that returned index; the previous match must not be returned again.
4. **Not found** — use a string that cannot match any installed applet; expect `0`.
5. **Upper boundary** — call with `start_index = 31`; expect `0` without a candidate comparison.
6. **Empty string** — with a controlled installed-app table, verify that `""` returns the first installed candidate after the supplied start index.

NULL is intentionally excluded from the regression matrix because the delivered mechanical contract says it is not a safe input.

The presence of this specification does **not** mean it has been executed on an emulator or physical AlphaSmart.

## 9. SDK status

Formal state after this consolidation pass:

**RECEIVED FROM REVERSE-ENGINEERING PROCESS — MECHANICAL A -> CONSOLIDATED -> IMPLEMENTED/AUDITED -> PUBLISHED -> PROBE/REGRESSION PENDING**
