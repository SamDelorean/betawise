# A39C / `strncpy` closure

A39C (index 231) is mechanically reconstructed as:

```c
char *strncpy(char *dst, const char *src, size_t num);
```

## Primary mechanics

Canonical entry points:

- AS3000 2005: runtime `0x004DDC62`, file `+0x01DC62`
- NEO 2005: runtime `0x005E0380`, file `+0x020380`
- NEO 2013: runtime `0x0043677E`, file `+0x02677E`

The contractual handler is 0x3E bytes and byte-identical across all three ROMs (SHA-256 `aed3189037cf76785032ac950109fa37a67451b01f24e0974f8f29860ca3ac79`). It consumes destination, source and a full 32-bit count, preserves the original destination pointer and returns that pointer in `D0.L`.

The count gate precedes the first source/destination access, so `num == 0` performs no string dereference or write and returns `dst`. For a positive count, bytes are copied from `src` to `dst` up to `num`. If a source NUL is encountered before count exhaustion, that NUL is copied and the handler fills every remaining destination byte with NUL until exactly `num` bytes have been written. If `num` is exhausted on a non-NUL source byte, the handler stops without appending an extra terminator.

These details are contractually important: truncation is not guaranteed to be NUL-terminated, while a short source is NUL-padded through the requested width.

## Caller correlation

The validated official corpus is complete at 41/41. The corrected physical A-line target (`A308 + 0x3A`) finds 34 executable A39C calls in 12 applets; the remaining 18 table-bearing applets and all 11 structural negatives have no A39C call.

Representative callers confirm three 32-bit argument slots and 12-byte cleanup. `text2speechupdate` passes a concrete count of 5 with independent source/destination pointers; `wirelessfiletransfer` passes a concrete count of 0x18. Other callsites use dynamic full-long counts.

Post-call `D0` liveness across all 34 sites found no incompatible use. One `text2speechupdate` call propagates the A39C return value through its enclosing function return, independently confirming that the returned original destination pointer is observable rather than incidental.

Direct firmware xrefs are 1/1/8 absolute JSRs for AS3000 2005 / NEO 2005 / NEO 2013; no direct absolute JMP or PC-relative BSR to the handler was found.

## Adversarial alternatives

- `strcpy` is rejected because A39C consumes a count and performs NUL padding.
- `strncat` is rejected because the handler never scans `dst` for its existing terminator before copying.
- `memcpy` is rejected because source NUL terminates source copying and triggers padding.
- `strlcpy`-like behavior is rejected because exhausting `num` on a non-NUL byte does not force a final terminator.

The historical BetaWise `strncpy` mapping and existing `os3k.h` declaration are secondary corroboration only. `size_t` is physically 32 bits on this ABI.

## Validation status

Static regression executed with **OVERALL PASS**. It validates all three canonical ROM hashes, exact handler length/hash, firmware xrefs 1/1/8, the 34-call/12-applet official corpus with 41/41 coverage, representative fixed-count callers and 12-byte cleanup, the zero-count no-access path, NUL padding, non-termination on truncation, and return-value propagation.

Dynamic emulator-first regression was not executed for this mechanically determined entry.

## SOURCE-FIRST re-audit — 2026-09-04

The re-audit began from source/API anchors before accepting the historical symbol. BetaWise master maps index 231 to `strncpy`; the public `ioma8/neo-re` snapshot independently preserves both `DEFINE_SYSCALL(231, strncpy)` and the matching `char *strncpy(char *dst, const char *src, size_t num)` declaration. A separate recovered `functions.c` source uses `strncpy(out, alphabet + i, 1)` and `strncpy(out, alphabet + tens, 1)`, followed by caller-managed terminators, providing a concrete source-level use consistent with bounded copying. A search of the public `alphasmart-research` tree produced no direct `strncpy` match; that is recorded as a contextual negative rather than contradictory evidence.

All three canonical ROMs were then re-read in the current pass. Full ROM hashes matched the canonical manifest, the handlers re-extracted at the offsets above, and all three again produced the exact 0x3E-byte sequence and handler SHA-256. Control flow confirms that entry branches to the count gate before the first copy. `num == 0` therefore performs no source or destination access; a positive count copies through the first NUL and pads the remainder with zero bytes, while count exhaustion on a non-NUL byte terminates without adding an extra NUL. The original destination is deliberately preserved and returned in `D0.L`. There are no helper or global-state dependencies and no generational differences in the handler.

The official SmartApplet corpus was re-used from the fresh materialization performed in the immediately preceding A398 audit and independently rescanned for the next physical slot. All 30 table-bearing binaries contained the expected C-library sequence exactly once. A39C reproduced exactly 34 executable calls in 12 applets, plus 18 table-bearing negatives and 11 structural negatives. Stable same-run controls reproduced A36C=99, A378=598, A380=130, A384=24, A388=244, A390=477 and A398=48. Direct firmware JSR counts reproduced 1/1/8 with no direct JMPs.

A new static regression initially encoded the count-gate assertion using physical byte ordering rather than control-flow ordering. The assertion was corrected to verify the entry `BRA` to the `TST.L` gate and the `BHI` edge back to the copy block. This was a regression-specification correction, not a firmware contradiction. The corrected SOURCE-FIRST regression executed with **OVERALL PASS**. Dynamic/emulator-first regression remains **SPECIFIED / NOT EXECUTED**.

Classification: **CONFIRMED / CLOSED A / SOURCE_FIRST / PUBLISHED**. A3A0/index232 is released as the next sequential audit block.
