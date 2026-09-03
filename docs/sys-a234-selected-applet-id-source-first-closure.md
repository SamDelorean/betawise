# SYS_A234 — selected SmartApplet ID getter — source-first closure

Status: **CLOSED A / SOURCE-FIRST**

This note records the restarted source-first audit of A234/index141. Earlier applet-selection documentation was used only as a correlation source; the contract below was revalidated directly against the three canonical firmware images.

## Reconstructed ABI

```c
uint16_t SYS_A234(void);
```

No reliable original vendor symbol has been recovered, so the neutral `SYS_A234` name is retained.

## Confirmed behavior

A234 reads the global selected-SmartApplet runtime index shared with A22C/A230.

- If the selected index is `0xFFFFFFFF` (`-1`), A234 clears only `D0.W` and returns a contractual value of `0`.
- Otherwise it multiplies the selected runtime index by four, indexes the generation-specific table of `AppletHeader_t *`, dereferences that entry, and loads the 16-bit word at `header + 0x14` into `D0.W`.
- It takes no arguments.
- It performs no selection-block-mask revalidation, bounds check, or NULL-pointer check before dereferencing a non-sentinel selection.
- It has no state-changing side effects.
- Only the low 16 bits of D0 are contractual: neither the successful `MOVE.W` nor the sentinel `CLR.W` normalizes the upper word.

The portable reconstructed contract is therefore a selected runtime-index to public SmartApplet-ID bridge, not a Boolean query and not a pointer getter.

## Cross-ROM primary validation

Canonical images revalidated:

- AlphaSmart 3000, November 2005
- NEO, November 2005
- NEO/System 3.15, July 2013

A234 begins at `0x004E0DD2`, `0x005E372E`, and `0x00439B30`, respectively. In all three images A238 begins exactly `0x28` bytes later, giving an exact 40-byte A234 handler.

The implementations are instruction-for-instruction equivalent apart from relocated globals/table bases. The selected-index globals are `0x3560`, `0x35B8`, and `0x35F0`; the corresponding applet-header pointer-table bases are `0x0D82`, `0x0DDA`, and `0x0E0A`.

## Independent structural correlation

The immediately adjacent named `AppletFindById` implementation independently walks the same generation-specific applet-header pointer table and reads the same word at `AppletHeader_t + 0x14` when comparing an input applet ID. This provides a firmware-level inverse-operation check:

- A234: selected runtime index -> header -> 16-bit ID at `+0x14`.
- AppletFindById: 16-bit ID -> scan headers at `+0x14` -> runtime index.

This refutes interpretations of A234 as a table-index getter, pointer getter, clipboard operation, or generic selection-presence Boolean.

## Callers / xrefs

A renewed full-ROM search for direct absolute `JSR` calls to the A234 handler found:

- AS3000 2005: `0`
- NEO 2005: `0`
- NEO 2013: `0`

These are retained as explicit negative search results; no inferred callers are substituted for them.

## Regression

Static source-first regression was **EXECUTED: 42/42 PASS** (14 checks per ROM). It covered canonical ROM identity, exact A234 boundary, sentinel comparison, shared selected-index global, sentinel branch, index scaling, applet-header table base, pointer indirection, `+0x14` field read, low-word zero return, terminal RTS, direct-JSR negative search, and two independent `AppletFindById` structural checks.

Dynamic emulator/hardware regression remains **SPECIFIED / NOT EXECUTED**. Minimum future dynamic cases are: reset selection with A230 and verify A234 returns zero; select a valid applet with A22C and verify A234 equals that header's `+0x14` word; and verify A234 itself does not alter selection state. Invalid non-sentinel indices should not be tested on irreplaceable hardware because A234 performs no bounds or pointer validation.

## Confidence

- **CONFIRMED:** `uint16_t SYS_A234(void)`; sentinel `-1 -> 0`; selected-index table lookup; `AppletHeader_t + 0x14` 16-bit return; no arguments; no helpers/calls; no bounds/NULL/mask validation; only `D0.W` contractual; no direct absolute JSR callers in the three canonical ROMs; 40-byte handler across all three generations.
- **STRONG INFERENCE:** neutral semantic description “selected SmartApplet ID getter,” supported by A22C/A230 state sharing and the inverse `AppletFindById` firmware path.
- **UNKNOWN:** original vendor symbol and whether the original public API documented any stronger preconditions for the selected-index state.
