# A214 / ClipboardClear — source-first closure

Status: **CLOSED A / SOURCE-FIRST**.

## Identity and provenance

BetaWise `syscall.c` carries index 133 / trap A214 as `ClipboardClear`. The broader source-first pass did not recover an independent primary AlphaSmart declaration that proves the exact modern vendor symbol, so `ClipboardClear` remains a **BetaWise-inherited name** whose semantics are confirmed directly from firmware.

Public contract:

```c
void ClipboardClear(void);
```

Confidence: **CONFIRMED (A)** for ABI shape and semantics; **inherited name** for nomenclature provenance.

## Firmware confirmation

Canonical ROM hashes were revalidated before analysis:

- AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Vector/handler locations:

| ROM | handler VA | file offset | next vector | span |
|---|---:|---:|---:|---:|
| AS3000 2005 | `0x004E0CB6` | `0x020CB6` | `0x004E0CEE` | `0x38` |
| NEO 2005 | `0x005E3612` | `0x023612` | `0x005E364A` | `0x38` |
| NEO 2013 | `0x00439A14` | `0x029A14` | `0x00439A4C` | `0x38` |

The three handlers are instruction-for-instruction equivalent after relocation of globals/helpers.

## Confirmed semantics

A214 takes no arguments and has no meaningful return value. In all three ROMs it:

1. loads the dedicated clipboard descriptor;
2. clears descriptor longwords at offsets `+0x08` and `+0x04`;
3. clears descriptor longword at offset `+0x14` (cursor/state field in the reconstructed File/clipboard model);
4. invokes the same private allocation/reclamation helper used by A20C, passing a zero-size request for the clipboard allocation domain;
5. clears the shared one-byte clipboard tag/metadata global;
6. invokes the same private synchronization/trace helper family used by A20C;
7. returns directly.

This confirms that `ClipboardClear` is not merely a logical-size setter: it resets clipboard bookkeeping **and** executes the allocation/reclamation path, consistent with releasing/resetting clipboard storage state.

## Cross-generation details

Descriptor pointer globals are platform-relocated (`0x344A`, `0x34A2`, `0x34DA`). The allocator domain globals are likewise relocated (`0x3442`, `0x349A`, `0x34D2`), and the shared clipboard tag lives at `0x3D6B`, `0x3DC3`, `0x3DFB` respectively.

The final private helper receives a ROM-specific source/trace token: `0x18A0B` in both 2005 images and `0x18A0F` in NEO 2013. No semantic divergence in the public A214 contract is visible from this difference.

## Xrefs / callers

Absolute internal JSR xrefs to the handler were re-extracted from each canonical ROM:

- AS3000 2005: 2 (`0x004DA564`, `0x004DA5C8`)
- NEO 2005: 2 (`0x005DCCCA`, `0x005DCD48`)
- NEO 2013: 3 (`0x004170F0`, `0x0042F4C0`, `0x0042F544`)

The extra NEO 2013 caller is a platform-era initialization/control path; it does not pass arguments and does not consume a return value, reinforcing the `void(void)` ABI.

## Refutation pass

The dominant minimal hypothesis — “A214 only zeroes the clipboard size/tag” — is **refuted**. Primary firmware shows a call through the shared private allocator/reclamation helper after zeroing descriptor fields. Conversely, no argument decoding, error branch, or return-value construction exists in the handler, supporting `void ClipboardClear(void)`.

## Classification

- **CONFIRMED:** no arguments; no meaningful return; descriptor `+0x04/+0x08/+0x14` cleared; shared tag cleared; allocator/reclamation helper called; final private sync/trace helper called; equivalent public behavior across all three canonical ROMs.
- **INFERENCE STRONG:** the zero-size allocator call releases or normalizes clipboard backing storage rather than only updating counters.
- **INHERITED NAME:** `ClipboardClear` from BetaWise.
- **UNKNOWN:** exact original vendor name/prototype source independent of BetaWise; exact vendor names for the two private helpers and the final trace token.

## Regression

Static regression was **EXECUTED: 48/48 PASS** against the three canonical ROMs. It checks ROM hashes, exact handler boundaries/span, opcode landmarks for all three cleared fields, allocator-domain argument, helper targets, tag clear, final helper/token, stack cleanup/RTS, and exact direct-JSR xref sets.

Dynamic/emulator regression remains **SPECIFIED / NOT EXECUTED**.

Firmware bytes and extended disassembly remain private and are intentionally not published here.
