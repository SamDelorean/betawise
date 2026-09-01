# A02C / `SYS_A02C` — restore internal screen snapshot

Status: **mechanically closed (A confidence)** for the November 2005 AlphaSmart 3000 and NEO System 3 images and the July 2013 NEO System 3 image.

The original vendor symbol has not been recovered. The neutral name `SYS_A02C` is retained. The long-standing functional comment that this syscall restores the screen is now supported directly by firmware behavior.

## ABI contract

```c
void SYS_A02C(void);
```

A02C consumes no caller arguments and uses the same firmware-owned internal snapshot object populated by A028. No snapshot pointer, index, or handle is supplied by the caller, and no return value is part of the demonstrated contract.

## Confirmed behavior

### AlphaSmart 3000

The restore path reconstructs the saved 4 x 40 character display from the internal snapshot and then reapplies the saved display/cursor state.

### NEO

The restore path reconstructs all layers captured by A028:

- the System 3 software framebuffer/shadow representation;
- display RAM in both physical LCD controllers;
- the associated display/cursor state fields.

The 2005 and 2013 NEO implementations are structurally equivalent with relocated firmware globals and helper addresses.

## Relationship with A028

A028 and A02C hard-code the same internal snapshot object on each analyzed firmware generation. This establishes a paired save/restore facility rather than an externally managed buffer API.

The demonstrated storage model is a **single snapshot**. A later A028 replaces the state that A02C will restore; there is no firmware-level stack of snapshots and no handle selection argument.

A02C also does not demonstrate a guard requiring a previous A028 call. Code using the facility should therefore treat a valid prior save as a precondition rather than assuming useful initialized snapshot contents.

## Caller evidence

In the inspected AlphaWord Plus 2005 and 2012 binaries, A02C appears only in the imported System 3 syscall table; no additional executable A02C caller was identified there. This is a scoped negative result and is not generalized to the complete official SmartApplet corpus.

## Refutation checks

The closure rejects the following narrower interpretations:

- **cursor-only restore:** inconsistent with complete display reconstruction;
- **caller-provided snapshot:** inconsistent with the fixed firmware-owned object;
- **selection among multiple snapshots:** unsupported by both the ABI and storage model;
- **recovered vendor name `RestoreScreen`:** not established. The functional restore-screen interpretation is confirmed, but the original symbol remains unknown.

## Validation

A deterministic static regression was executed against all three canonical firmware generations and the two inspected AlphaWord Plus binaries: **91/91 checks passed**. The suite checks canonical firmware identity, A02C wrapper and restore-helper identity, fixed snapshot storage, the A028/A02C pairing, AS3000 4 x 40 restoration, NEO shadow/LCD/state restoration, cross-generation NEO structure, and the scoped caller search.

Dynamic emulator probing remains useful as a later regression, but it is not required to establish this static ABI contract.
