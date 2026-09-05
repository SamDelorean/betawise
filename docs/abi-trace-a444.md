# ABI trace — A444 / index 273

Status: **MECANICA_RECONSTRUIDA A / SOURCE_FIRST / PUBLICADO**.

- Historical correlation: `betawise-master/os3k/syscall.c` establishes neutral placeholder `SYS_A444` at index 273; `os3k.h` contains no independent vendor prototype.
- AS3000 2005 and NEO 2005: non-pointer/unresolved vector values; no callable handler attributed.
- NEO 2013 handler: runtime `0x0044313A..0x00443213`, length `0xDA`, SHA-256 `beeaa6288ce535f5a31b1e54559274ea2018d15662ada66c1cf68b0196610a14`.
- External stack evidence: four 32-bit slots. Slot 3 is mechanically signed 16-bit; slot 4 is a writable 16-bit output-count-like pointer.
- Incoming `A1` is preserved and used as the destination for a terminal six-byte aggregate result; the sole direct caller confirms this result channel and ignores D0.
- Functional behavior: bounded single-byte XML/entity-reference decoding is **confirmed** from primary firmware evidence. Ordinary bytes copy directly; `&...;` references decode the predefined names `amp`, `lt`, `gt`, `apos`, `quot`, plus decimal and `x`-prefixed hexadecimal numeric references. Numeric results are emitted only below `0x100`.
- Destination output is NUL-terminated. The output-count pointer is cleared on the early path and receives the produced-length delta on the normal path.
- `D0` as a coherent stable scalar return is **refuted** by path-dependent values and caller non-consumption.
- The source-level descriptor type and vendor state semantics remain unknown. These are evidence limits, not sequencing blockers.
- Fresh canonical-ROM directed static regression on 2026-09-05: **32/32 PASS**, including handler, entity helper/table, sole caller wrapper and negative absolute xrefs.
- Dynamic/emulator validation: **specified, not executed**.
- No callable C header or invented vendor semantic name is emitted.

Private ROM bytes, full correlated disassembly and regression workpapers remain outside the repository.
