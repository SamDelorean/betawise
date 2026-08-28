# System 3 / OS3K AppletFindById — A23C SDK consolidation

This note is the developer-facing consolidation record for A23C, exposed by BetaWise as:

```c
uint32_t AppletFindById(uint16_t id);
```

## 0. Consolidation status

A23C reached this SDK-consolidation pass after the reverse-engineering process had already closed its mechanical contract with confidence **A**. This pass does not repeat ROM/disassembly work. It audits the delivered contract against the current BetaWise declaration, A-line stub and SmartApplet runtime reference, then records the contract application code should rely on.

Current state:

- mechanical contract: **A**, received from the reverse-engineering process;
- public identity: `AppletFindById`, name continuity **strong**;
- BetaWise declaration/stub: **implemented and audited**;
- prior `uint8_t` return declaration: **superseded** by the delivered 32-bit machine contract;
- additional wrapper: **not required**;
- dynamic emulator/hardware regression: **pending**.

If later primary evidence changes the upstream mechanical contract, this SDK contract must be reconciled and revised rather than silently preserving an obsolete conclusion.

## 1. Public contract

```c
uint32_t AppletFindById(uint16_t id);
```

`id` is the exact 16-bit SmartApplet identifier stored in `AppletHeader_t.id`.

The service scans the System 3 runtime SmartApplet table, whose normal domain is indices `0..31`. For each installed entry it compares the requested 16-bit ID with the ID in the applet header.

On a match it returns the complete runtime index as a 32-bit value. If no match exists it returns `0`.

Equivalent developer model:

```text
for index in 0..31:
    header = runtime_table[index]
    if header != NULL and header->id == id:
        return index
return 0
```

The operation is read-only. It does not install or select an applet, send a message, change A5 context, mutate the runtime table, or change the currently selected/current applet state.

## 2. Important index-0 ambiguity

Runtime index `0` is the System applet. The same numeric value, `0`, is also the not-found sentinel.

Therefore a successful lookup of the System applet's own ID is indistinguishable from not-found by return value alone.

For ordinary third-party applets, which occupy nonzero runtime slots, a positive return value identifies the matching installed runtime entry.

Application code must not reinterpret the result as an applet ID. The argument is an applet ID; the return is a runtime table index.

## 3. BetaWise integration

The current BetaWise branch already matches the reconstructed ABI:

```c
uint32_t AppletFindById(uint16_t id);
```

`os3k/syscall.c` maps A-line index 143 directly to `AppletFindById`, corresponding to trap A23C.

No wrapper layer is required. The public SDK symbol is the direct System 3 resident service.

Earlier BetaWise material used an 8-bit return:

```c
uint8_t AppletFindById(uint16_t id);
```

That declaration was adequate only because valid runtime indices happen to fit in one byte. It did not describe the machine contract: the delivered reverse-engineering result establishes that the handler produces and returns the runtime index as a full longword. The current `uint32_t` declaration is therefore the canonical SDK form.

No behavioral code change is required by this consolidation pass because that width correction had already been incorporated into the working branch before consolidation.

## 4. Mechanical evidence received from the reverse-engineering process

The upstream reconstruction compared A23C in:

- AlphaSmart 3000 System 3, November 2005;
- AlphaSmart NEO System 3, November 2005;
- AlphaSmart NEO/System 3.15, July 2013.

The compared implementations preserve the same public mechanics: exact 16-bit header-ID comparison over the 32-entry runtime table and full runtime-index return.

The detailed low-level evidence remains in [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md). This consolidation note intentionally does not duplicate the underlying disassembly workpapers.

## 5. Relationship to neighboring SmartApplet services

A23C belongs to a small named runtime API family:

```text
A238  AppletFindByName
A23C  AppletFindById
A240  AppletGetName
A244  AppletSendMessage
```

The identities must remain distinct:

- `AppletFindById` accepts a 16-bit header ID and returns a runtime index;
- `AppletFindByName` searches names and also returns a runtime index;
- `AppletGetName` consumes a runtime index and copies the header name;
- `AppletSendMessage` consumes a runtime index and performs dispatch/context switching.

A23C itself performs lookup only.

## 6. Relationship to standard C / BetaWise runtime

`AppletFindById` is not an ISO C or hosted-runtime facility. It queries OS3K's resident SmartApplet registry through the BetaWise A-line runtime.

Application model:

```text
SmartApplet C -> BetaWise declaration/stub -> A23C -> System 3 applet runtime
```

Do not substitute a host process ID, library handle, pointer, or operating-system application identifier for either side of this contract. `id` is the OS3K `AppletHeader_t.id`; the returned value is an OS3K runtime slot.

## 7. Safety and preconditions

This is a non-destructive query and is suitable for direct regression testing.

There are no caller-owned output pointers and no persistent mirror/lifetime effects.

A return value of `0` must be interpreted with the index-0 ambiguity described above. Callers searching ordinary non-System applets can normally treat positive values as found and zero as not found.

## 8. Regression specification — not yet executed

Minimum useful regression cases are:

1. **Known installed non-System applet**
   - obtain a known installed applet ID;
   - call `AppletFindById(id)`;
   - verify the returned runtime index is the slot containing that same header ID.

2. **Unknown ID**
   - use an ID known not to be installed;
   - verify the result is `0`.

3. **Cross-check with name lookup/header data**
   - locate an installed applet by an independent path;
   - read/know its 16-bit `AppletHeader_t.id`;
   - verify A23C returns the same runtime index.

4. **No mutation**
   - capture relevant selected/current applet state before the call;
   - invoke A23C;
   - verify lookup alone does not alter that state.

5. **System-slot ambiguity**
   - when a safe test fixture exposes the System applet ID, document that a valid index-0 match is numerically identical to the not-found sentinel rather than attempting to infer a distinction the ABI does not provide.

A probe specification is not evidence that these cases have been executed. Dynamic validation remains pending.

## 9. Canonical SDK conclusion

The supported developer contract is:

```c
uint32_t AppletFindById(uint16_t id);
```

A23C performs an exact, read-only lookup of a 16-bit SmartApplet header ID and returns the matching OS3K runtime index as a 32-bit value, or `0` when no match exists. Index 0 is also the System applet, so that one case is intrinsically ambiguous. The old 8-bit return declaration is superseded.
