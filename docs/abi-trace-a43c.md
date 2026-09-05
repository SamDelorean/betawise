# ABI trace — A43C / index 271

Status: **MECANICA_RECONSTRUIDA A / SOURCE_FIRST / PUBLICADO**.

SOURCE-FIRST revalidation of OS3K A-line entry `A43C`.

- Disposition: unresolved return interpretation is an evidence limit, not a sequencing blocker.
- Historical correlation: `betawise-master/os3k/syscall.c` establishes neutral placeholder `SYS_A43C` at index 271; `os3k.h` provides no independent A43C prototype or vendor semantic name.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: callable handler at runtime `0x00442F86`, file offset `0x032F86`.
- Handler: `0xCC` bytes, SHA-256 `512c14da95b3e86859ea281f446cfc1bc7492ac2aa5b75fbf5886cb35fa11bae`.
- External stack ABI: five 32-bit slots; slot 4 is consumed through its low byte.
- Incoming `A1` is preserved and used as the destination for a terminal six-byte copy.
- Material aggregate layout: byte at offset 0, one uninitialized/padding-compatible byte at offset 1, and a 32-bit value at offset 2.
- The 32-bit field is mechanically `$1407A - arg1`; `D0.L` also contains that delta on exit.
- Related NEO 2013 family A43C/A440/A444/A448/A44C repeats the preserved-`A1` plus terminal-six-byte-copy pattern.
- GCC-like hidden structure return through A1 remains a **strong inference**; a proprietary A1 output convention remains mechanically possible.
- A scalar contractual interpretation of D0 is therefore not published.
- Direct firmware xrefs to A43C remain zero direct JSR/JMP/BSR targets.
- Fresh canonical-ROM directed static regression on 2026-09-05: **41/41 PASS**; helper/callback hashes and the terminal A1 copy were revalidated directly.
- Dynamic/emulator-first validation: **specified, not executed**.
- Public callable header: intentionally not emitted while the source-level A1/return convention remains unproved.

Private ROM bytes, full disassembly, helper/callback analysis and regression workpapers remain outside the repository.
