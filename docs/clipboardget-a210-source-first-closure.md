# A210 / index132 — ClipboardGet — source-first closure

Status: **MECHANICAL A / SOURCE-FIRST / CLOSED**.

This note is the source-first re-audit of trap A210 after the sequential restart at A000. It supersedes any earlier shorthand that treated `ClipboardGet` as a generation-invariant copy primitive. The inherited BetaWise name remains useful, but the ABI and behavior below are justified by direct firmware mechanics and caller correlation rather than by the name alone.

## Source-first correlation

Before relying on the binary contract, the re-audit correlated the already-known layers:

- BetaWise retains index132 as `ClipboardGet`, adjacent to `ClipboardSet` A20C and `ClipboardClear` A214.
- The historical `os3k.h` material available in the repository does not independently recover a stronger vendor prototype/name for this routine.
- The reconstructed A200/A204/A208 clipboard-edit family independently anchors the dedicated clipboard descriptor and the special filtered/control-code-aware read path used when the selector is exactly `3`.
- A20C/A214/A218 independently anchor the shared clipboard state and descriptor identity.
- Official AlphaWord Plus callers use selector `3`; an internal System 3.15 caller uses a non-3 selector (`1`), exercising the raw path.

Therefore `ClipboardGet` is retained as an inherited BetaWise name whose functional identity is confirmed by firmware. Exact original symbolic names for the third argument remain unrecovered; documentation uses neutral `read_variant`.

## Confirmed ABI

```c
int32_t ClipboardGet(void *dst, uint32_t count, uint8_t read_variant);
```

The ABI uses three 32-bit argument slots. The third slot contributes its low byte.

Common validation is confirmed:

- `dst == NULL` -> raw `-6`;
- `count > 0x7fffffff` -> raw `-128`.

No validity range is imposed on `read_variant`; the branch is an exact equality test against `3`.

## Canonical firmware map

The public A210 handlers are:

| ROM | entry | handler length |
| --- | ---: | ---: |
| AS3000 2005 | `0x004E0C38` | `0x7E` |
| NEO 2005 | `0x005E3594` | `0x7E` |
| NEO 2013 | `0x00439992` | `0x82` |

The two 2005 handlers are logically equivalent. NEO 2013 retains the same public selector split but deliberately changes the filtered-path setup.

## Raw path — CONFIRMED portable behavior

For every `read_variant != 3`:

```text
actual = min(count, clipboard.current_size)
memmove(dst, clipboard.storage, actual)
return actual
```

The raw path is deterministic across all three canonical ROMs. It clamps by current clipboard logical size, copies that many bytes, and returns the copied count.

## Filtered path — CONFIRMED generation difference

`read_variant == 3` does not call a plain copy. It routes clipboard bytes through the same control-code-aware/filtered helper family correlated with A208. The helper scans source bytes, recognizes control sequences, may suppress or translate them, and returns the emitted output count through an in/out length pointer.

### AS3000 2005 / NEO 2005

The shipped 2005 A210 handlers allocate a four-byte local and pass its address to the filter helper **without initializing that local first**. They also pass caller `count` as the helper source length.

Mechanically:

```text
uint32_t output_limit;       /* uninitialized firmware local */
filter(clipboard.storage,
       dst,
       count,
       &output_limit);
return output_limit;
```

This is a real firmware quirk and must not be normalized away when documenting or emulating the 2005 systems.

### NEO 2013

The 2013 handler grows by four bytes and explicitly initializes the local with caller `count`. It also uses `clipboard.current_size` as the filter source length:

```text
uint32_t output_limit = count;
filter(clipboard.storage,
       dst,
       clipboard.current_size,
       &output_limit);
return output_limit;
```

This is a material implementation correction relative to both 2005 ROMs.

The filtered helper also contains generation-dependent control-code values, so byte-for-byte formatted output must not be promised across generations without dynamic regression.

## Caller/xref evidence

Caller correlation supports both selector branches:

- AlphaWord Plus uses literal selector `3` for the filtered path in both early and later images.
- System 3.15 contains an internal caller using literal `1`, proving a real non-3/raw use.
- Call shapes clean three 32-bit slots and agree with `(dst, count, read_variant)`.

No evidence supports collapsing the third parameter to a Boolean.

## Refutation

The re-audit explicitly rejects the following weaker interpretations:

1. **“ClipboardGet is just memcpy.”** False: selector `3` routes through a control-code-aware helper.
2. **“The filtered path is equivalent in all generations.”** False: the 2005 handlers pass an uninitialized output-limit local and caller `count` as source length; 2013 initializes the output limit and uses clipboard logical size as source length.
3. **“Selector 3 is a guessed enum value.”** False as mechanics: exact comparison with literal `3` is in firmware and official callers exercise it. The *symbolic enum name* remains unknown.
4. **“Any nonzero selector means filtered.”** False: only exact value `3` selects that path.

## Classification

**CONFIRMED**

- three-slot ABI and parameter order;
- NULL `dst` -> `-6`;
- oversized count -> `-128`;
- exact `read_variant == 3` branch;
- raw-path clamping/copy/returned count;
- 2005 filtered-path uninitialized-local quirk;
- NEO 2013 filtered-path correction;
- generation-sensitive helper behavior;
- observed caller use of selector `3` and a non-3 raw selector.

**INFERENCE STRONG**

- descriptive phrase “control-code-aware/filtered” for the private helper family, based on its byte-table/switch behavior and correlated A208 use.

**INHERITED NAME**

- `ClipboardGet` from BetaWise; function role is confirmed, exact vendor provenance of the modern symbol is not independently recovered.

**UNKNOWN**

- original public enum/type name for `read_variant`;
- complete semantic names of every control code handled by the private filter helper.

## Regression status

Static/source-first checks are specified to cover:

- canonical entry points and 0x7E/0x7E/0x82 boundaries;
- validation immediates `-6` and `-128`;
- exact selector comparison with `3`;
- raw clamp against clipboard current size;
- raw copy and returned `actual`;
- 2005 absence of local initialization before filtered helper call;
- 2013 presence of local initialization and clipboard-size source length;
- caller slot count and literal selectors.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**. It must be generation-separated, especially for selector `3`.

## SDK consequence

The portable contract is strongest for the raw path (`read_variant != 3`). Selector `3` is a real filtered mode, but applications and emulators must preserve the 2005-vs-2013 distinction rather than silently substituting the corrected 2013 semantics into older firmware behavior.

Next sequential block after administrative closure: **A214/index133 ClipboardClear**. A500+ remains preserved and paused.
