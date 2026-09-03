# SYS_A264 (A264/index153) — source-first closure

Status: **CERRADO A / SOURCE-FIRST**

```c
void SYS_A264(uint32_t applet_index);
```

## Confirmed firmware semantics

All three canonical firmware generations implement the same operation:

```c
mask_table[applet_index] |= 0x00000001u;
```

- `applet_index` is a full 32-bit ABI value and is scaled by four.
- The table is the same runtime mask table read by `SYS_A260`.
- Only bit 0 is set; all other mask bits are preserved.
- There is no bounds check, helper call, branch, or validation.
- There is no contractual return value.
- The handler is structurally equivalent in AS3000 2005, NEO 2005, and NEO 2013 and is 0x16 bytes long.
- Complete direct-JSR searches found 2/2/2 firmware callers.

## Correlation

`SYS_A260` reads this same mask table, while `SYS_A22C` permits SmartApplet selection only when the aggregate mask is zero. Official AlphaSmart Manager 2 documentation exposes a user-facing `Disable This SmartApplet` operation. Together, these facts make the interpretation of bit 0 as a disabled-state component a **strong inference**, while the exact modern vendor symbol remains unknown.

## Adversarial checks

Rejected interpretations include:

- generic boolean setter,
- full-mask assignment,
- byte- or word-sized public index,
- bounds-checking API,
- result-mask return value.

## Regression

- Static: **31/31 PASS — EXECUTED**
- Dynamic: **SPECIFIED / NOT EXECUTED**

Raw firmware bytes and extended disassembly remain private.
