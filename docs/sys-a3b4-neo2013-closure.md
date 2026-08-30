# A3B4 / index 237 — generational closure

Status: **mechanically closed** from primary firmware evidence. The original vendor symbol/name remains unknown, so the neutral `SYS_A3B4` label is retained.

## Platform availability

- AlphaSmart 3000 (2005): the index-237 table slot contains `0x01020304`, not a firmware code pointer.
- NEO (2005): the same slot contains `0x01020304`, not a firmware code pointer.
- NEO (2013): index 237 points to runtime `0x0043ED3E` (file offset `0x02ED3E`) and has a real handler.

The neighboring old-firmware slots also contain non-pointer patterns. These values are recorded mechanically; they are **not** labeled `reserved`, `unimplemented`, or given any vendor meaning.

## NEO 2013 mechanical contract

The handler accepts one 32-bit stack slot used as a required mutable pointer. It accesses fields beginning at offsets `+0`, `+1`, `+2`, and `+3` and performs a synchronous hardware-backed transaction through private helpers and memory-mapped state.

The only independently justified public-width contract is therefore:

```c
/* NEO 2013 only; vendor name and record layout unknown. */
uint16_t SYS_A3B4(void *record);
```

This is a **raw 16-bit status/result** contract, not a recovered semantic enum. The handler deliberately finishes with a word move into `D0`; official and firmware callers consume `D0.W`. Observed raw outcomes include `0x0000`, `0x0001`, `0x0004`, `0xFFFE`, `0xFFFD`, and `0xFFFC`. No names are assigned to these values.

## Handler and callers

NEO 2013 handler size is `0x12A` bytes, with 115 decoded 68k instructions and one final `RTS`. SHA-256 of the exact handler bytes is:

`85479553c51ada4171c8ced37cf4eefe609fdd95fa38caba196b6de7b3f21424`

Firmware xrefs: 8 direct absolute `JSR` sites and 2 direct `BSR.W` sites. The observed callers pass one pointer-like argument and several copy/test the word result.

The official SmartApplet corpus was checked 41/41 using the existing corpus classification. Of the 30 applets with A-line tables, 16 NEO applets include the A3B4–A3BC extension and 14 applets use legacy tables ending at A3B0. The remaining 11 are structural negatives without the table. Exactly one executable A-line A3B4 caller was found: NEO ControlPanel. It passes a local pointer and compares the returned word against raw values including `0xFFF0` and zero.

## Private evidence

Full firmware disassembly, ROM bytes, helper listings, caller manifests, hashes, and regression material remain in the private Drive workpapers. The static regression covering ROM hashes, generational table values, handler boundary/hash, branch targets, helpers, firmware xrefs, SmartApplet table classification, and the ControlPanel callsite reports **OVERALL PASS**. Dynamic/emulator regression has not been executed.

## Adversarial conclusions

- Rejected: treating A3B4 as another standard-library routine solely because it follows the libc region.
- Rejected: treating `0x01020304` as a valid AS3000/NEO-2005 code address.
- Rejected: inventing a vendor name or semantic error enum for the word return values.
- Rejected: publishing a portable all-platform callable header when primary evidence only supports an implementation on NEO 2013.

A3B4 should therefore be consumed as a neutral, NEO-2013-specific raw ABI until stronger symbol or subsystem evidence is recovered.
