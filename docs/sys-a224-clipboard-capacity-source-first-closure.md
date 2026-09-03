# SYS_A224 — clipboard maximum capacity (source-first closure)

## Status

`A224 / index 137` is closed at **confidence A** under the source-first audit.
The original vendor name has not been recovered, so the generic `SYS_A224`
identifier is intentionally retained.

```c
uint32_t SYS_A224(void);
```

## Contract

`SYS_A224` returns the 32-bit maximum allocated capacity (`max_size`) held at
offset `+0x0c` of the dedicated System 3 clipboard descriptor.

It:

- receives no arguments;
- performs no helper calls;
- does not modify clipboard state;
- performs no explicit validation or guard; and
- returns the descriptor capacity field directly.

The existing declaration in `os3k/file_clipboard_state.h` is therefore confirmed
by the renewed audit and does not require an ABI change.

## Source correlation

BetaWise `syscall.c` identifies index 137 only as `SYS_A224`; no reliable public
vendor name was recovered from the reviewed SDK/documentation anchors. Earlier
clipboard notes proposed capacity semantics, but that characterization was
retained only as a search hypothesis until firmware revalidation.

Independent File API reconstruction also identifies descriptor `+0x0c` as
`max_size`, while neighboring clipboard traps distinguish storage at `+0x00` and
logical size at `+0x04`. This provides a separate structural anchor for the
field meaning.

## Firmware verification

The handler was independently re-extracted from the canonical November 2005
AlphaSmart 3000 ROM, November 2005 NEO ROM and July 2013 NEO/System 3.15 ROM.
All three canonical SHA-256 values were rechecked before analysis.

Across all three generations the handler is exactly `0x0c` bytes and has the
same three-instruction semantics: load the generation-specific clipboard
descriptor, load its longword at `+0x0c` into the return register, and return.
Only the absolute descriptor address relocates between generations. The next
handler, A228, begins exactly at the following byte boundary.

## Caller evidence

A renewed complete-ROM search found **0 / 0 / 0** direct absolute-JSR references
to the A224 handler. This negative result is recorded explicitly rather than
substituted with inferred callers. As a scanner control, the same search method
reproduced the already established A220 direct-JSR counts.

Earlier SmartApplet analysis remains corroborative context for use of this trap,
but the closure does not depend on that derived evidence: the complete handler,
field identity and cross-generation comparison are sufficient to establish the
mechanical contract.

## Refutation

Alternative interpretations were tested against the code and surrounding
structure:

- `current_size` is excluded because the logical-size path uses descriptor
  `+0x04`;
- a storage pointer is excluded because A220 returns descriptor `+0x00`;
- cursor/recoverable-size interpretations use different descriptor fields; and
- token/status/mutating interpretations are incompatible with the complete
  three-instruction read-only handler.

The return is therefore modeled as a 32-bit unsigned capacity value rather than
a Boolean or status code.

## Regression state

Static regression: **EXECUTED — 36/36 PASS** across the three canonical ROMs.
Checks cover canonical hashes, handler bounds, exact `0x0c` length, descriptor
relocations, the `+0x0c` read, return boundary, structure discrimination,
negative direct-JSR search and the A228 boundary.

Dynamic emulator/hardware regression: **SPECIFIED / NOT EXECUTED**.

A minimal dynamic regression should create a clipboard with known allocation,
verify that A224 equals the descriptor `max_size`, and confirm repeated calls do
not change tag, storage, logical/recoverable size or cursor state.

The next sequential audit target is A228/index138. Higher A500+ work remains
preserved and paused by project policy.
