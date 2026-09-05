# ABI trace — A448 / index 274

Status: **MECANICA_RECONSTRUIDA A / SOURCE_FIRST / PUBLICADO**.

- Historical correlation: `betawise-master/os3k/syscall.c` establishes neutral placeholder `SYS_A448` at index 274; `os3k.h` contains no independent vendor prototype.
- AS3000 2005 and NEO 2005: non-pointer/unresolved vector values; no callable handler attributed.
- NEO 2013 handler: runtime `0x00443214..0x004432AF`, length `0x9C`, SHA-256 `480cd19d43b2fc3535ede8bd7b600845217cc3dea3a4f337d0eed6bd63f7e3a8`.
- External ABI: five 32-bit stack slots plus a six-byte aggregate result through incoming `A1`.
- Mechanical slot roles: input descriptor span; exact NUL-string selector/filter; writable array of 16-byte four-long descriptor records; effective signed-8 record capacity; writable one-byte accepted-record count.
- Parser correlation: common core recognizes XML/markup syntax and shares the entity-reference helper independently established in A444.
- Predicate callback compares the supplied selector against a parser-maintained NUL string with a strcmp-like helper and accepts exact equality.
- Descriptor callbacks write records at `base + count*16`, fields 0/4/8/12, and increment count only after predicate acceptance.
- Direct NEO 2013 xrefs: exactly two `BSR.W` callers at `0x004432D0` and `0x0044333C`; zero direct absolute JSR/JMP.
- Both callers pass literal capacity 2, local descriptor storage and count, prepare A1 to a six-byte aggregate, ignore D0, then consume the first descriptor through A440 or A444.
- State 8 is mechanically tied to the record-capacity condition; no vendor enum name is assigned.
- `D0.L` duplicates an aggregate detail delta, but no recovered caller consumes it independently, so a separate scalar-return contract remains unproved.
- Selector meaning (tag/path/other structural key) and descriptor field names remain evidence-limited, not sequencing-blocking.
- Fresh canonical-ROM directed static regression on 2026-09-05: **32/32 PASS**.
- Dynamic/emulator validation: **specified, not executed**.
- No callable C header or invented vendor semantic name is emitted.

Private ROM bytes, full parser/callback disassembly and regression workpapers remain outside the repository.
