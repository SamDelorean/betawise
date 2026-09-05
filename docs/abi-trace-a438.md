# ABI trace — A438 / index 270

Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**.

Source-first revalidation of OS3K A-line entry `A438`.

- Historical correlation: `betawise-master/os3k/syscall.c` maps index 270 to neutral placeholder `SYS_A438`; no vendor semantic name or independent prototype is present in the recovered public header.
- AS3000 2005: non-callable table value `0x5A06004E`.
- NEO 2005: non-callable table value `0x7F08005E`.
- NEO 2013: handler at runtime `0x0043496A`, file offset `0x02496A`.
- Handler: `0x16` bytes, SHA-256 `e1dce54eff2e86dbf882576bdea0a01f636575281e30faaa41728242a8c87f42`.
- Mechanical ABI: one external 32-bit destination pointer.
- The handler calls the already reconstructed `strcpy` implementation with `dst=arg1` and fixed source `(const char *)0x00012C70`.
- No write to `D0` occurs after `strcpy`; therefore `D0.L` returns the original destination pointer.
- Neutral mechanically justified contract: `char *SYS_A438(char *dst)`.
- Direct firmware xrefs to A438: zero; instruction-level `PEA $00012C70` references: three.
- The vendor meaning of the fixed global buffer remains unknown and is not required for the ABI contract.
- Fresh canonical-ROM directed static regression on 2026-09-05: **22/22 PASS**.
- Dynamic validation: **not executed**.

Private ROM bytes and extended disassembly remain outside the repository.
