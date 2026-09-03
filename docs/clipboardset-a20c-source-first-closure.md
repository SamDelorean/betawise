# ClipboardSet / A20C — source-first closure

Status: mechanically and semantically closed across the three canonical System 3 generations. `ClipboardSet` is the inherited BetaWise name for index 131 / trap A20C; no independent modern vendor symbol has yet been recovered, so the naming provenance remains explicitly bounded.

## Contract

```c
int32_t ClipboardSet(const void *src, uint32_t count, uint8_t clipboard_tag);
```

The ABI occupies three 32-bit stack slots: source pointer, 32-bit count, and a third slot whose low byte is the non-zero clipboard tag.

A20C copies from caller-supplied memory into the dedicated System 3 clipboard. It does not require an active file descriptor.

## Validation and return value

Primary firmware revalidation corrects two earlier assumptions:

- `src == NULL` returns raw **`-6`**, not `-128`;
- `count > 0x7fffffff` returns **`-128`**;
- `clipboard_tag == 0` returns **`-129`**;
- success returns the **actual number of bytes copied after capacity clipping**, not zero.

## Core semantics

For a valid request, A20C:

1. clears clipboard descriptor state bit 2;
2. invokes the private clipboard allocation/reclamation path;
3. clips the transfer to the resulting clipboard capacity;
4. stores the non-zero tag in the shared clipboard-tag global;
5. calls A364 / `memmove` as `memmove(clipboard.storage, src, actual)`;
6. writes `actual` into the clipboard current/recoverable size fields at descriptor offsets `+0x08` and `+0x04`;
7. returns `actual` in `D0.L`.

The source buffer is not modified.

## Firmware evidence

Canonical public handler entries:

| ROM | A20C entry | handler length |
| --- | ---: | ---: |
| AlphaSmart 3000 System 3 (2005) | `0x004E0B8A` | `0xAE` |
| NEO System 3 (2005) | `0x005E34E6` | `0xAE` |
| NEO System 3 (2013) | `0x004398E4` | `0xAE` |

Handler SHA-256 values from the canonical images:

- AS3000 2005: `c85d4fe9f5296f70a57db4eebcdd14347a7335101b638a652867757409235c8e`
- NEO 2005: `89e00ae3f8060353075b0e8f6fe635776100a982893317897733176534cfa2e7`
- NEO 2013: `7f556ddb870dab7a3e87addc785a613f8b46b8f455d989aaa564751fb6254f96`

The public A364/memmove target and the private allocator/reclamation calls were revalidated separately in each generation. Control flow, validation, clipping, descriptor writes and return convention are equivalent across all three.

## Callers / xrefs

Direct absolute JSR callers found in the complete canonical firmware images: **2 / 2 / 2**.

- AS3000: `0x004DE2E6`, `0x004DE596`
- NEO 2005: `0x005E0A30`, `0x005E0CE0`
- NEO 2013: `0x00439B6C`, `0x00439E20`

The recovered internal caller setups pass three arguments and use literal clipboard tag `1`, independently supporting argument order and the non-zero-tag requirement.

## Correlation and naming boundary

A200/A204 use the same dedicated clipboard descriptor and shared tag state. A214 clears that state. A364 is independently identified as `memmove` in the BetaWise syscall table and is the direct byte mover used here.

`ClipboardSet` is therefore retained as the BetaWise name because it is consistent with the mechanically recovered behavior. It is **not** promoted to a vendor-confirmed modern symbol: no independent primary symbol table/header has been recovered that proves that exact name for System 3.

## Refutation record

- Refuted: `src == NULL` returns `-128`. Firmware emits raw `-6`.
- Refuted: success returns `0`. The epilogue returns the clipped `actual` count.
- Refuted: A20C depends on the active file. Its source is the explicit caller pointer and its destination is the dedicated clipboard descriptor.
- Not proven: exact modern vendor symbol and original public enum/type name for `clipboard_tag`.

## Confidence

- **CONFIRMED:** three-slot ABI, pointer/count/tag interpretation, raw errors `-6/-128/-129`, capacity clipping, A364/memmove use, descriptor size updates, success return value, 2/2/2 callers and cross-generation equivalence.
- **INFERENCE FUERTE:** descriptive roles of the private allocation/reclamation helpers.
- **NOMBRE HEREDADO:** `ClipboardSet` from BetaWise.
- **DESCONOCIDO:** exact modern vendor symbol and original symbolic label for the tag byte.

Static source-first regression: **51/51 PASS**. Dynamic/emulator regression remains **SPECIFIED / NOT EXECUTED**, covering null source, high-bit count, zero tag, unclipped copy, capacity-clipped copy, tag persistence and A214 clear-state interaction.
