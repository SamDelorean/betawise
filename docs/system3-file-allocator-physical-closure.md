# System 3 / OS3K file allocator — three-generation physical closure

Status: **mechanics closed A / source-first / three-generation correlation**.

This note closes the physical allocator model behind the reconstructed System 3 File API. It is reconciled from the master evidence corpus into `sdk/abi-automation` without changing any public ABI classification.

## Audit-relevant conclusions

The three canonical generations (AlphaSmart 3000 System 3 Nov 2005, NEO System 3 Nov 2005, and NEO/System 3 Jul 2013) share the same allocator architecture: ordered `0x48` descriptor records, direct SRAM storage pointers, adjacent capacity transfer, `0x200` accounting/alignment, active-descriptor elasticity, and persistent live mirrors.

Generation-specific file arenas are:

| generation | arena base | arena end, exclusive |
| --- | ---: | ---: |
| AS3000 2005 | `0x4558` | `0x3D800` |
| NEO 2005 | `0x69C0` | `0x7D800` |
| NEO 2013 | `0x1509C` | `0x7D800` |

The descriptor invariant relevant to SDK/emulator work is:

```text
next.storage == current.storage + current.max_size
```

The allocator uses:

```text
watermark = max(recoverable_size, min_size)
normal_floor = max(round_up(watermark, 0x200), 0x200)
```

This independently supports the already-closed `descriptor +0x10 = min_size` interpretation and the A1E8 creation floor. It does not recover vendor public names and therefore does not authorize renaming raw `SYS_Axxx` declarations.

A1E8 creation physically inserts a descriptor, repairs group pointers/tokens, seeds its storage pointer, initializes state, obtains capacity through redistribution, and on insufficient space performs a global reclaim retry before returning raw `-12`.

A1EC deletion transfers the deleted allocation to an adjacent descriptor, compacts the descriptor table, repairs group pointers/tokens, and redistributes newly available capacity. This strengthens internal mechanics but does not change the already-established public ABI.

The internal file-system validator is distinct from the allocator: it checks pointer/size/minimum/cursor invariants and normalizes the recoverable/high-water field; it does not perform payload redistribution.

The persistent storage model is SRAM-resident. All three generations use RAM-valid signature value `0x0D89` at generation-specific addresses. FileClose is therefore not a serialization boundary.

## Audit consequence

This imported evidence is **supporting physical/mechanical evidence only**. It must not be used to:

- promote `CERRADA_CON_LÍMITE_DE_EVIDENCIA`;
- invent or promote public vendor names;
- reopen an already closed syscall absent a concrete ABI/header dependency;
- claim a m68k SDK build.

Static regression associated with the source evidence was 86/86 PASS across the three canonical images. Dynamic emulator regression remains pending. No `BUILD_VALIDATED` claim is made by this reconciliation.

Source provenance: reconciled from master commit `ad557fcf4efbf07fcf0ff3f398a71a7987b7a41c` (`docs: close three-generation System 3 file allocator mechanics`). The master source note contains the full physical derivation; this branch copy intentionally retains only the audit-relevant conclusions so the ABI automation corpus remains self-contained without merging unrelated branch history.
