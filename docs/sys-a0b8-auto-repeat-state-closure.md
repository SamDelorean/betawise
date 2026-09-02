# SYS_A0B8 — active Auto Repeat state getter closure

Status: **CLOSED (source-first)**  
ABI identity: **CONFIRMED**  
Functional identity: **STRONG INFERENCE**  
Vendor symbol / original typedef: **UNKNOWN**

## Why the symbol remains `SYS_A0B8`

The historical BetaWise syscall table names index 46 only as `SYS_A0B8`. Neither the recovered public SDK header nor the available OS3K documentation provides a vendor prototype for this index. This document therefore does **not** invent a vendor name.

## Raw ABI contract

The three canonical firmware generations implement the same 8-byte handler shape:

```text
MOVE.B <generation-specific-global>.L,D0
RTS
```

Canonical handler locations:

| Generation | Handler | Global read |
|---|---:|---:|
| AS3000 2005 | `0x004CE00C` | `0x00004343` |
| NEO 2005 | `0x005D1FF0` | `0x000057CD` |
| NEO 2013 | `0x00422FE0` | `0x00005D25` |

Therefore the minimum justified raw contract is:

```c
/* vendor name unknown */
uint8_t SYS_A0B8(void);
```

`D0.b` is overwritten with the byte read from the global. The handler does not normalize the value and does not define or clear `D0[31:8]`. There are no arguments and no memory writes in the handler.

## Functional reconstruction

Across all three canonical ROMs, the byte read by A0B8 has exactly seven absolute references and the same structural lifecycle after relocation:

- explicit initialization to `1`;
- writers that copy an argument byte into the active byte;
- a paired shadow/configuration byte initialized to `1`;
- a copy from the shadow/configuration byte back into the active byte;
- the A0B8 getter;
- an internal keyboard-path consumer that tests `CMPI.B #1,<active-byte>` before entering the repeat path.

The active/shadow pairs are:

| Generation | Active byte (A0B8) | Shadow/config byte |
|---|---:|---:|
| AS3000 2005 | `0x4343` | `0x4351` |
| NEO 2005 | `0x57CD` | `0x57DB` |
| NEO 2013 | `0x5D25` | `0x5D33` |

A neighboring state family uses a different active/shadow pair that is initialized to `0`. Product documentation describes **Auto Repeat as enabled by default** and **Sticky Keys as disabled by default**. Combined with the direct `== 1` test in the keyboard repeat path, this refutes Sticky Keys as the best interpretation of the A0B8 byte and strongly identifies it as the **active Auto Repeat enable state**.

Important nuance: A0B8 returns the raw byte. The observed internal consumer treats exactly `1` as enabled; the getter itself does not coerce arbitrary nonzero values to boolean true.

## External caller correlation

AlphaWord Plus 2012 contains one direct wrapper/trampoline reference for A0B8 (`MOVEA.L #$0000A0B8,A0` followed by the common call trampoline). The same app image contains the configuration labels `AutoRept:` and `StickKeys:`. This is supporting caller correlation, not the primary basis for the semantic claim.

No recovered source currently supplies a trustworthy vendor symbol for index 46, so `SYS_A0B8` remains the public reconstruction name.

## Refutation attempts

The following alternatives were considered and rejected or left unsupported:

- **Sticky Keys:** contradicted by the default value split: the A0B8 state initializes to `1`, while the neighboring state family initializes to `0`; official behavior documents Auto Repeat as on by default and Sticky Keys as off by default.
- **Slow Keys:** unsupported because the observed A0B8 state is a byte enable flag consumed by an exact compare-to-1 gate, whereas Slow Keys is a timing/delay policy.
- **generic keyboard-present/key-down flag:** contradicted by the active/shadow preference lifecycle and by the specific use in the repeat-processing path.

## Regression

Static/contract regression was **executed**, not merely specified.

Result: **21/21 PASS** covering:

- SHA-256 identity of all three canonical ROMs;
- exact 8-byte handler body in each ROM;
- exactly seven absolute references to the generation-specific active byte;
- explicit initialization to `1`;
- exact compare-to-`1` consumer in each ROM;
- writers from an argument byte;
- AlphaWord Plus 2012 A0B8 wrapper reference;
- `AutoRept:` / `StickKeys:` string anchors in the caller image.

Dynamic probe/emulator execution remains a separate future validation layer and is **not** claimed here.

## Final classification

- **CONFIRMED:** syscall index/address, handler bytes, no-argument ABI, byte-width return, raw byte semantics, referenced globals, initialization/write/read structure, compare-to-1 consumer, and cross-generation equivalence.
- **STRONG INFERENCE:** the returned byte is the current/active Auto Repeat enable state.
- **UNKNOWN:** original vendor symbol and exact source typedef.

Sequential audit may proceed to **A0BC / index 47**.
