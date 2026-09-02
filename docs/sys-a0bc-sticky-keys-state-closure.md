# SYS_A0BC — active Sticky Keys state getter closure

Status: **CLOSED (source-first)**  
ABI identity: **CONFIRMED**  
Functional identity: **STRONG INFERENCE**  
Vendor symbol / original typedef: **UNKNOWN**

## Source correlation

BetaWise retains index 47 only as `SYS_A0BC`; no trustworthy vendor prototype was recovered from the available SDK header/documentation. Product documentation describes Sticky Keys as disabled by default, while Auto Repeat is enabled by default. That behavior is used only as a hypothesis anchor and is checked against firmware.

## Raw ABI contract

The three canonical firmware generations implement the same 8-byte handler shape:

```text
MOVE.B <generation-specific-global>.L,D0
RTS
```

| Generation | Handler | Active byte |
|---|---:|---:|
| AS3000 2005 | `0x004CE04E` | `0x00004342` |
| NEO 2005 | `0x005D2032` | `0x000057CC` |
| NEO 2013 | `0x00423022` | `0x00005D24` |

Minimum justified raw contract:

```c
/* vendor name unknown */
uint8_t SYS_A0BC(void);
```

The handler has no arguments and no writes. It returns the raw byte in `D0.b`; it does not normalize to 0/1 and does not define or clear `D0[31:8]`.

## Functional reconstruction

The active byte has exactly 11 absolute references in each canonical ROM. After relocation the lifecycle is structurally identical:

- explicit initialization to `0`;
- paired shadow/configuration byte also initialized to `0`;
- writers from an argument byte;
- shadow/configuration-to-active restore;
- A0BC getter;
- multiple internal consumers using `CMPI.B #1,<active-byte>` in keyboard/modifier-processing paths.

Active/shadow pairs:

| Generation | Active byte | Shadow/config byte |
|---|---:|---:|
| AS3000 2005 | `0x4342` | `0x4350` |
| NEO 2005 | `0x57CC` | `0x57DA` |
| NEO 2013 | `0x5D24` | `0x5D32` |

This pattern strongly identifies the byte as the **active Sticky Keys enable state**. The interpretation is reinforced by the neighboring A0B8 state family: A0B8 initializes to `1` and gates keyboard auto-repeat, whereas this family initializes to `0` and is consumed repeatedly in modifier handling. That default split matches documented Auto Repeat-on / Sticky Keys-off behavior and refutes Auto Repeat as the A0BC meaning.

As with A0B8, the getter returns a raw byte. Observed firmware consumers treat exactly `1` as enabled.

## Refutation attempts

- **Auto Repeat:** rejected by default value and by the separate A0B8 repeat-path state.
- **Slow Keys:** unsupported; A0BC is a byte boolean-like state used in modifier logic, not a timing value.
- **generic modifier mask:** rejected because the state is a single active/shadow preference byte and consumers compare it to exactly `1`; modifier masks are represented elsewhere in the ABI.

## Regression

Static/contract regression was **executed**.

Result: **18/18 PASS** over the three canonical ROMs, checking:

- exact 8-byte handler body;
- exactly 11 absolute references to the active byte;
- explicit clear/default initialization;
- writers from an argument byte;
- shadow/configuration-to-active restoration;
- at least four compare-to-`1` consumers per generation.

Dynamic probe/emulator validation remains a separate future layer and is not claimed here.

## Final classification

- **CONFIRMED:** syscall index/address, handler bytes, no-argument ABI, raw byte return, active/shadow globals, default zero, writer/restore structure, compare-to-1 consumers, cross-generation equivalence.
- **STRONG INFERENCE:** returned byte is the current/active Sticky Keys enable state.
- **UNKNOWN:** original vendor symbol and exact source typedef.

Sequential audit may proceed to **A0C0 / index 48**.
