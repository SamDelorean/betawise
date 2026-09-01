# A028 / `SYS_A028` — internal screen snapshot

Status: **mechanically closed (A confidence)** for the November 2005 AlphaSmart 3000 and NEO System 3 images and the July 2013 NEO System 3 image.

The original vendor symbol has not been recovered. The neutral name `SYS_A028` is therefore retained. The existing historical/BetaWise comment that this syscall saves the screen is now supported directly by firmware behavior rather than by naming alone.

## ABI contract

```c
void SYS_A028(void);
```

A028 consumes no caller arguments. It saves the current display state into a firmware-owned internal snapshot object. No caller-provided destination buffer or snapshot handle is used, and no return value is part of the demonstrated contract.

## Confirmed behavior

The implementation differs by display generation while preserving the same logical operation.

### AlphaSmart 3000

The save path captures the full 4 x 40 character display and the associated display/cursor state needed by the matching restore path.

### NEO

The save path is more extensive. It captures all of the following into the internal snapshot object:

- display RAM read directly from both LCD controllers;
- the System 3 software framebuffer/shadow representation;
- display and cursor-related state used by the restore path.

The 2005 and 2013 NEO implementations have the same core structure with relocated firmware state.

This directly rules out the narrower interpretation that A028 merely preserves cursor coordinates or a small logical state record.

## Relationship with A02C

A02C uses the same firmware-owned snapshot object in every analyzed generation and dispatches to the corresponding restore implementation. A028/A02C are therefore a paired internal save/restore facility.

Because A028 always targets one fixed internal object, the demonstrated model is a **single snapshot**, not a stack of independently nestable snapshots. Calling A028 again replaces the state that a subsequent A02C will restore.

## Caller evidence

In the inspected AlphaWord Plus 2005 and 2012 binaries, A028 appears only in the imported System 3 syscall table; no additional executable A028 caller was identified there. This is a scoped negative result and is not claimed as proof that no official SmartApplet ever calls A028.

## Refutation checks

The closure explicitly tested competing explanations:

- **cursor-state-only save:** rejected by full display-content capture;
- **caller-supplied snapshot buffer:** rejected by the fixed firmware-owned object;
- **returned snapshot handle:** unsupported and inconsistent with A02C reusing the same internal object;
- **recovered vendor name `SaveScreen`:** not established. Only the functional save-screen interpretation is confirmed.

## Validation

A deterministic static regression was executed against all three canonical firmware generations and the two inspected AlphaWord Plus binaries: **76/76 checks passed**. The regression covers canonical image identity, A028 wrapper shape, fixed snapshot storage, save/restore pairing, AS3000 4 x 40 capture mechanics, NEO LCD/shadow/state capture dimensions, cross-generation NEO structure, and the scoped caller search.

Dynamic emulator probing remains useful as a later behavioral regression, but it is not required to establish this static ABI contract.
