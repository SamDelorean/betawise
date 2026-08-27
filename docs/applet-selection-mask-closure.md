# A260–A270 SmartApplet selection-block mask closure

## Status

`A260`, `A264`, `A268`, `A26C` and `A270` are mechanically closed with
**confidence A** across the compared AlphaSmart 3000 and NEO System 3 ROMs.
Reliable original public/vendor symbols have not been recovered, so the neutral
`SYS_Axxx` names remain.

## Shared model

The five services operate on the same runtime table of 32-bit masks indexed by
SmartApplet runtime index. A nonzero aggregate mask prevents the selection path
used by A22C; the table must not be modeled as a Boolean array.

## ABI contracts

```c
uint32_t SYS_A260(uint32_t applet_index);
void SYS_A264(uint32_t applet_index);
void SYS_A268(uint32_t applet_index);
void SYS_A26C(uint32_t applet_index);
void SYS_A270(uint32_t applet_index);
```

- `A260` returns the complete 32-bit mask at `applet_index`. It performs no
  range check.
- `A264` sets bit 0 of the selected entry and preserves every other bit. It
  performs no range check.
- `A268` clears the complete 32-bit entry. It performs no range check.
- `A26C` accepts runtime indexes 1 through 31, selects a bit from a global
  reason/selector value and ORs `1 << selector` into the entry.
- `A270` uses the same validation and selector, then clears only that bit with
  an AND mask.

The exact private name and higher-level meaning of the global selector used by
A26C/A270 remain unresolved.

## Cross-ROM and caller evidence

The table shape and all five contracts are stable in the three compared ROMs.
A260 is called by AlphaQuiz, AlphaWord Plus, ControlPanel and KeyWords; A264 and
A268 appear in ControlPanel enable/disable paths; A26C and A270 occur as a pair
in AlphaQuiz and AlphaWord Plus. Later System 3 callers also associate these
paths with known SmartApplet IDs such as SpellCheck and Calculator.

## Safety and validation

A260, A264 and A268 do not validate `applet_index`; callers must keep it within
the 32-entry runtime table. A26C and A270 reject indexes outside 1–31 internally.
A268 clears every accumulated blocking reason, whereas A270 clears only the
currently selected reason bit.

No emulator/hardware regression is claimed by this closure.

## Publication boundary

This specification and the generic header contain only the reconstructed
contract. ROM bytes, verbatim disassembly, complete xrefs and private workpapers
remain outside the public repository.
