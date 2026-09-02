# A104 / index65 — DialogRun — source-first closure

Status: **CORE MECHANICS CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED**

## Recovered contract

```c
short DialogRun(void);
```

BetaWise historically declares `short DialogRun();`. The syscall table places `DialogRun` at index 65, therefore its A-line opcode is `A104`. No separate C wrapper is required for the recovered public contract.

`DialogRun` consumes no public arguments. Normal callers use its 16-bit result as a key/event word; high-byte modifier bits may remain present in that returned value.

## Role in the dialog subsystem

The normal flow is:

`DialogInit` → add items / exit keys → `DialogSetChoice` → `DialogDraw` → `DialogRun` → choice getters.

`DialogDraw` materializes layout and viewport. `DialogRun` is the interactive navigation/input phase.

## Public navigation semantics

The recovered handler explicitly recognizes the normal navigation keys before the general shortcut/exit-key scans:

- `KEY_HOME`: move to choice 1 and redraw when the selection changes;
- `KEY_END`: move to the final item and redraw when the selection changes;
- `KEY_RIGHT`: advance one item only when that does not cross the right edge of the logical row and an item exists;
- `KEY_LEFT`: move one item left only when not already in the first column;
- `KEY_UP`: subtract `columns` when a row exists above;
- `KEY_DOWN`: add `columns` only when an item exists directly below.

The normal navigation paths do not wrap between rows. In particular, an incomplete final row does not invent a `DOWN` target.

## Scrolling and viewport

When movement takes `current_choice` outside the visible window, the dialog adjusts `first_visible` in increments of `columns`, i.e. one logical row per ordinary scrolling step, and redraws. Larger jumps such as Home, End, or a shortcut can trigger a full redraw that re-establishes a viewport containing the selected item.

## Shortcuts and exit keys

Navigation has priority over the general shortcut and exit-key scans. Registering a normal navigation key as an exit key therefore does not turn its ordinary navigation path into an immediate dialog exit.

For other keys:

1. shortcuts are scanned first using the low byte of the current key/event;
2. each matching shortcut sets `current_choice = item_index + 1` and redraws;
3. the shortcut loop does not stop on the first duplicate, so the last matching item remains selected;
4. exit keys are scanned afterwards.

Therefore, when one ordinary key is both an item shortcut and an exit key, the item is selected/redrawn first and then `DialogRun` returns the key/event word.

Shortcut matching itself is byte-sized, so modifier bits in the high byte do not participate in the shortcut comparison even though they may remain in the returned word.

## Private event codes

The firmware contains explicit early handling for byte values `0x64`, `0x65`, `0x66`, and `0x67` in the dialog exit/event machinery. They participate in private System 3 paths and can produce behavior that is not equivalent to a normal keyboard exit.

Their vendor names and complete private contracts have not been recovered. They are deliberately not added to `Key_e` and are outside the closed public `DialogRun` contract.

## Cross-generation result

The canonical AS3000 2005, NEO 2005, and NEO 2013 ROMs were freshly recovered and rehashed during this audit. The vector interval from `A104` to `A108` is `0x424` bytes in all three images. The public entry reaches its first `RTS` at the same relative location in each generation, followed by private local helpers within the same vector interval.

The three blocks retain the same public control-flow structure and navigation constants. Direct absolute firmware callers found during the audit are 4 in AS3000 2005, 5 in NEO 2005, and 6 in NEO 2013. In each ROM, the only additional raw reference to the A104 entry is its A-line vector entry.

## Static regression

Static regression: **EXECUTED — 67/67 PASS**.

It verifies:

- exact canonical ROM sizes and SHA-256 hashes;
- `A104`/index65 and `A108`/index66 vector resolution;
- the common `0x424` vector interval and consistent public-entry/helper return anchors;
- no-public-argument entry structure;
- explicit Home, End, Left, Right, Up, and Down comparison constants;
- explicit presence of the private `0x64`–`0x67` event-code checks;
- direct caller and total-reference counts;
- a stable cross-generation structural fingerprint after masking the previously correlated relocation/displacement positions.

Dynamic visual/keyboard regression remains **SPECIFIED / NOT EXECUTED**.

## Confidence

- **CONFIRMED:** A104/index65 identity; no-argument `short` contract; 16-bit key/event return use; Home/End/arrow navigation; no-wrap rules; row-sized scrolling; navigation priority; shortcut-before-exit ordering; byte-sized shortcut selection; shortcut+exit interaction; three-ROM public-mechanics consistency; caller counts.
- **UNKNOWN:** vendor names and full private semantics of `0x64`–`0x67`.
- **OPEN FOR DYNAMIC REGRESSION:** exact visual effects, invalid-state/pathological geometry behavior, and unusual keyboard-layout edge cases.

Raw firmware, private offsets/disassembly, and audit corpus remain outside the public repository.
