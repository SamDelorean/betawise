# SYS_A2C4 closure

Status: **MECÁNICA_CERRADA A**. Dynamic regression is specified but not yet executed.

## Neutral contract

```c
int32_t SYS_A2C4(void *state);
```

A2C4 consumes one physical 32-bit stack slot, a mandatory mutable state pointer. No direct ROM caller was recovered; occurrences in inspected official applets are entries in their contiguous A-line stub tables rather than semantic callsites.

## Cross-generation handler

| ROM | Runtime | File offset | Size |
| --- | ---: | ---: | ---: |
| AS3000 2005 | `0x004D49C0` | `0x0149C0` | `0xAC` |
| NEO 2005 | `0x005D6C8A` | `0x016C8A` | `0xAC` |
| NEO 2013 | `0x00428366` | `0x018366` | `0xC6` |

AS3000 and NEO 2005 are instruction-equivalent after relocation normalization. NEO 2013 adds a preparatory sequence and temporary handling of `state+0x0A`, without changing the external ABI.

## Correlation with A2BC

On the accepted path A2C4 constructs and calls A2BC with values taken from `state`:

```c
SYS_A2BC(state,
         zero_extend_u16(*(uint16_t *)(state + 0x00)),
         0,
         *(uint32_t *)(state + 0x02),
         zero_extend_u16(*(uint16_t *)(state + 0x0E)),
         zero_extend_u16(*(uint16_t *)(state + 0x10)),
         zero_extend_u16(*(uint16_t *)(state + 0x12)),
         zero_extend_u16(*(uint16_t *)(state + 0x14)));
```

The complete A2BC `D0.L` result is propagated. This linkage is present in all three canonical ROM generations.

## Return behavior

The return is a full signed 32-bit status, not a Boolean. Mechanically demonstrated routes include:

- a negative full-long helper result, propagated after cleanup;
- `0` for ordinary gating failures;
- explicit `0x01000002` when the word at `state+0x4A` is nonzero;
- the complete A2BC result on the main path.

No vendor meanings are assigned to these status values.

## Adversarial exclusions

A2C4 is not the public three-argument `ProcessMessage` interface; it does not take eight external arguments (those slots belong to the internal A2BC call); and it is not a `void` wrapper. No independent vendor symbol was recovered, therefore `SYS_A2C4` remains the neutral name.

## Regression status

**Specified / not executed.** Tests should cover exact A2BC field forwarding and full-long return propagation, `state+0x4A != 0`, negative preliminary status propagation, zero gating paths, and NEO 2013 preservation of `state+0x0A` on the path reaching A2BC.

## 2026-09-04 source-first re-audit

The closure was revalidated directly against all three canonical ROM images before advancing the sequential frontier. The ROM hashes matched the canonical manifest. Exact handler fingerprints also reproduced: AS3000 `70a7c3539a10e12aba2c2b1408440781b973af08b6cb7ea99041a322a22e7332`, NEO 2005 `546c0239baeb92fba1d68e08eb877fe55bbd548046340c080031e3f310896914`, and NEO 2013 `8a81cffe764df0498148369f52753af19dd08b8613dc55b92f00c6a282b88bca`.

A fresh whole-ROM xref scan found zero absolute `JSR`/`JMP` references to A2C4 in each generation, reproducing the prior negative caller search. Independently, the internal word-displacement `BSR` on the accepted path resolves exactly to the corresponding A2BC entry in AS3000, NEO 2005, and NEO 2013. Static structural regression: **12/12 PASS** (3 canonical ROM identities, 3 handler fingerprints, 3 negative absolute-xref checks, 3 exact A2BC branch-target checks).

No new source or manual evidence contradicted the neutral contract or justified a vendor name. Dynamic/emulator regression remains **specified / not executed**.

Full ROM bytes and detailed disassembly remain private workpapers.
