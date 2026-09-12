# SDK audit — canonical navigation gap after A28C

Status: **AUDIT_FINDING / DOCUMENTATION_RECONCILIATION_REQUIRED**

## Finding

`docs/abi-reconstruction-index.md` describes itself as the canonical navigation and traceability index for reconstructed OS3K interfaces, but its current reconstructed-block table stops at `A28C`.

That stopping point is no longer aligned with the actual consolidated ABI state. The later consolidation records the corroborated `A000..A470` range as complete with:

- `CERRADA`: 208
- `CERRADA_CON_LÍMITE_DE_EVIDENCIA`: 56
- `NO_SYSCALL/NO_CALLABLE`: 21
- `PENDIENTE_REAL`: 0
- total: 285

The public umbrella header also already exposes a subset of later NEO13-only contracts:

```c
uint16_t SYS_A3B4(void *record);
uint8_t SYS_A3B8(uint16_t arg1, void *arg2, uint16_t arg3, uint8_t arg4);
uint8_t SYS_A3BC(void);
uint8_t SYS_A3C0(void);
uint8_t SYS_A404(void);
void *SYS_A41C(uint8_t selector);
char *SYS_A438(char *dst);
uint16_t SYS_A470(void);
```

Therefore the canonical index currently gives an incomplete navigation picture even though the underlying ABI inventory itself is not incomplete.

## Scope boundary

This finding does **not** mean that every numeric position after `A28C` should receive a public C declaration. The consolidated status must remain authoritative:

- `CERRADA` entries may be indexed as usable contracts when evidence supports them;
- `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entries must retain their explicit limit and must not be promoted into stronger public prototypes merely for completeness;
- `NO_SYSCALL/NO_CALLABLE` positions should be represented as structural/non-callable where useful for navigation;
- the canonical sequential range remains `A000..A470`; no extrapolation beyond corroborated callable evidence is implied.

## Recommended correction

Extend the canonical navigation index beyond A28C by referencing the already-consolidated later blocks rather than reopening reverse engineering. The index should at minimum make the following facts visible:

1. sequential ABI reconstruction is complete through `A470`;
2. there are zero `PENDIENTE_REAL` entries in that corroborated range;
3. later NEO-only contracts already published in `os3k.h` are discoverable from the index;
4. evidence-limited and non-callable entries remain explicitly distinguished;
5. detailed late-block evidence may remain in focused closure/consolidation documents instead of duplicating all analysis into the index.

## Classification

This is a documentation/navigation reconciliation issue only.

- no ABI status changes;
- no firmware reanalysis required;
- no SDK prototype changes required by this finding alone;
- no build gate is required for an index-only correction.

The existing `docs/os3k-abi-consolidation-2026-09-11.md` remains the authoritative inventory source for the complete `A000..A470` status until the canonical navigation index is reconciled.
