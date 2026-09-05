# A3B4 / index 237 — source-first generational closure

Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**.

`SYS_A3B4` is a project-neutral label. No independently reproducible vendor symbol or historical prototype has been recovered, so this document deliberately does not invent a semantic name.

## Source correlation

The reconstructed syscall index places index 237 immediately after `ungetc`, but that position is only an anchor, not evidence that A3B4 is another libc service. Searches across the available BetaWise/reference material did not recover an independent vendor name or prototype for index 237. Firmware therefore controls the contract below.

## Platform availability

Fresh revalidation against the three canonical ROMs confirms a real generational split:

- AlphaSmart 3000 (2005): the index-237 table slot contains `0x01020304`, not a demonstrable firmware code pointer.
- NEO (2005): the same slot contains `0x01020304`, not a demonstrable firmware code pointer.
- NEO (2013): index 237 points to runtime `0x0043ED3E` (file offset `0x02ED3E`) and has a real handler.

The neighboring old-firmware slots also contain non-pointer patterns. These values are recorded mechanically; they are **not** labeled `reserved`, `unimplemented`, or assigned vendor meaning. There is therefore no callable A3B4 contract for the two 2005 ROMs.

## NEO 2013 raw contract

The handler consumes one 32-bit stack slot as a required mutable pointer. It accesses bytes beginning at offsets `+0`, `+1`, `+2`, and `+3` and performs a synchronous hardware-backed transaction through private helpers and memory-mapped state.

The narrowest justified public representation remains:

```c
/* NEO 2013 only; vendor name and record layout unknown. */
uint16_t SYS_A3B4(void *record);
```

This declaration preserves the observed 16-bit result width; it does **not** claim that the firmware's semantic type is unsigned. Semantic signedness is unknown. The epilogue explicitly moves a word into `D0`, and callers consume `D0.W`. Observed raw bit patterns include `0x0000`, `0x0001`, `0x0004`, `0xFFFE`, `0xFFFD`, and `0xFFFC`. No names are assigned to those values.

## Handler, helpers, and callers

Fresh primary revalidation reproduces the NEO 2013 handler size of `0x12A` bytes and exact handler SHA-256:

`85479553c51ada4171c8ced37cf4eefe609fdd95fa38caba196b6de7b3f21424`

The archived deterministic 68k decode contains 115 instructions with no unknown instructions. Two private helpers used by this path remain private evidence. Firmware xrefs reproduce 8 direct absolute `JSR` sites and 2 direct `BSR.W` sites.

The existing full SmartApplet regression remains **EJECUTADA / PASS**: 41/41 corpus objects classified, with 16 NEO applets carrying the A3B4–A3BC extension, 14 legacy tables ending at A3B0, and 11 structural negatives. Exactly one executable A-line A3B4 caller was found: NEO ControlPanel. Fresh revalidation of that caller again shows one local pointer argument, consumption of the returned word, and comparison of the raw word result.

A fresh source-first directed regression covering the three canonical ROM hashes, vector entries, NEO 2013 handler, private-helper hashes, firmware xrefs, and the ControlPanel callsite is also **EJECUTADA / PASS**. Dynamic/emulator regression remains **ESPECIFICADA / NO EJECUTADA**.

## Confidence classification

- **CONFIRMADO:** A3B4 has no demonstrable callable handler in the two 2005 canonical ROMs and has a real implementation in NEO 2013.
- **CONFIRMADO:** NEO 2013 takes one mutable pointer-like argument and returns a raw 16-bit value in `D0.W`.
- **CONFIRMADO:** the handler mutates/reads record bytes beginning at offsets `+0..+3` and interacts with hardware/private state.
- **INFERENCIA FUERTE:** the pointed object is a transaction/status record whose first bytes participate in the hardware exchange.
- **DESCONOCIDO:** vendor function name, high-level subsystem name, semantic field names, semantic status names, and signedness of the status domain.

## Adversarial conclusions

- Rejected: identifying A3B4 as a standard-library routine solely because it follows `ungetc`.
- Rejected: treating `0x01020304` as a valid AS3000/NEO-2005 handler address.
- Rejected: inventing a vendor name, record layout, or semantic error enum.
- Rejected: publishing a portable all-platform callable header when primary evidence supports a callable implementation only on NEO 2013.

Full ROM bytes, extensive disassembly, helper listings, and firmware workpapers remain private in Drive.
