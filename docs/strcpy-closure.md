# A38C / `strcpy` closure

A38C (index 227) is mechanically reconstructed as:

```c
char *strcpy(char *dst, const char *src);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004DDC0E`, file `+0x01DC0E`
- NEO 2005: runtime `0x005E032C`, file `+0x02032C`
- NEO 2013: runtime `0x0043672A`, file `+0x02672A`

The contractual handler is 0x16 bytes and byte-identical across all three ROMs (SHA-256 `50054e3d38a584435f567a7c20a921920dada63b58334adae697f339fd2dcfa2`). It consumes two pointer slots, copies bytes from `src` to `dst` until and including the first NUL byte, deliberately preserves the original destination pointer, and returns that pointer in `D0.L`.

There is no count/capacity argument, NULL validation, overlap handling, helper call, global state or table access in the handler.

## Caller correlation

The validated official corpus is complete at 41/41. The corrected physical A-line table target (`A308 + 0x32`, after omission of A30C–A328) finds 302 executable A38C calls in 24 table-bearing applets. Six table-bearing applets are negative and all 11 structural negatives lack the A-line table.

The same detector reproduces the established positive controls A36C=99, A378=598, A380=130, A384=24 and A388=244. A representative KeyWords caller computes the source pointer, pushes it first, pushes the destination second, invokes A38C and removes exactly 8 bytes. This matches the two-argument 68k C calling convention.

Direct absolute firmware JSR counts are 25/25/27 for AS3000 2005 / NEO 2005 / NEO 2013; no direct JMP entry use was found.

The explicit return is not inferred from caller usage: the handler saves the original destination solely to reconstruct `D0.L` immediately before `RTS`, making the destination return part of the mechanical contract.

## Adversarial alternatives

- `memcpy` and `strncpy` are rejected because there is no count argument.
- `memmove` is rejected because there is neither a count argument nor overlap-dependent direction selection.
- `strcat` is rejected because the handler never searches for the existing destination terminator before copying.
- A void proprietary string copy is rejected because the original destination is deliberately preserved and returned in `D0.L`.

The historical BetaWise index-227 `strcpy` mapping and existing `os3k.h` declaration are secondary corroboration only.

## Source-first revalidation — 2026-09-04

The sequential source-first audit rechecked the historical anchors before accepting the libc identity: `syscall.c` maps index 227 to `strcpy`, while `os3k.h` declares `char *strcpy(char *dst, const char *src)`. Those names/signatures remain corroboration rather than proof.

The canonical extraction is consistent 3/3 at the entry points above. The preserved exact handler bytes are:

```text
2f0a226f0008206f000c244912d866fc200a245f4e75
```

They are 22 bytes and independently hash to `50054e3d38a584435f567a7c20a921920dada63b58334adae697f339fd2dcfa2`. Decoding yields: save `A2`; load `dst` and `src` from two 32-bit ABI slots; preserve original `dst` in `A2`; `MOVE.B (A0)+,(A1)+`; loop on nonzero copied byte; move original `dst` to `D0.L`; restore `A2`; `RTS`. This directly establishes NUL-terminated copying and the destination-pointer return, with no helper/global/table side effects.

The prior complete caller corpus remains consistent with that ABI: 302 executable calls in 24 applets, representative two-pointer push order with 8-byte cleanup, and direct ROM JSR/JMP counts 25/0, 25/0 and 27/0. No contradictory caller was identified.

Classification after revalidation: **CONFIRMED** for index, handler mechanics, two-pointer ABI, copied-NUL termination, `D0.L=dst`, absence of count/helpers/globals and cross-generation identity; **INFERENCIA FUERTE** only for treating the standard-library name as the vendor-facing identity beyond the historical headers; **DESCONOCIDO**: none material to the contract. Dynamic emulator-first regression remains **SPECIFIED / NOT EXECUTED**.

## Validation status

Static regression over the canonical ROMs and official SmartApplet manifest executed with **OVERALL PASS**. It validates ROM hashes, exact handler bytes/hash, direct JSR/JMP counts, the 302-call/24-applet corpus, detector controls and a representative two-slot caller. Dynamic emulator-first regression was not executed for this mechanically determined entry.

Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**. The existing `os3k.h` declaration already matches the reconstructed contract, so no header modification is required.
