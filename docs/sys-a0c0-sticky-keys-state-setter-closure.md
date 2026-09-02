# SYS_A0C0 — Sticky Keys state setter closure

Status: **CLOSED (source-first)**  
ABI identity: **CONFIRMED**  
Functional identity: **STRONG INFERENCE**  
Vendor symbol / original typedef: **UNKNOWN**

## Source correlation

BetaWise retains index 48 only as `SYS_A0C0`; no trustworthy vendor prototype was recovered from the available SDK header/documentation. The immediately preceding A0BC audit established, from firmware evidence, the active/shadow state pair strongly identified with Sticky Keys. ControlPanel 2012 also contains the user-facing string `Sticky Keys and Auto\r  Repeat status`, which is supporting subsystem correlation only.

## Raw ABI contract

All three canonical ROMs implement the same 18-byte handler shape:

```text
MOVE.B 7(SP),<active>.L
MOVE.B 7(SP),<shadow>.L
RTS
```

| Generation | Handler | Active byte | Shadow/config byte |
|---|---:|---:|---:|
| AS3000 2005 | `0x004CE01C` | `0x00004342` | `0x00004350` |
| NEO 2005 | `0x005D2000` | `0x000057CC` | `0x000057DA` |
| NEO 2013 | `0x00422FF0` | `0x00005D24` | `0x00005D32` |

Exact bytes:

```text
AS3000: 13 EF 00 07 00 00 43 42 13 EF 00 07 00 00 43 50 4E 75
NEO05 : 13 EF 00 07 00 00 57 CC 13 EF 00 07 00 00 57 DA 4E 75
NEO13 : 13 EF 00 07 00 00 5D 24 13 EF 00 07 00 00 5D 32 4E 75
```

The handler consumes the byte at `7(SP)` and copies it without normalization to both the active byte and its shadow/configuration byte. It performs no other direct state change and does not define a return value.

The stack access establishes the raw ABI byte location. It does **not**, by itself, justify inventing a historical high-level C parameter type.

## Functional reconstruction

The two destination globals are exactly the active/shadow pair used by A0BC. Their lifecycle, default value, and downstream consumers were independently reconstructed in that audit. Therefore A0C0 is strongly identified as the corresponding **raw Sticky Keys state setter**.

Observed downstream firmware consumers treat exactly `1` as enabled. A0C0 itself does not force the input into the domain `{0,1}`; arbitrary byte values are stored unchanged.

## Caller search and refutation

ControlPanel 2012 contains the complete A-line opcode table, so an isolated `A0C0` occurrence inside that table was explicitly excluded as a caller. A targeted sweep did not recover an unambiguous structural A0C0 wrapper from that binary; this negative caller result is preserved rather than promoted into evidence.

The alternative Auto Repeat interpretation is rejected: A0C0 writes the `0x4342/0x4350` family (and relocated equivalents), while A0B8 was independently tied to the different `0x4343/0x4351` family and its keyboard repeat gate.

## Regression

Static/contract regression was **executed**.

Result: **18/18 PASS** across the three canonical ROMs, covering canonical SHA-256 identity, exact handler bytes, active write, shadow write, final `RTS`, and identity of the destination pairs with the A0BC reconstruction.

Dynamic probe/emulator validation remains a separate future layer and is not claimed here.

## Final classification

- **CONFIRMED:** syscall index/address, handler bytes, byte read from `7(SP)`, two raw byte writes, no normalization, no handler-defined return, and cross-generation equivalence.
- **STRONG INFERENCE:** setter for the active/persisted Sticky Keys state associated with A0BC.
- **UNKNOWN:** original vendor symbol and exact historical high-level parameter type.

Sequential audit may proceed to **A0C4 / index 49**.
