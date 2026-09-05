# ABI trace — A41C / index 263

Source-first traceability summary for the OS3K ABI audit.

- Disposition: `MECÁNICA_CERRADA A / SOURCE_FIRST / PUBLICADO`.
- Historical anchors: historical BetaWise `syscall.c` and `ioma8/neo-re` both map index 263 to the neutral placeholder `SYS_A41C`; neither source recovers a vendor semantic name or independent prototype.
- Neutral callable contract: `void *SYS_A41C(uint8_t selector)`.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: callable handler at runtime `0x00433B86`, file offset `0x023B86`.
- Handler: `0x92` bytes / 52 instructions / one terminal RTS; SHA-256 `ab0942b2bd308ec22364abbd82bd419f0c5b6bebe884cbb4702e70828d25f255`.
- Physical ABI: one 32-bit stack slot; only its low byte is semantically consumed. The selector helper independently re-reads the low byte and accepts the raw range `5..21`, so `uint8_t` preserves the demonstrated input domain without inventing an enum.
- Return: NULL when the destination-slot allocator or descriptor selector fails; success explicitly returns the firmware-owned destination structure pointer in D0.L.
- Private mechanics: allocator `0x00434298` searches 20 private slots at base `0x00007E92`, stride `0x5E0`; selector helper `0x00434852` maps raw selector bytes `5..21` to private descriptors. A successful call initializes state byte `5` plus the selector and builds at most five destination records (`0x12C` stride) from private descriptors (`0x0C` stride) through helper `0x00441A4A`.
- Direct firmware callers: exactly four JSRs, with concrete selector values `7`, `8`, `10`, `10`; all provide positive pointer-return evidence by storing, moving into an address register and/or dereferencing D0 after the call. Direct JMP/BSR references are zero.
- Adversarial checks: `void` is refuted by four pointer consumers and the explicit A3/NULL return; an integer return is refuted by address-register use/dereference. Vendor-semantic names for the selector, destination object and descriptor family remain unknown and are intentionally not invented.
- Official SmartApplet exposure: structurally absent (`0/41`) because this late slot is beyond the validated applet table.
- Static regression: historical `42/42 PASS`; fresh source-first primary revalidation `51/51 PASS / OVERALL PASS`. A fresh preliminary `46/51` run is invalidated because five test assertions used incorrect local patterns/offsets; after correcting those test-only transcription errors, the entire suite was rerun from zero and passed.
- Dynamic/emulator-first regression: `ESPECIFICADA / NO EJECUTADA`.
- Public header: `os3k/sys_a41c.h`.

Private ROM extraction, raw handler/helper bytes, extensive disassembly, helper correlation, caller traces and regression workpapers remain outside the repository.
