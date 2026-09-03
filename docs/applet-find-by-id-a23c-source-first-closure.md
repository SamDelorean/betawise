# A23C / AppletFindById — source-first closure

Status: **CLOSED A / SOURCE-FIRST**

A23C/index143 was re-audited source-first. Historical SDK naming/prototypes, the current BetaWise declaration, earlier ABI notes, A234, A238, and `AppletHeader_t` structure work were treated as correlation sources; the contract below was revalidated directly against the three canonical firmware images.

## ABI

```c
uint32_t AppletFindById(uint16_t id);
```

The argument occupies the normal 32-bit C stack slot, but firmware compares the low 16-bit word. The returned runtime index is produced as a full longword in `D0.L`.

## Confirmed behavior

- Initialize candidate runtime index to 0.
- Scan the generation-specific 32-entry `AppletHeader_t *` runtime table while candidate `< 32`.
- If the current table entry is NULL, terminate immediately and return 0; holes are not skipped.
- Dereference the current header and read the 16-bit word at `AppletHeader_t + 0x14`.
- Compare that word exactly with the input `id`.
- On equality, return the current runtime index in `D0.L`.
- Otherwise increment the candidate and continue.
- Exhaustion returns 0.

There are no helper calls, state writes, bounds checks beyond the fixed candidate `< 32` loop, or pointer validation beyond the NULL table-entry termination.

Index 0 is the System applet and 0 is also the not-found sentinel, so a successful match at runtime slot 0 is indistinguishable from not-found by return value alone. This ambiguity is present in the historical SDK documentation as well.

## Cross-ROM primary validation

A23C entry / boundary:

- AS3000 2005: `0x004E0E5E` -> A240 `0x004E0E96`
- NEO 2005: `0x005E37BA` -> A240 `0x005E37F2`
- NEO 2013: `0x00439BBC` -> A240 `0x00439BF4`

Exact handler length: `0x38` bytes in all three ROMs.

Applet-header pointer-table bases:

- AS3000: `0x0D82`
- NEO 2005: `0x0DDA`
- NEO 2013: `0x0E0A`

Handler SHA-256:

- AS3000: `87271ed453655b359d975ed4cfb2cdb2acd67f4b1ba572d2b478285f035c47c6`
- NEO 2005: `e67a61a104df60ecf877f006412a100f672cbdd69fcbc9d988a53a14ff813d5d`
- NEO 2013: `ecdf11fb4df1d93c83bd51889117faf478340377e4ef227a3d772614bbc9f3c6`

The implementations are instruction-for-instruction equivalent apart from the relocated table base.

## Structural correlation

A234 reads the same `AppletHeader_t + 0x14` field for the currently selected runtime index, establishing an inverse relationship:

- A234: runtime index -> 16-bit applet ID.
- A23C: 16-bit applet ID -> runtime index.

A238 uses the same pointer table but reads the inline name at `+0x18`, independently reinforcing the reconstructed header layout.

## Callers / xrefs

Full-ROM absolute `JSR` search found:

- AS3000 2005: 7 direct callers
- NEO 2005: 7 direct callers
- NEO 2013: 8 direct callers

Native literal call sites include IDs `0xA000`, `0xA001`, `0xA002`, `0xA005`, and `0xA010`; NEO 2013 also contains a dynamic-ID caller. Multiple callers consume the return as a runtime index and pass it into subsequent applet/file operations, corroborating the return contract.

## Refutation

Firmware refutes alternative interpretations as a Boolean presence test, header-pointer getter, 32-bit ID comparator, sparse-table search, or name-derived lookup. The compared field is exactly the 16-bit word at header `+0x14`, and the successful result is the candidate runtime index.

## Regression

Static source-first regression **EXECUTED: 48/48 PASS** (16 checks per ROM): canonical ROM identity, exact 0x38 boundary, saved-register frame, zero initial candidate, correct pointer-table base, index scaling, NULL-slot termination, pointer dereference, `+0x14` word load, low-word argument compare, match return, candidate increment, `<32` loop, zero no-match return, absence of helper JSRs inside the handler, and direct-caller count.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**. Safe future cases: known installed nonzero applet ID; unknown ID; slot-0 ID with explicit awareness of the zero ambiguity; and cross-check `AppletFindById(SYS_A234())` when a nonzero applet is selected.

## Confidence

- **CONFIRMED:** `uint32_t AppletFindById(uint16_t id)`; exact 16-bit `+0x14` comparison; runtime-index return; first-NULL termination; 32-slot limit; no helpers/side effects; handler 0x38 3/3; direct callers 7/7/8.
- **CONFIRMED caveat:** return 0 is ambiguous between not-found and a successful slot-0 match.
