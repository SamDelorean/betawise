# SYS_A0C4 — Slow Keys state getter closure

Status: **CLOSED (source-first)**  
ABI identity: **CONFIRMED**  
Functional identity: **STRONG INFERENCE**  
Vendor symbol / original typedef: **UNKNOWN**

## Source correlation

BetaWise retains index 49 only as `SYS_A0C4`; `os3k.h` has no prototype for the A0B4..A0D0 gap between `IsKeyDownNow` and the sleep functions. No reproducible vendor symbol was recovered from the available source-first material.

AlphaWord Plus 2012 contains the user-facing setting `Slow Keys status`, separately from `Sticky Keys, Auto Repeat, and Show Marks status`. This is supporting functional correlation only; it is not used as primary proof and does not justify inventing a vendor symbol.

## Raw ABI contract

All three canonical ROMs implement the same 8-byte handler shape:

```text
MOVE.B <active>.L,D0
RTS
```

| Generation | Handler | File offset | Active byte | Shadow/config byte |
|---|---:|---:|---:|---:|
| AS3000 2005 | `0x004CDFB4` | `0x0000DFB4` | `0x00004344` | `0x00004352` |
| NEO 2005 | `0x005D1F98` | `0x00011F98` | `0x000057CE` | `0x000057DC` |
| NEO 2013 | `0x00422F88` | `0x00012F88` | `0x00005D26` | `0x00005D34` |

Exact bytes:

```text
AS3000: 10 39 00 00 43 44 4E 75
NEO05 : 10 39 00 00 57 CE 4E 75
NEO13 : 10 39 00 00 5D 26 4E 75
```

After neutralizing only the relocated absolute operand, all three handlers have the same SHA-256: `43c97db25353073aaf7a135445cec6b0d561fa67b19102f7b3b0df1fda36e958`.

A0C4 consumes no arguments and performs no write. It defines only `D0.b`; the upper bits of `D0` are preserved and are not part of the raw contract. A conservative C-facing description is therefore `uint8_t SYS_A0C4(void)`.

## State-family reconstruction

The active byte has exactly 11 aligned absolute references in each canonical ROM. Its lifecycle is conserved across generations:

- explicit clear during keyboard-state initialization;
- a setter that writes the same byte argument to both active and shadow/configuration bytes;
- independent active/shadow accessors;
- internal consumers that test the active byte.

The active byte is explicitly cleared at `0x004CDE6E`, `0x005D1E1E`, and `0x00422E0E`. The active+shadow setter begins at `0x004CDF70`, `0x005D1F54`, and `0x00422F44`.

This is a distinct state pair from the A0BC/A0C0 Sticky Keys family and the A0B8 Auto Repeat family. A0C4 therefore cannot be identified with either of those settings merely from ABI adjacency.

## Internal caller and return width

Each canonical ROM contains one direct absolute `JSR` to A0C4 in the reconstructed internal path:

```text
JSR A0C4
MOVE.B D0,D4
CMPI.B #1,D4
```

Caller addresses are `0x004CAAF8`, `0x005CA650`, and `0x0041AE34`. The byte consumption independently confirms the return width.

When the byte equals exactly `1`, the same path calls A0C8. A0C8 reads a related word state (`0x4348`, `0x57D2`, `0x5D2A` respectively). This enable-byte plus associated-word pattern, combined with the independent AlphaWord Plus `Slow Keys status` anchor, supports the Slow Keys interpretation strongly.

AlphaWord Plus 2012 and Control Panel 2012 were also checked for literal A0C4 occurrences. In those two samples, A0C4 appears only in contiguous A-line stub/import tables; no additional executable A0C4 caller was recovered. This is a deliberately limited negative result and is not extrapolated to the full applet corpus.

## Refutation and NEO 2013 mapping correction

A provisional NEO 2013 mismatch was refuted: this ROM is mapped at runtime base `0x00410000`, not `0x00400000`. Using the wrong base points file extraction into unrelated code and creates a false generational divergence. With the correct base, A0C4 is structurally identical in all three canonical ROMs.

The Sticky Keys and Auto Repeat alternatives are also rejected because they use different globals and independently reconstructed state families. A0C4 itself returns the stored byte without normalization; the observed caller treats exactly `1` as enabled.

## Regression

Static/contract regression was **executed** and passed across all three canonical ROMs. It covered:

- 3/3 canonical SHA-256 identities;
- index-49 vector and runtime/file mapping;
- exact 8-byte handler and common normalized hash;
- active/shadow global relocation;
- 11 aligned active-global references per ROM;
- explicit zero initialization;
- active+shadow setter correlation;
- direct internal caller and `D0.b` consumption;
- AlphaWord Plus functional text anchor;
- limited negative caller search in AlphaWord Plus 2012 and Control Panel 2012.

Dynamic hardware/emulator probing remains a separate future layer and is **not executed** here.

## Final classification

- **CONFIRMED:** syscall index/address, exact handler bytes, no arguments, no writes, raw byte load into `D0.b`, upper `D0` bits non-contractual, active/shadow state-family mechanics, zero initialization, and cross-generation equivalence.
- **STRONG INFERENCE:** getter for the active Slow Keys enable/state byte; A0C8 supplies an associated word parameter when enabled.
- **UNKNOWN:** original vendor symbol, historical typedef, exact semantics/unit of A0C8, and supported meaning of non-boolean byte values.

The neutral symbol `SYS_A0C4` is retained. Sequential audit may proceed to **A0C8 / index 50** after persistent checkpoint reconciliation.
