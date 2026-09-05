# ABI trace — A440 / index 272

SOURCE-FIRST revalidation of OS3K A-line entry `A440`.

- Disposition: `MECANICA_CERRADA A / SOURCE_FIRST`; unresolved source-level semantics are an evidence limit, not a sequencing blocker.
- Historical correlation: recovered BetaWise/ioma8 mapping establishes `SYS_A440` at index 272; no independent vendor prototype was recovered.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: runtime `0x00443052`, file offset `0x033052`.
- Handler: `0xE8` bytes, SHA-256 `f312ec08cdf5791a160f4ce68ca81ff16dc50a13f727dc380e3a306319a3fc02`.
- External ABI: five 32-bit stack slots.
- Effective mechanics: arg1 points to at least two readable longwords; arg2 is pointer-like byte/NUL input; arg3 is a writable byte buffer; arg4 is mechanically signed 16-bit (`MOVE.W` + sign extension); arg5 is a pointer receiving a conditional 16-bit write outside the early path.
- Incoming `A1` is preserved and receives a terminal six-byte aggregate result: byte at offset 0, one uninitialized/padding-compatible byte at offset 1, and a 32-bit field at offset 2.
- Contractuality of the A1 aggregate channel is confirmed by the sole direct caller in A44C: it prepares a six-byte local in A1, pushes five stack arguments, calls A440 through `BSR.W` at runtime `0x00443304`, discards exactly 20 bytes, ignores D0, and later forwards the six-byte local through its own saved A1.
- Stable scalar D0 return is refuted: early and normal paths leave incompatible internal values while the aggregate result is independently formed through A1, and the caller ignores D0.
- Direct xrefs: one `BSR.W` caller, zero direct absolute JSR/JMP.
- Fresh directed static regression: `35/35 PASS`.
- Dynamic/emulator validation: not executed.
- Source-level meanings of the five arguments, status codes and private globals remain unknown; no semantic function name or callable C header is emitted.

Private ROM bytes, full disassembly, callback/helper analysis and regression workpapers remain outside the repository.
