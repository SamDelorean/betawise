# A100 / index64 — DialogDraw — source-first closure

Status: **CORE MECHANICS CLOSED / SOURCE-FIRST / CORRELATED-CORPUS REVALIDATED**

## Recovered contract

```c
void DialogDraw(void);
```

BetaWise historically declares this as `void DialogDraw();`. The syscall table fixes `DialogDraw` at index 64, therefore its A-line opcode is `A100`. No separate C wrapper is required for the recovered public contract.

## Confirmed role

`DialogDraw` materializes the current dialog layout and visible window before the interactive `DialogRun` phase. It is not the choice setter and it is not the keyboard/input loop.

The normal dialog flow independently observed in historical/debug and System 3 callers is:

`DialogInit` → add items / exit keys → `DialogSetChoice` → `DialogDraw` → `DialogRun` → choice getters.

## Core layout behavior

The recovered core behavior is:

1. if the dialog contains no items, return;
2. scan the rendered item lengths and find the maximum;
3. compute `item_width = max_rendered_item_length + 3`;
4. preserve a preconfigured nonzero `columns` value;
5. when `columns == 0`, compute `columns = col / item_width`;
6. arrange choices in row-major order;
7. use the inclusive row interval from `row_first` through `row_last` for the visible row count;
8. treat `first_visible` as a 1-based item index;
9. normalize/clamp the visible window against `item_count` before rendering.

`DialogInit(single=true, ...)` initializes `columns` to 1. `single=false` initializes it to 0, which requests the automatic column calculation above. Thus `single=false` is not equivalent to a forced single-column dialog.

## Row-major model

For `N = columns`, choices `1..N` occupy the first visible row, `N+1..2N` the next row, and so on. Conceptually, for a valid normal geometry:

```text
visible_rows = row_last - row_first + 1
last_visible = min(first_visible + visible_rows * columns - 1, item_count)
```

The display path normalizes the window before drawing.

## DialogAddItem metadata

The render path consumes the metadata accumulated by `DialogAddItem`. The private correlated analysis also finds the per-item `file_size` presentation field consumed in this rendering path; it is presentation metadata here, not evidence that `DialogDraw` performs file I/O.

## Cross-generation result

The correlated canonical AS3000 2005, NEO 2005 and NEO 2013 maps delimit the A100 handler with the same `0x7E` span, and direct prior analysis of those implementations recovers the same public core geometry described above.

Equal span does **not** imply byte-for-byte identity, and this closure deliberately does not claim that.

## Historical continuity

An older `DialogModule` object independently uses the same conceptual family of row-grid display/input operations. This supports continuity of the layout model but is not used as a substitute for the later System 3 handler analysis.

A System 3 internal caller also invokes the sequence containing `DialogDraw` between `DialogSetChoice` and `DialogRun`, independently confirming its position in the dialog subsystem.

## Validation boundary

This pass revalidated the source signature, syscall index, three-generation correlated handler map, subsystem state relationships, and previously recovered core geometry.

The raw ROM binaries were not freshly re-exported/rehashed in this publication pass, so no new byte-level regression count is claimed here. Dynamic visual/keyboard regression is also **not executed**.

Still outside the closed public core contract:

- exact cosmetic rendering details;
- pathological geometries that can produce unusable widths/column counts;
- downstream behavior after an invalid `current_choice` value;
- platform-specific cosmetic differences;
- pixel/character-exact emulator comparison.

## Confidence

- **CONFIRMED:** A100/index64 identity; no-argument `void` contract; empty-dialog early return; maximum rendered width plus three; explicit-vs-automatic column behavior; row-major arrangement; inclusive visible-row range; 1-based `first_visible`; viewport normalization; render-before-`DialogRun` role; same recovered core contract across the three canonical generations.
- **STRONG INFERENCE:** conceptual continuity with the older DialogModule implementation.
- **OPEN FOR DYNAMIC REGRESSION:** exact visual presentation and pathological/invalid-state behavior.

Raw firmware, private offsets/disassembly and audit corpus remain outside the public repository.
