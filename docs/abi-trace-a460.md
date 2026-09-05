# OS3K ABI trace — A460 / index 280

Status: **MECANICA_CERRADA A / SOURCE_FIRST / NO_CONTRATO_CALLABLE**.

## Source correlation

The recovered BetaWise/ioma8 syscall mapping omits index 280: it maps A454/index277 and resumes at A468/index282. No `SYS_A460` definition or independent prototype was recovered. This negative source evidence agrees with the canonical ROMs and official SmartApplet corpus.

## Canonical ROM evidence

| Platform | Vector/file slot | Raw value | Disposition |
|---|---:|---:|---|
| AlphaSmart 3000 (2005) | `file+0x02B07C` | `0x07090501` | outside the mapped ROM/runtime window; non-callable |
| NEO (2005) | `file+0x0319A6` | `0x03040501` | outside the mapped ROM/runtime window; non-callable |
| NEO (2013) | `file+0x03F5BE` | `0x00000000` | null vector |

The 2005 images are in the compact-data region following the preceding callable range. In NEO 2013, indices 278–281 are four consecutive null entries and index 282 (`A468`) resumes with a valid code pointer.

## Official SmartApplet corpus

The canonical 41-applet sweep contains no valid A460 stub/caller. All 16 EXTENDED applets expose the late callable sequence `A450, A454, A468, A46C`, physically omitting A458–A464. Fourteen LEGACY applets end before this range and eleven have no A-line table.

The A460-specific ROM regression is `23/23 PASS`; the shared 41/41 negative caller/stub manifest was revalidated at the adjacent A458 closure. Dynamic execution is not applicable because no executable target exists.

## Mechanical disposition

A460/index280 is an **ABI hole / non-callable slot** for the canonical corpus. No handler, arguments, return contract, globals/helpers, semantic function name, or callable prototype is asserted.

ABI tooling and emulation should model this number as unavailable in these canonical generations. This conclusion remains corpus-bounded and does not rule out reuse in unavailable builds.
