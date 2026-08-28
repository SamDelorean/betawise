# System 3 / OS3K AppletSendMessage — A244 SDK consolidation

This note is the developer-facing consolidation record for A244, exposed by BetaWise as:

```c
uint8_t AppletSendMessage(
    uint32_t index,
    Message_e message,
    uint32_t param,
    uint32_t *status);
```

## 0. Consolidation status

A244 reached this SDK-consolidation pass after the reverse-engineering process had already closed its mechanical contract with confidence **A**. This pass does not repeat ROM/disassembly work. It audits the delivered contract against the current BetaWise declaration, A-line stub and SmartApplet runtime reference, then records the contract application code should rely on.

Current state:

- mechanical contract: **A**, received from the reverse-engineering process;
- public identity: `AppletSendMessage`, name continuity **strong**;
- BetaWise declaration/stub: **implemented and audited**;
- prior `int` return and `uint8_t index` declaration: **superseded** by the delivered byte-return / 32-bit-index machine contract;
- public wrapper: **not required**; the SDK symbol is the direct A244 service;
- dynamic emulator/hardware regression: **pending** and should be emulator-first because the target callback may have side effects.

If later primary evidence changes the upstream mechanical contract, this SDK contract must be reconciled and revised rather than silently preserving an obsolete conclusion.

## 1. Public contract

```c
uint8_t AppletSendMessage(
    uint32_t index,
    Message_e message,
    uint32_t param,
    uint32_t *status);
```

A244 consumes four 32-bit argument slots. `Message_e` remains the public source type used by BetaWise/`ProcessMessage`, while the underlying message slot is handled as a full 32-bit value.

The operation dispatches a message to another System 3 SmartApplet while temporarily installing the target applet's OS-owned runtime context.

## 2. Target validation

For a nonzero target index, A244 requires all of the following:

- `index < 32`;
- the runtime applet-table entry is installed/non-NULL;
- the target's selection-block mask is zero.

A rejected nonzero target returns contractual low byte `0` without changing the active runtime context.

Index `0` is special: the handler bypasses the ordinary nonzero-target range/table/block-mask validation path and proceeds through the System-applet dispatch path.

The index argument is therefore a **runtime SmartApplet index**, not a SmartApplet ID.

## 3. Message normalization

Raw message values `0x1C` and `0x1D` are passed through unchanged. Their original symbolic names have not been recovered and are intentionally left unnamed.

Every other message is normalized before delivery as:

```text
normalized = (message & 0x00FFFFFF) | 0x01000000
```

This matches BetaWise's existing `MSG_MOD_SYNTHETIC = 0x01000000` namespace and establishes A244 as the normal System 3 mechanism for private/synthetic cross-applet messages.

The target callback receives:

```c
ProcessMessage(normalized_message, param, status);
```

`param` and the `status` pointer are forwarded unchanged. A244 does not itself make a NULL `status` pointer safe; SDK callers should provide a valid `uint32_t` status object unless the destination applet's own message contract explicitly permits otherwise.

## 4. A5 and current-applet context

Before invoking the destination callback, A244 installs the target applet's OS-owned A5 context and records the previous A5 value. It also saves the previous current-applet runtime index and installs the target index for the duration of the callback.

After the callback returns, A244 restores both the previous A5 context and the previous current-applet index.

This is part of the resident service's contract and is direct support for BetaWise's long-standing `-ffixed-a5` build rule: A5 belongs to the OS3K applet runtime and must not be treated as a normal compiler register.

Application code should use A244 rather than attempting to reproduce these context switches manually.

## 5. Return contract

Successful dispatch returns full `D0 = 1`.

Rejection/failure clears only the **low byte** of D0. Upper D0 bits are not contractual on that path.

Therefore the portable SDK return type is:

```c
uint8_t
```

with contractual low-byte meaning:

- `1` — dispatch accepted and callback invoked;
- `0` — target rejected before dispatch.

Earlier BetaWise material used `int`; that form could expose non-contractual upper D0 bits on rejection. The current `uint8_t` declaration is canonical.

## 6. BetaWise integration

The current working branch already matches the delivered ABI:

```c
uint8_t AppletSendMessage(uint32_t index, Message_e message, uint32_t param, uint32_t *status);
```

`os3k/syscall.c` maps A-line index 145 directly to `AppletSendMessage`, corresponding to trap A244.

No behavioral source change is required by this consolidation pass because the corrected argument and return widths had already been incorporated into the working branch.

## 7. Mechanical evidence received from reverse engineering

The upstream closure compared the A244 handler in all three canonical firmware generations used by this project:

| ROM | A244 runtime address | Handler length |
| --- | ---: | ---: |
| AlphaSmart 3000, Nov. 2005 | `0x4E0F02` | `0xAA` |
| NEO, Nov. 2005 | `0x5E385E` | `0xAA` |
| NEO/System 3.15, Jul. 2013 | `0x439C60` | `0xAA` |

The implementations are structurally equivalent. Differences are relocation operands for current-applet state, the selection-block helper, applet table and per-applet A5-context locations. Message normalization and callback sequencing are unchanged across the compared ROMs.

This consolidation treats that delivered mechanical closure as its upstream evidence package and does not independently redisassemble the handlers.

## 8. Relationship to C / BetaWise runtime

`AppletSendMessage` has no ISO C or hosted-library equivalent. It is a System 3 resident runtime service exposed through BetaWise's A-line syscall layer.

It is also more than a plain C function-pointer call: it validates the target, applies System 3 message normalization and owns the A5/current-applet context transition around the callback.

## 9. Safety and regression specification

Unlike A238/A23C/A240, A244 executes destination applet code. Even a mechanically correct call can therefore cause destination-specific state changes.

Minimum regression matrix:

1. call a rejected or selection-blocked nonzero target and verify contractual low byte `0`;
2. verify that rejection preserves previous A5/current-applet context;
3. use a cooperating test applet and a known benign private message;
4. confirm normalization to `0x01000000 | low24(message)` for an ordinary private selector;
5. confirm raw `0x1C` and `0x1D` are not synthetically modified;
6. verify callback receives the original `param` and `status` pointer;
7. verify contractual low byte `1` on successful dispatch;
8. verify A5 and current-applet state are restored after callback return.

Perform this regression emulator-first. Do not send arbitrary private selectors to irreplaceable hardware.

No emulator or hardware regression is claimed as executed by this consolidation note.

## 10. Canonical SDK status

A244 is now:

**RECEIVED FROM DEEP REVERSE ENGINEERING — MECHANICAL A → SDK CONSOLIDATED/AUDITED → PUBLISHED → REGRESSION PENDING**
