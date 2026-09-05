# ABI trace — A440 / index 272

Status: **MECANICA_RECONSTRUIDA A / SOURCE_FIRST / PUBLICADO**.

SOURCE-FIRST revalidation of OS3K A-line entry `A440`.

- Unresolved source-level semantics are an evidence limit, not a sequencing blocker.
- Historical correlation: `betawise-master/os3k/syscall.c` establishes neutral placeholder `SYS_A440` at index 272; `os3k.h` contains no independent A440 prototype.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: runtime `0x00443052`, file offset `0x033052`.
- Handler: `0xE8` bytes, SHA-256 `f312ec08cdf5791a160f4ce68ca81ff16dc50a13f727dc380e3a306319a3fc02`.
- External ABI: five 32-bit stack slots.
- Effective mechanics: arg1 points to at least two readable longwords; arg2 is pointer-like byte/NUL input; arg3 is a writable byte buffer; arg4 is mechanically signed 16-bit; arg5 is a pointer receiving a conditional 16-bit write outside the early path.
- Incoming `A1` is preserved and receives a terminal six-byte aggregate result.
- Contractuality of the A1 aggregate channel is **confirmed** by the sole direct caller in A44C: it prepares a six-byte local in A1, pushes five stack arguments, calls A440 at runtime `0x00443304`, discards 20 bytes, ignores D0, and later forwards the six-byte local through its own saved A1.
- Stable scalar D0 return is **refuted**: early and normal paths leave incompatible internal values while the aggregate result is independently formed through A1, and the caller ignores D0.
- Direct xrefs: one `BSR.W` caller, zero direct absolute JSR/JMP.
- Fresh canonical-ROM directed static regression on 2026-09-05: **29/29 PASS**.
- Dynamic/emulator validation: **specified, not executed**.
- Source-level meanings of arguments, status codes and private globals remain unknown; no semantic function name or callable C header is emitted.

Private ROM bytes, full disassembly, callback/helper analysis and regression workpapers remain outside the repository.
