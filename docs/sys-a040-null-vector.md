# OS3K A040 — null vector closure

A040 (A-line index 16) is not exposed as a callable service by the three canonical OS3K firmware images used by this project.

Primary firmware evidence shows that A040's vector entry is exactly zero in AS3000 2005, NEO 2005, and NEO 2013. The same ROM rematerialization and SHA-256 verification used for the immediately preceding A03C audit reads the real A-line tables at file offsets `0x02AC1C`, `0x031546`, and `0x03F15E`; slot 16 is `0x00000000` in all three images. Neighboring audited entries remain correctly aligned, so this is not a table-location artifact.

Source-first correlation found no documented A040 wrapper, prototype, or recovered vendor name in the historical SDK/BetaWise material used by the reconstruction. The ABI map had therefore preserved A040 as an unknown gap rather than assigning a function by numeric continuity.

Because the vector is null, there is no handler address, 68k handler body, CFG, argument list, return value, or side-effect contract to publish for these firmware generations. The previously audited A-line dispatcher does not establish a vendor-defined semantic meaning for a null slot, so this document deliberately does not label A040 as a reserved service, unsupported operation, error, or crash contract.

Status: **mechanically closed / source-first revalidated / null vector confirmed**. No callable prototype is published. The original vendor purpose or label for the slot remains unknown. ROM bytes and private reverse-engineering workpapers are intentionally omitted.
