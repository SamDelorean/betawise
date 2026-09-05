# ABI trace — A418 / index 262

Source-first traceability summary for the OS3K ABI audit.

- Disposition: `MECÁNICA_RECONSTRUIDA A / SOURCE_FIRST / RETORNO_CONTRACTUAL=DESCONOCIDO`.
- Historical anchors: historical BetaWise `syscall.c` and `ioma8/neo-re` both map index 262 to the neutral placeholder `SYS_A418`; neither source recovers a vendor semantic name or independent prototype.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: callable handler at runtime `0x004342D4`, file offset `0x0242D4`.
- Handler boundary: `0x3A` bytes / 19 instructions / one terminal RTS. A41C points physically backward (`0x00433B86`) and is not used as A418's upper boundary.
- Physical ABI: one 32-bit mutable pointer slot.
- Observed input layout: 16-bit count at `+0`, byte state at `+2`; other fields remain untyped.
- Mechanical effect: if the pointer is non-NULL, iterate the initial count over a private table at base `0x0000F43E`, stride `0x56`, calling helper `0x004427FC` for each record; then clear count and store state byte `5`. NULL input performs neither helper calls nor mutation.
- Private helper: the demonstrated `0x22`-byte helper clears the observed fields of each `0x56`-byte record; no vendor-semantic record or operation name is assigned.
- Direct firmware xrefs: exactly one JSR at runtime `0x00442F6A` / file `0x032F6A`; zero direct JMP/BSR. The caller supplies one pointer and immediately overwrites D0 after the call.
- Return contract: `DESCONOCIDO`. D0 is path-dependent/residual (NULL, zero-count and positive-count paths do not form a uniform scalar result); `void` remains an inference about intent, not a confirmed contract.
- Official SmartApplet exposure: structurally absent (`0/41`) because the validated tables end before A418.
- Static regression: historical corrected runs `22/22` and `26/26 PASS`; fresh source-first primary revalidation `41/41 PASS / OVERALL PASS`. A fresh preliminary `40/41` run is invalidated because its test incorrectly expected a byte clear at record `+2`; the firmware correctly performs a word clear and the corrected suite was rerun from zero.
- Dynamic/emulator-first regression: `ESPECIFICADA / NO EJECUTADA`.
- Public callable header: intentionally not published.

Private ROM extraction, raw handler/helper bytes, extensive disassembly, caller traces and regression workpapers remain outside the repository.
